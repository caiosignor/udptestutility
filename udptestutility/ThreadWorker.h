#pragma once

#include "IniFileParser.h"
#include <chrono>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

class ThreadWorker
{
public:
	ThreadWorker(const ConnectionConfig& config);
	~ThreadWorker();

	inline void Join()
	{
		if(m_threadHandler.joinable())
			m_threadHandler.join();
	}

private:
	void RunLoop();

	ConnectionConfig m_config;
	std::chrono::milliseconds m_period;
	std::chrono::steady_clock::time_point m_nextTimeToExecute{};
	int m_socket;
	std::thread m_threadHandler;

	sockaddr_in m_destInfo;
};

