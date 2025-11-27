#include "ConfigurationManager.h"
#include <iostream>

ConfigurationManager& ConfigurationManager::Instance()
{
    static ConfigurationManager instance;
    return instance;
}

bool ConfigurationManager::Initialize(const std::string& filename)
{
    // Clear old configuration if re-initializing
    m_loadedConfigs.clear();

    // Parse the file and populate m_loadedConfigs
    IniFileParser parser(filename, m_loadedConfigs);

    // If parser loaded nothing, treat it as a failure
    if (m_loadedConfigs.empty())
    {
        std::cout << "ConfigurationManager: No valid ConnectionConfig entries loaded from file: "
            << filename << std::endl;
        return false;
    }

    return true;
}

const std::list<ConnectionConfig>& ConfigurationManager::GetConfiguration() const
{
    return m_loadedConfigs;
}
