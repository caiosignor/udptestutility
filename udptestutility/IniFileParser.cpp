#include "IniFileParser.h"
#include <iostream>
#include <fstream>
#include <string>

IniFileParser::IniFileParser(const std::string& filename, std::list<ConnectionConfig>& out) :
    m_file(filename),
    m_outputList(out)
{
    // Create a mapping between INI keys and struct-member setter functions.
    // Each key maps to a lambda that modifies the current ConnectionConfig object.
    m_dispatch["[Connection]"] = [&](ConnectionConfig*& current, const std::string&)
        {

            // First connection found — instantiate it
            if (current == nullptr)
            {
                current = new ConnectionConfig();
            }
            else
            {
                // New connection header encountered while parsing another connection
                // → push previous connection and start a new one
                m_outputList.push_back(*current);
                delete current;
                current = new ConnectionConfig();
            }
        };

    m_dispatch["payload_length"] = [](ConnectionConfig*& current, const std::string& value) { current->SetPayloadLength(value); };
    m_dispatch["payload"] = [](ConnectionConfig*& current, const std::string& value) { current->SetPayload(value); };
    m_dispatch["destination_ip"] = [](ConnectionConfig*& current, const std::string& value) { current->SetDestinationIp(value); };
    m_dispatch["destination_port"] = [](ConnectionConfig*& current, const std::string& value) { current->SetDestinationPort(value); };
    m_dispatch["sending_period"] = [](ConnectionConfig*& current, const std::string& value) { current->SetRate(value); };

    // Ensure output list starts empty
    m_outputList.clear();

    if (m_file.is_open())
    {
        parseFile();
        m_file.close();
    }
    else
    {
        std::cout << "Failed to open file: " << filename
            << " (error flags=" << m_file.rdstate() << ")\n";
    }
}

void IniFileParser::parseFile()
{
    std::string line;
    ConnectionConfig* connection = nullptr;

    while (std::getline(m_file, line))
    {
        // Ignore empty and commented lines
        if (line.empty() || line[0] == '#')
            continue;

        std::string key, value;

        // Special case: section header
        if (line == "[Connection]")
        {
            key = line;
        }
        else
        {
            tokenizerString(line, key, value);
        }

        // Search for a handler for the extracted key
        auto handler = m_dispatch.find(key);

        if (handler == m_dispatch.end())
        {
            if (!key.empty())
                std::cout << "Unknown parameter in INI file: " << key << std::endl;
        }
        else
        {
            handler->second(connection, value);
        }
    }

    // Store final connection if the file ends while parsing one
    if (connection != nullptr)
    {
        m_outputList.push_back(*connection);
        delete connection;
    }
}

void IniFileParser::tokenizerString(const std::string& input, std::string& key, std::string& value)
{
    // Helper lambda to trim whitespace and remove comments
    auto trimStr = [](std::string& s)
        {
            // Remove inline comments
            auto pos = s.find('#');
            if (pos != std::string::npos)
                s = s.substr(0, pos);

            // Trim left side
            s.erase(s.begin(),
                std::find_if(s.begin(), s.end(), [](char c) { return !std::isspace(c); }));

            // Trim right side
            s.erase(
                std::find_if(s.rbegin(), s.rend(), [](char c) { return !std::isspace(c); }).base(),
                s.end()
            );
        };

    auto pos = input.find('=');
    if (pos == std::string::npos)
    {
        // Invalid key/value format (line without '=')
        key.clear();
        value.clear();
        return;
    }

    key = input.substr(0, pos);
    value = input.substr(pos + 1);

    // Remove extra spaces and inline comments
    trimStr(key);
    trimStr(value);
}