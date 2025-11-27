#pragma once
#include <iostream>
#include <list>
#include <string>
#include "IniFileParser.h"

/**
 * @class ConfigurationManager
 * @brief Singleton responsible for loading and storing all ConnectionConfig entries.
 *
 * This class wraps IniFileParser and guarantees that configuration data
 * is loaded only once and stored safely for global use.
 */
class ConfigurationManager
{
public:

    /**
     * @brief Retrieves the singleton instance.
     *
     * Uses thread-safe static initialization (C++11 guaranteed).
     *
     * @return Reference to the global ConfigurationManager instance.
     */
    static ConfigurationManager& Instance();

    /**
     * @brief Loads and parses an INI configuration file.
     *
     * If the method is called more than once, previously loaded
     * configurations will be cleared and replaced.
     *
     * @param filename Path to the INI file.
     * @return true if the file was loaded successfully, false otherwise.
     */
    bool Initialize(const std::string& filename);

    /**
     * @brief Provides read-only access to the parsed configuration list.
     *
     * @return Const reference to the list of loaded ConnectionConfig objects.
     */
    const std::list<ConnectionConfig>& GetConfiguration() const;

private:

    /**
     * @brief Private constructor for Singleton pattern.
     */
    ConfigurationManager() = default;

    /**
     * @brief Deleted copy constructor to prevent duplication.
     */
    ConfigurationManager(const ConfigurationManager&) = delete;

    /**
     * @brief Deleted assignment operator to prevent duplication.
     */
    ConfigurationManager& operator=(const ConfigurationManager&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    ConfigurationManager(ConfigurationManager&&) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    ConfigurationManager& operator=(ConfigurationManager&&) = delete;

private:
    std::list<ConnectionConfig> m_loadedConfigs; /**< Stored configuration entries. */
};
