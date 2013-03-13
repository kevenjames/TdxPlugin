#include "Log.h"
#include <stdarg.h>
#include <windows.h>


namespace TdxPlugin
{
	FILE* CLog::m_hLogFile = NULL;
	LogLevel CLog::m_LogLevel = LOG_TRACE;
	string CLog::m_strFilePath = LOG_FILE_PATH;

	const char* LogLevelStr[] = {
		"TRACE",
		"INFO",
		"WARNING",
		"ERROR",
		"CRITICAL",
		"FATAL",
	};

	CLog::CLog()
	{
		Initialise(false);	
	}

	CLog::~CLog()
	{
		Dispose();
	}

	void CLog::Initialise(bool bAppend)
	{
		if (m_strFilePath.length() > 0)
		{
			m_hLogFile = bAppend ? fopen(m_strFilePath.c_str(), "a+") : fopen(m_strFilePath.c_str(), "w+");
		}
	}

	void CLog::Dispose()
	{
		if (NULL != m_hLogFile)
		{
			fflush(m_hLogFile);
			fclose(m_hLogFile);
			m_hLogFile = NULL;
		}
	}

	CLog& CLog::Instance()
	{
		static CLog logInstance;
		return logInstance;
	}

	void CLog::SetLogFilePath(const string& strFilePath, bool bAppend)
	{
		m_strFilePath = strFilePath;
		Dispose();
		Initialise(bAppend);
	}

	void CLog::SetLogLevel(const LogLevel level)
	{
		m_LogLevel = level;
	}

	void CLog::Log(const LogLevel level, const char* Format, ...)
	{
		if (m_LogLevel > level)
			return;

		if (NULL == m_hLogFile)
			return;
		
		char szBuffer[1024];

		va_list args;
		va_start(args, Format);
		vsprintf(szBuffer, Format, args);
		va_end(args);

		SYSTEMTIME st;
		GetLocalTime(&st);
		if ( 0 > fprintf( m_hLogFile, "[%02u:%02u:%02u:%03u]\t[%s]\t%s\n", 
			st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, LogLevelStr[level], szBuffer))
		{
			Dispose();
		}
		else
		{
			fflush(m_hLogFile);
		}
	}


};
