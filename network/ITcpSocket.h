#ifndef _ITCPSOCKET_H_
#define _ITCPSOCKET_H_

#include "ISocket.h"
#include "IConnection.h"

namespace IObject
{
	namespace INetwork
	{
		class ITcpSocket : virtual public ISocket
		{
		public:
			ITcpSocket();
			virtual ~ITcpSocket();
			
			sg_int32_t initTcpSocket();
			sg_int32_t startTcpServer(const int port);
			sg_int32_t startTcpClient();
			sg_int32_t connectServer(const sg_string_t serverIp, const sg_int32_t port);

			sg_int32_t getSocketHandle(){return m_nSocket;}
		private:
			sg_int32_t m_nSocket;
		};
	}
}
#endif
