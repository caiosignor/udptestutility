#include "IniFileParser.h"
#include <iostream>
#include <fstream>
#include <string>

const char* IniFileParser::m_validEntries[] = {
		"[Connection]",
		"payload_length",
		"payload",
		"destination_ip",
		"destination_port"
};

IniFileParser::IniFileParser(const std::string& filename, std::list<Connection>& out) :
	m_file(filename)
	, m_succesfullRead(false)
	, m_outputList(out)
{
	m_outputList.clear();

	if (m_file.is_open())
	{
		m_succesfullRead = ParseFile();
		m_file.close();
	}
	else
	{
		std::cout << "Failed to read " << filename << " err code " << m_file.rdstate() << std::endl;
		m_succesfullRead = false;
	}

	
}

bool IniFileParser::ParseFile()
{
	std::string line;

	Connection* connection = nullptr;

	while (std::getline(m_file, line))
	{
		if (line.empty() || line[0] == '#')
			continue; // ignore empty a commented lines

		//identified a new connection
		if (!line.compare(m_validEntries[0]))
		{
			//the first connection
			if (connection == nullptr)
			{
				connection = new Connection();
			}
			else //happens when a new connection was identified during parsing a previous connection. In this case, we shall close the previous connection; and start a new one
			{
				m_outputList.push_back(*connection);

				connection->Print();

				delete connection;
				connection = new Connection();
			}
			
			//std::cout << line << std::endl;
		}
		else //load parameters
		{
			size_t totalEntries = std::size(m_validEntries);
			std::string key, value;
			tokenizerString(line, key, value);

			//i = 0, already covered in previous if. 0 reserved for [Connection]
			for (int i = 1; i < totalEntries; i++)
			{
				if (!key.compare(m_validEntries[i]))
				{
					switch (i)
					{
					case 0:break; //[Connection]; ignore from here.
					case 1: connection->payload_length = static_cast<size_t>(std::stoi(value));break; //payload_length;
					case 2: connection->payload = new uint8_t(value.size());
						memcpy(connection->payload, value.c_str(), value.size());
						break; //payload;
					case 3: connection->destination_ip = value; break; //destination_ip;
					case 4: connection->destination_port = static_cast<size_t>(std::stoi(value));break; //destination_port;
					default:
						break;
					}
				}
			}
		}

	}

	//if reach the end of the file. Store the current connection config to list
	if (connection != nullptr)
	{
		connection->Print();
		m_outputList.push_back(*connection);
		delete connection;
	}

	return false;
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