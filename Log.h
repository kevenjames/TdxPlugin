#ifndef _LOG_H
#define _LOG_H

#include <string>
#define LOG_FILE_PATH "D:\\new_zszq\\Log\DebugInfo.log"

#ifdef TEST
	#define LogData Logger::Instance().log
	#define SetLogFile Logger::SetLogFilePath
#else
	#define LogData
	#define SetLogFile
#endif

using namespace std;

namespace TdxPlugin 
{
	typedef enum tagLogLevel
	{
		LOG_TRACE = 0,
		LOG_INFO,
		LOG_WARNING,
		LOG_FATAL,
		LOG_NONE,
	}LogLevel

	class CLog
	{
	public:
		static CLog& Instance();
		static void SetLogFilePath(const string& strFilePath, bool bAppend);
	
	};
}

#endif //_LOG_H