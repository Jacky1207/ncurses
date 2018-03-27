#include <unistd.h>

#include "sshInterface.h"
#include "sshWindow.h"

extern IWindow *g_pWindow;
#define REQUEST_PORT		55555

IInterface::IInterface() :
		m_pClient(NULL),
		_pSocket(NULL)
{
	
}

IInterface::IInterface(int type) :
		m_pClient(NULL),
		_pSocket(NULL)
{
	_pSocket = new ITcpSocket();
	_pSocket->initTcpSocket();
	_pSocket->connectServer(LOCAL_ADDRESS,REQUEST_PORT);
}

IInterface::~IInterface()
{
	if(m_pClient != NULL)
	{
		delete m_pClient;
		m_pClient = NULL;
		PRINTF("~IInterface");
	}
	
	if(_pSocket != NULL)
	{
		delete _pSocket;
		_pSocket = NULL;
	}
}

void IInterface::init()
{
	m_pClient = new TcpClient();
	m_pClient->connectTcpServer(LOCAL_ADDRESS,REQUEST_PORT);
	PRINTF("init");
}

void IInterface::exit()
{

}

void IInterface::getRequest(int index)
{
	m_pClient->setRequestType(REQUEST_GET);
	m_pClient->sendRequest(index);
	m_pClient->register_callback(&IInterface::parseRecvData);
}

void IInterface::postRequest(int type, const char* buf, int len)
{
	char buff[4096] = {0};
	int index = 0;

	memcpy(buff, "POST", strlen("POST"));
		
	index += 5;
	char tm[5];
	sprintf(tm, "%d", type);
	memcpy(buff+index, tm, 2);
	
	index += 2;
	{
		tm[0] = len;
		tm[1] = len >> 8;
		tm[2] = len >> 16;
		tm[3] = len >> 24;
		memcpy(buff+index, tm, sizeof(int));
		index += 4;
		
		memcpy(buff+index, buf, len);
		index += len;
	}

	_pSocket->sendMsg(_pSocket->getSocketHandle(), buff, index);
	
//	setRequestType(REQUEST_POST);
	
//	formatBuff(buf, len);
//	_pSocket->sendRequest(index);
}

void IInterface::parseRecvData(const char *buf, int len)
{
	PRINTF("callback %d:%s", len, buf);
	
	int index = 0;
	do
	{
		char *p = strstr(buf+index, "\n");
		if(p != NULL)
		{
			char sub[100] = {0};
			memcpy(sub, buf+index, len-strlen(p)-index);
			index = len-strlen(p)+1;
			PRINTF("%s  *%d", sub, index);
			char *tmp = strstr(sub, "=");
			if(tmp != NULL)
			{
				char key[20] = {0};
				char val[64] = {0};
				memcpy(key, sub, strlen(sub)-strlen(tmp));
				memcpy(val, tmp+1, strlen(tmp)-1);
				PRINTF("key=%s, val=%s", key, val);
				
				g_pWindow->setFormText(key, val);
			}
			if(index >= len)
				break;
		}
		else
			break;
	}while(1);
	
	PRINTF("callback finish!");
	g_pWindow->clientFinish();
}