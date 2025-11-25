#pragma once

#include "IniFileParser.h"
#include <chrono>
#include <winsock2.h>
#include <ws2tcpip.h>

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
	int m_socket;

	sockaddr_in m_destInfo;
};

