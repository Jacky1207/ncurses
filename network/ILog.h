#ifndef _ILOG_H_
#define _ILOG_H_

#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <list>

#include "typedef.h"
#include "IMutex.h"
#include "IRunable.h"
#include "IThread.h"

namespace IObject
{
	#define LOG_WRITE(mode,level,format,...)  ISystemLog::getInstance()->writeLog(mode,level,"[%d](%s):" format,__LINE__,__FUNCTION__,##__VA_ARGS__)

	namespace ISystemBase
	{
		enum LOG_LEVEL
		{
			LOG_SYSTEM = 0,
			LOG_ERROR,
			LOG_WARNING,
			LOG_INFO,
			LOG_DEBUG,
			LOG_HIDE
		};

#define MAX_LOG_LEN		( 10 * 1024 )
#define MAX_LOG_SIZE	( 10 * 1024 * 1024 )

		class ILogImpl 
		{
		public:
			ILogImpl();
			virtual ~ILogImpl();

			virtual void writeLogImpl( const char* strLog, int len);
			void setLogPathImpl(std::string strPath){ m_strPath = strPath; }
			virtual std::string getCurrentTime();
		private:
			FILE *m_pFile;
			sg_string_t m_strFileName;
			sg_string_t m_strPath;

			FILE *getCurrentLogHandle();
			long getFileSize(const std::string fileName);
			void resetFileHandle();
		};


		class ISystemLog : virtual public ILogImpl, virtual public IRunable
		{
		public:
			ISystemLog();
			virtual ~ISystemLog();

			static ISystemLog* getInstance();
			virtual void writeLog(std::string module, int level, const std::string strLog, ...);
			virtual void setLogPath(const std::string strPath){ setLogPathImpl(strPath); }

			virtual void run();
			virtual void stop(){ isActive = false; }
		private:
			bool isActive;
			std::list <std::string >m_strLogList;
			std::string getFirstLog();
			int getConfigLogLevel();

			IMutex m_pLock;
			IThread _pThread;
		};

	}
}
#endif
