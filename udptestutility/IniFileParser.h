#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <list>

typedef struct _Connection{
	size_t payload_length;
	uint8_t* payload;
	std::string destination_ip;
	uint16_t destination_port;
	uint16_t rate;

	_Connection() :
		payload_length(0)
		, payload(nullptr)
		, destination_ip("")
		, destination_port(0)
		, rate(0)
	{
	}

	void Print()
	{
		std::cout << "PayloadLength = " << payload_length << std::endl;
		if (payload != nullptr)
		{
			std::cout << "payload = " << payload << std::endl;
		}
		std::cout << "destination_ip = " << destination_ip << std::endl;
		std::cout << "destination_port = " << destination_port << std::endl;
		std::cout << "rate = " << rate << std::endl;
	}

}ConnectionConfig;

class IniFileParser
{
public:
	IniFileParser(const std::string& filename, std::list<ConnectionConfig>& out);
private:
	bool ParseFile();

	void tokenizerString(const std::string& input, std::string& key, std::string& value);

	static const char* m_validEntries[];

	std::ifstream m_file;

	bool m_succesfullRead;

	std::list<ConnectionConfig>& m_outputList;
};

