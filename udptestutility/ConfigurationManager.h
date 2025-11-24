#pragma once
class ConfigurationManager
{
	static ConfigurationManager& Instance();

	bool Initialize();

	void GetConfiguration();
};

