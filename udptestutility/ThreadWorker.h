#pragma once

#include "IniFileParser.h"
#include <chrono>

class ThreadWorker
{
public:
	ThreadWorker(const ConnectionConfig& config);

	void RunLoop();

private:
	ConnectionConfig m_config;
	bool m_isValid;
	std::chrono::milliseconds m_period;
	std::chrono::steady_clock::time_point m_nextTimeToExecute{};
};

