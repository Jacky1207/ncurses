#include "sshObject.h"

#ifdef  DEBUG
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <list>

#define TMP_FILE	"./log.txt"
#define MAX_LOG_LEN	2048
FILE *m_pFile = NULL;

void initLog()
{
	m_pFile = fopen(TMP_FILE, "a");
	if(m_pFile == NULL)
		printf("error open file\r\n");
}

void exitLog()
{
	fclose(m_pFile);
	m_pFile = NULL;
}

void printfLog(const char *strLog, ...)
{
	char buff[MAX_LOG_LEN];
	memset(buff, 0, MAX_LOG_LEN);
	va_list arg;

	va_start(arg, strLog);
	vsprintf(buff, strLog, arg);
	va_end(arg);

	time_t timer = time(NULL);
	struct tm  *p = localtime(&timer);

	char time[64] = { 0 };
	snprintf(time,sizeof(time)-1 , "%04d-%02d-%02d %02d.%02d.%02d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

	char prin[MAX_LOG_LEN] = { 0 };
	snprintf(prin, sizeof(prin)-1, "[%s]:%s\r\n", time, buff);

	fwrite(prin, sizeof(char), strlen(prin), m_pFile);
	fflush(m_pFile);
}
#endif