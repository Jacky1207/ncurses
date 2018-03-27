#include "ITcpSocket.h"

namespace IObject
{
	namespace INetwork
	{
		ITcpSocket::ITcpSocket() : 
				m_nSocket(-1)
		{
			initTcpSocket();
		}
		
		ITcpSocket::~ITcpSocket()
		{
			if(m_nSocket > 0)
			{
				ISocket::disConnect(m_nSocket);
				LOG_WRITE(NETWORK , LOG_DEBUG, "disconnect socket %d",m_nSocket);
			}
			
			LOG_WRITE(NETWORK , LOG_INFO, "~ITcpSocket");
		}
		
		sg_int32_t ITcpSocket::initTcpSocket()
		{
			m_nSocket = ISocket::initSocket(TcpSocket);
			if(m_nSocket < 0)
				LOG_WRITE(NETWORK ,LOG_ERROR, "init socket error: %d",ISocket::getSocketError());
				
			LOG_WRITE(NETWORK ,LOG_DEBUG, "init socket succes: %d",m_nSocket);
			return m_nSocket;
		}
		
		sg_int32_t ITcpSocket::startTcpServer(const int port)
		{
			ISocket::setLocalInfo("", port);
			ISocket::bindSocket();
			int ret = ISocket::listenSocket();
			if(ret != 0)
			{
				LOG_WRITE(NETWORK ,LOG_ERROR, "startTcpServer error: %d",ISocket::getSocketError());
				return ret;
			}
				
			LOG_WRITE(NETWORK ,LOG_DEBUG, "startTcpServer succes!");
				
			return ret;
		}
		
		sg_int32_t ITcpSocket::connectServer(const std::string serverIp, const sg_int32_t port)
		{
			return ISocket::connectSocket(serverIp,port);
		}
	}
}
