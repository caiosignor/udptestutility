#pragma once
#include <iostream>
#include "IniFileParser.h"

class ConfigurationManager
{
public:
	static ConfigurationManager& Instance();

	bool Initialize(const std::string& filename);

	const std::list<ConnectionConfig>& GetConfiguration() const;
private:
	std::list<ConnectionConfig> m_loadedConfigs;
};

