#include "ConfigurationManager.h"
#include "IniFileParser.h"

ConfigurationManager& ConfigurationManager::Instance()
{
	static ConfigurationManager instance;
	return instance;
}

bool ConfigurationManager::Initialize(const std::string& filename)
{
	IniFileParser configFile(filename, m_loadedConfigs);
	return false;
}

void GetConfiguration()
{

}