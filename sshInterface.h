#ifndef SSH_INTERFACE_H
#define SSH_INTERFACE_H

#include "network/config.h"
#include "network/ITcpSocket.h"

class IInterface
{
public:
	IInterface();
	IInterface(int type);
	~IInterface();
	
	void init();
	void exit();
	
	void getRequest(int index);
	void postRequest(int type, const char* buf, int len);
	
	static void parseRecvData(const char *buf, int len);
private:
	//for get request
	TcpClient *m_pClient;
	
	ITcpSocket *_pSocket;
#define 	LOCAL_ADDRESS	"192.125.155.117"
};
	

#endif