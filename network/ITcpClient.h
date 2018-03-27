/*******************************************************************
 *
 *		ITcpClient class
 *	this class used for a tcp client.
 *	when building a tcp client ,you need to achieve function readyRead.
 *  member function:
 *		readyRead()  		achieved by derived class
 * 	member var:
 *		m_pSocket:			tcp socket
 *
 *
 ******************************************************************/

#ifndef _ITCPCLIENT_H_
#define _ITCPCLIENT_H_

#include "ITcpSocket.h"
#include "IConnection.h"

namespace IObject
{
	namespace INetwork
	{
		class ITcpClient
		{
		public:
			ITcpClient();
			virtual ~ITcpClient();
			
			sg_int32_t connectTcpServer(const std::string serverIp, const sg_int32_t port);
			//function for send data.
			sg_int32_t send(const sg_char_t *buff, const sg_int32_t len);
			//virtual function for a tcp client.
			virtual sg_int32_t readyRead(sg_char_t *pData, sg_int32_t len) = 0;
		protected:
		
		private:
			//this 
			static ITcpClient *_pClient;
			//_pSocket is a tcp socket.
			ITcpSocket 		*_pSocket;
			//_pConnection is a tcp connection
			IConnection 	*_pConnection;
			
			static sg_int32_t readyReadFun(sg_int32_t sock,sg_char_t *pData, sg_int32_t len);
		};
	}
}
#endif
