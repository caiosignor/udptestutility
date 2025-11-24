#pragma once
#include <iostream>
#include "IniFileParser.h"

class ConfigurationManager
{
public:
	static ConfigurationManager& Instance();

	bool Initialize(const std::string& filename);

	void GetConfiguration();
private:
	std::list<Connection> m_loadedConfigs;
};

