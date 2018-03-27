#include "ITcpClient.h"
#include <string>

using namespace IObject::INetwork;
using namespace std;
class TcpClient : public ITcpClient
{
#define 		LOGIN		0
#define 		DCU			1
#define 		METER		2
#define 		VPN			3
#define 		NTP			4
#define 		SNMP		5
#define 		DHCP		6
#define 		COMM		7
#define 		ADVANCE		8
#define			HARDWARE	9
#define			DATAINIT	10
#define			PARAMINIT	11
#define			LOGIC		12
#define			SSHLOGIN	13

#define 	REQUEST_GET		1
#define 	REQUEST_POST	2

public :
	TcpClient();
	~TcpClient();
	
	int readyRead(char *buff, int len);
	void sendRequest(int index);
	
	void formatBuff(const char* buf, int leng);
	
	int currentState(){return isCompile;}
	void setCurrentState(int state){isCompile = state;}
	void setRequestType(int type);
		
	typedef void( *readyReadFun)(const char *buf, int len);
	void register_callback(readyReadFun fun){_fun = fun;}
private:
	int isCompile;
#define 	ENV_LENGTH		4096
	char InputBuffer[ENV_LENGTH];
	int  InputLength;
	int request_type;
	int _nSocket;
	readyReadFun _fun;
};