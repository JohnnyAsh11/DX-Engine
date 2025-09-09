#ifndef __LOGGER_H_
#define __LOGGER_H_

// 
#define DEBUG_LOG 0
#define INFO_LOG 1

#include<string>

/// <summary>
/// Formats logging out to the console window.
/// </summary>
class Logger
{
private:
	static Logger* m_pInstance;

public:
	/// <summary>
	/// Gets the single instance of the Logger.
	/// </summary>
	static Logger* GetInstance();

	/// <summary>
	/// Logs a passed in message to the console.
	/// </summary>
	/// <param name="a_sFile">The file being logged from.</param>
	/// <param name="a_sMessage">The message being printed.</param>
	/// <param name="a_uLogFlag">Whether or not this message is debug or info.</param>
	void Log(std::string a_sFile, std::string a_sMessage, unsigned int a_uLogFlag);

	/// <summary>
	/// Frees up the memory taken up by the Logger singleton.
	/// </summary>
	static void Release();
private:
	Logger();
	~Logger();

	// Removing the copy constructor and operator.
	Logger(const Logger& a_Other) = delete;
	Logger& operator =(const Logger& a_Other) = delete;
};

#endif //__LOGGER_H_
