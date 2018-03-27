#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../sshObject.h"

/**
 *	getConfigure
 * get config env from config.env
 */
/* current request type get/post */
static int request_type = 0;

struct tcpMsg
{
    char	req_type[5];		//request type get/post
    char 	msg_type[2];		//msg type

	int 	msg_length;			//length of msg
	
	/**
	 *	msg format
	 * name=value\n...
	 */
	char 	msg_value[];			//data of msg
};

static int  bool_quit = 0;
/* tcp client handle*/
#include "config.h"

TcpClient::TcpClient() : 
	isCompile(0),
	InputLength(0)
{
	memset(InputBuffer, 0, sizeof(InputBuffer));
}
TcpClient::~TcpClient()
{
}

int TcpClient::readyRead(char *buff, int len)
{
	struct tcpMsg *msg = (struct tcpMsg *)buff;
	if(strstr(msg->req_type, "GET")!= NULL)
	{
		int leng = len-11;
		if(leng > 0)
		{
			memset(InputBuffer, 0, sizeof(InputBuffer));
			memcpy(InputBuffer, buff+11 ,leng);
			//submit to callback
			_fun(InputBuffer, leng);
		}
	}
}

void TcpClient::sendRequest(int type)
{
	int post = 0;
	char buff[ENV_LENGTH] = {0};
	int index = 0;
	if(request_type == REQUEST_GET)
	{
		memcpy(buff, "GET", strlen("GET"));
	}
	else if(request_type == REQUEST_POST)
	{
		memcpy(buff, "POST", strlen("POST"));
		post = 1;
	}
	
	index += 5;
	char tm[5];
	sprintf(tm, "%d", type);
	memcpy(buff+index, tm, 2);
	
	index += 2;
	if(post)
	{
		tm[0] = InputLength;
		tm[1] = InputLength >> 8;
		tm[2] = InputLength >> 16;
		tm[3] = InputLength >> 24;
		memcpy(buff+index, tm, sizeof(int));
		index += 4;
		
		memcpy(buff+index, InputBuffer, InputLength);
		index += InputLength;
	}
	send(buff, index);
}

void TcpClient::setRequestType(int type)
{
	request_type = type;
}

void TcpClient::formatBuff(const char* buf, int leng)
{
	InputLength = leng;
	memset(InputBuffer, 0, sizeof(InputBuffer));
	
	memcpy(InputBuffer, buf, InputLength);
}
/*****************************************************************************/
