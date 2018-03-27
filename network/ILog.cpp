#include "ILog.h"

namespace IObject
{
	namespace ISystemBase
	{
		ILogImpl::ILogImpl() :
			m_strFileName(""),
			m_pFile(NULL),
			m_strPath("./")
		{

		}

		ILogImpl::~ILogImpl()
		{
			if (m_pFile != NULL)
			{
				fflush(m_pFile);
				fclose(m_pFile);
				m_pFile = NULL;
			}
		}

		std::string ILogImpl::getCurrentTime()
		{
			time_t timer = time(NULL);
			struct tm  *p = localtime(&timer);

			char buff[64] = { 0 };
			snprintf(buff,sizeof(buff)-1 , "%04d-%02d-%02d %02d.%02d.%02d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
			return std::string(buff);
		}

		FILE *ILogImpl::getCurrentLogHandle()
		{
			if (m_strFileName == "")
			{
				char buff[1024] = { 0 };
				snprintf(buff, sizeof(buff) ,"%s.txt", getCurrentTime().c_str());
				m_strFileName = m_strPath + std::string(buff);
			}

			m_pFile = fopen((m_strFileName).c_str(), "a");
			if (m_pFile == NULL)
			{
				perror("failed to open log file\n");
			}

			return m_pFile;
		}

		long ILogImpl::getFileSize(const std::string fileName)
		{
			unsigned long filesize = -1;
			struct stat statbuff;
			if (stat(fileName.c_str(), &statbuff) < 0)
			{
				return filesize;
			}
			else
			{
				filesize = statbuff.st_size;
			}
			return filesize;
		}

		void ILogImpl::resetFileHandle()
		{
			if (m_pFile != NULL)
			{
				fflush(m_pFile);
				fclose(m_pFile);
				m_pFile = NULL;
			}
			m_strFileName = "";
		}

		void ILogImpl::writeLogImpl(const char* strLog, int len)
		{
			if(strLog == NULL)
			{
				printf("file handle error *!\n");
				return;
			}
			getCurrentLogHandle();

			if (getFileSize(m_strFileName) + len >= MAX_LOG_SIZE)
			{
				printf("reset file handle\n");
				resetFileHandle();
				writeLogImpl(strLog, len);
			}

			if(m_pFile == NULL)
			{
				printf("file handle error -!\n");
				return;
			}
			if (fwrite(strLog, sizeof(char), len, m_pFile) != len)
			{
				perror("fwrite error");
			}
			fflush(m_pFile);
			fclose(m_pFile);
			m_pFile = NULL;
		}

		/***************************************************/
		ISystemLog::ISystemLog() :
			isActive(true)
		{

		}

		ISystemLog::~ISystemLog()
		{
			isActive = false;
			if (m_strLogList.size() > 0)
			{
				m_strLogList.clear();
			}
		}

		std::string ISystemLog::getFirstLog()
		{
			std::string str;
			if (m_strLogList.size() > 0)
			{
				str = m_strLogList.front();
				m_strLogList.pop_front();
			}

			return str;
		}

		static ISystemLog* g_pSystemLog = NULL;
		ISystemLog* ISystemLog::getInstance()
		{
			if (g_pSystemLog == NULL)
			{
				g_pSystemLog = new ISystemLog();
//				g_pSystemLog->_pThread.start(g_pSystemLog);
			}
			return g_pSystemLog;
		}

		/*
		*	start a thread for log writing
		*/
		void ISystemLog::run()
		{
			while (isActive)
			{
				if (m_strLogList.size() > 0)
				{
					std::string str = getFirstLog();
					writeLogImpl(str.c_str(), str.size());
				}

				_pThread.msleep(20);
			}
		}

		void ISystemLog::writeLog(std::string module, int level, const std::string strLog, ...)
		{
			if (level > getConfigLogLevel())
				return;
			
			char buff[MAX_LOG_LEN];
			memset(buff, 0, MAX_LOG_LEN);
			va_list arg;

			va_start(arg, strLog);
			vsprintf(buff, strLog.c_str(), arg);
			va_end(arg);

			std::string logType;
			switch (level)
			{
			case LOG_SYSTEM:
				logType = "SYSTEM";
				break;
			case LOG_ERROR:
				logType = "ERROR";
				break;
			case LOG_WARNING:
				logType = "WARN";
				break;
			case LOG_INFO:
				logType = "INFO";
				break;
			case LOG_DEBUG:
				logType = "DEBUG";
				break;
			default:
				break;
			}

			char prin[MAX_LOG_LEN] = { 0 };
			std::string time = getCurrentTime();
			snprintf(prin, sizeof(prin)-1, "[%s][%s][%s]:%s\r\n", time.c_str(), module.c_str(), logType.c_str(), buff);
			m_strLogList.push_back(std::string(prin));
		}
		
		int ISystemLog::getConfigLogLevel()
		{
			return LOG_SYSTEM;
		}
	}
}
