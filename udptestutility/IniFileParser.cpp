#include "IniFileParser.h"
#include <iostream>
#include <fstream>
#include <string>

const char* IniFileParser::m_validEntries[] = {
		"[Connection]",
		"payload_length",
		"payload",
		"destination_ip",
		"destination_port",
		"sending_rate"
};

IniFileParser::IniFileParser(const std::string& filename, std::list<ConnectionConfig>& out) :
	m_file(filename)
	, m_succesfullRead(false)
	, m_outputList(out)
{
	//create an map to handle each member of struct
	m_dispatch["[Connection]"] = [&](ConnectionConfig*& current, const std::string& value) {

		//if found [Connection] Keywork and current is nullptr, means it is the first connection found. Just instantiate the struct
		if (current == nullptr)
		{
			current = new ConnectionConfig();
		}
		else //happens when a new connection was identified during parsing another connection. In this case, we shall store the last connection; and start a new one
		{
			m_outputList.push_back(*current);
			delete current;
			current = new ConnectionConfig();
		}
	};

	m_dispatch["payload_length"] = [](ConnectionConfig*& current, const std::string& value) {current->SetPayloadLength(value);};
	m_dispatch["payload"] = [](ConnectionConfig*& current, const std::string& value) {current->SetPayload(value);};
	m_dispatch["destination_ip"] = [](ConnectionConfig*& current, const std::string& value) {current->SetDestinationIp(value);};
	m_dispatch["destination_port"] = [](ConnectionConfig*& current, const std::string& value) {current->SetDestinationPort(value);};
	m_dispatch["sending_rate"] = [](ConnectionConfig*& current, const std::string& value) {current->SetRate(value);};

	m_outputList.clear();

	if (m_file.is_open())
	{
		m_succesfullRead = parseFile();
		m_file.close();
	}
	else
	{
		std::cout << "Failed to read " << filename << " err code " << m_file.rdstate() << std::endl;
		m_succesfullRead = false;
	}
}

bool IniFileParser::parseFile()
{
	std::string line;

	ConnectionConfig* connection = nullptr;

	while (std::getline(m_file, line))
	{
		if (line.empty() || line[0] == '#')
			continue; // ignore empty a commented lines

		std::string key, value;

		if (line == "[Connection]")
		{
			key = line;
		}
		else
		{
			tokenizerString(line, key, value);
		}

		auto handler = m_dispatch.find(key);

		if (handler == m_dispatch.end())
		{
			std::cout << "Unkown parameter: " << key << std::endl;
		}
		else
		{
			handler->second(connection, value);
		}
	}

	//if reach the end of the file. Store the current connection config to list
	if (connection != nullptr)
	{
		m_outputList.push_back(*connection);
		delete connection;
	}

	return true;
}

void IniFileParser::tokenizerString(const std::string& input, std::string& key, std::string& value)
{
	auto pos = input.find('=');

	if (pos == std::string::npos)
	{
		//incorrect line
		key.clear();
		value.clear();
		return;
	}

	key = input.substr(0, pos);
	value = input.substr(pos+1);
}