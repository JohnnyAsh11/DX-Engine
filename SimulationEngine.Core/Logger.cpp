#include "Logger.h"

#include <iostream>
#include <time.h>

Logger* Logger::m_pInstance = nullptr;

Logger* Logger::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new Logger();
	}

	return m_pInstance;
}

void Logger::Log(std::string a_sFile, std::string a_sMessage, unsigned int a_uLogFlag)
{
	if (a_uLogFlag == DEBUG_LOG)
	{
		std::cout << "<DEBUG> ";
	}
	else if (a_uLogFlag == INFO_LOG)
	{
		std::cout << "<INFO> ";
	}

	std::cout << "[" << a_sFile << "]: " << a_sMessage << std::endl;
}

void Logger::Release()
{
	if (m_pInstance == nullptr)
	{
		return;
	}

	delete m_pInstance;
}

Logger::Logger() { }
Logger::~Logger() { }
