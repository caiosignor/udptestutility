#include "ConfigurationManager.h"

ConfigurationManager& ConfigurationManager::Instance()
{
	static ConfigurationManager instance;
	return instance;
}

bool ConfigurationManager::Initialize()
{
	return false;
}

void GetConfiguration()
{

}