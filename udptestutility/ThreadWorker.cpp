#include "ThreadWorker.h"
#include <chrono>

// link with Ws2_32.lib
#pragma comment(lib,"Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>

ThreadWorker::ThreadWorker(const ConnectionConfig& config)
	: m_config(config)
	, m_isValid(true)
	, m_period(std::chrono::milliseconds(0))
	, m_nextTimeToExecute(std::chrono::milliseconds(0))
	, m_socket(0)
	, m_destInfo({})
{
	if ((m_config.payload == nullptr) && (m_config.payload_length == 0))
	{
		std::cout << "Invalid payload or payload_length Config! " << std::endl;
		m_isValid = false;
		return;
	}

	if ((m_config.payload == nullptr))
	{
		m_config.payload = new uint8_t[m_config.payload_length]; // Fix: Allocate an array of the correct size
		for (size_t i = 0; i < m_config.payload_length; i++)
		{
			m_config.payload[i] = static_cast<uint8_t>(i & 0xff);
		}
	}
	
	if (m_config.rate != 0)
	{
		m_period = std::chrono::milliseconds(m_config.rate);
	}
	else
	{
		std::cout << "Invalid Rate Config! " << std::endl;
		m_period = std::chrono::milliseconds(0);
		m_isValid = false;
		return;
	}
	WSADATA wsaData = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (m_socket == INVALID_SOCKET)
	{
		std::cout << "Failed to create a socket" << std::endl;
		m_isValid = false;
		return;
	}

	m_destInfo.sin_family = AF_INET;
	m_destInfo.sin_port = htons(m_config.destination_port);
	inet_pton(AF_INET, m_config.destination_ip.c_str(), &m_destInfo.sin_addr);
}

void ThreadWorker::RunLoop()
{
	if (m_isValid)
	{
		using clock = std::chrono::steady_clock;

		auto now = clock::now();

		// Inicializa m_nextTimeToExecute apenas na primeira chamada
		if (m_nextTimeToExecute == clock::time_point{})
			m_nextTimeToExecute = now + m_period;

		if (now >= m_nextTimeToExecute)
		{
			m_nextTimeToExecute += m_period;

			sendto(m_socket, (char*)m_config.payload, m_config.payload_length, 0, (sockaddr*)&m_destInfo, sizeof(m_destInfo));
			/*std::cout << "Executado!" << std::endl;*/
		}
	}
}