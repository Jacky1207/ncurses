#include "ISocket.h"


namespace IObject
{
	namespace INetwork
	{
		ISocket::ISocket() : 
			_nSocket(-1),
			_nSocketError(SocketNoError),
			_nSocketType(UnknownType),
			_nSocketState(UnconnectedState)
			
		{
			
		}

		ISocket::~ISocket()
		{
			if (_nSocket >= 0)
				shutdown(_nSocket,SHUT_RDWR);

			LOG_WRITE(NETWORK, ISystemBase::LOG_INFO, "~ISocket");
		}
		
		sg_int32_t ISocket::initSocket(SocketType type)
		{
			if (type == TcpSocket)
				_nSocket = socket(AF_INET, SOCK_STREAM, 0);
			else if (type == UdpSocket)
				_nSocket = socket(AF_INET, SOCK_DGRAM, 0);

			if (_nSocket < 0)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "failed to create socket!");
				perror("create socket");
				return _nSocket;
			}

			_nSocketType = type;
			
			//allow port reuse
			int on=1;
			if(setsockopt(_nSocket,SOL_SOCKET,SO_REUSEADDR,&on ,sizeof(on)) < 0)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "failed to reuse port!");
				return -1;
			}
			
			return _nSocket;
		}
		
		sg_void_t ISocket::setLocalInfo(const std::string serverIp, const sg_uint32_t port)
		{
			memset(&_localAddr,0,sizeof(struct sockaddr_in));
		
			_localAddr.sin_family = AF_INET;
			if (serverIp.empty())
			{
				LOG_WRITE(NETWORK, LOG_DEBUG, "socket address info :INADDR_ANY");
				_localAddr.sin_addr.s_addr = INADDR_ANY;
			}
			else
			{
				LOG_WRITE(NETWORK, LOG_DEBUG, "socket address info %s: %d",serverIp.c_str(),port);
				_localAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
			}

			_localAddr.sin_port = htons(port);
			LOG_WRITE(NETWORK, LOG_DEBUG, "socket info %d ,%d", port, _localAddr.sin_port);
		}

		sg_int32_t ISocket::bindSocket()
		{
			if (_nSocket < 0)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "socket error.bread bind");
				return -1;
			} 
			
			LOG_WRITE(NETWORK, LOG_DEBUG, "bind server port:%d",  ntohs(_localAddr.sin_port));
			int ret = bind(_nSocket, (struct sockaddr*)&_localAddr, sizeof(struct sockaddr));
			if (ret < 0)
			{
				perror("bind:");
				LOG_WRITE(NETWORK, LOG_ERROR, "failed to bind %d",  ntohs(_localAddr.sin_port));
			}
			
			return ret;
		}

		sg_int32_t ISocket::connectSocket(const std::string destinationIp, const int port)
		{
			if (_nSocket < 0)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "socket error.break connect");
				return -1;
			}
			
			struct sockaddr_in dest;
			dest.sin_family = AF_INET;
			dest.sin_addr.s_addr = inet_addr(destinationIp.c_str());
			dest.sin_port = htons(port);
			
			LOG_WRITE(NETWORK, LOG_DEBUG, "connect to ip:%s ", destinationIp.c_str());

			int ret = connect(_nSocket, (struct sockaddr*)&dest, sizeof(dest));
			if (ret < 0)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "failed to connect %s", destinationIp.c_str());
				return -1;
			}
			
			return ret;
		}

		sg_int32_t ISocket::listenSocket()
		{
			if (_nSocket < 0)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "socket error.break listen");
				return -1;
			}

			int ret = listen(_nSocket, SERVER_MAX_CONNECTED);
			if (ret < 0)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "failed to start listen");
			}
			else
				LOG_WRITE(NETWORK, LOG_DEBUG, "start listen ");

			return ret;
		}

		sg_int32_t ISocket::acceptSocket(std::string &clientAddr, sg_int32_t &port)
		{
			if (_nSocket < 0)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "socket error.break listen");
				return -1;
			}

			AutoLock lk(m_pLock);

			struct sockaddr_in clientSock;
			socklen_t clientLen = sizeof(struct sockaddr_in);
			char buff[1024] = {0};
			int sockClient = accept(_nSocket, (struct sockaddr*)&clientSock, &clientLen);
			if (sockClient < 0)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "socket accept error");
				return -1;
			}
			else
			{
				clientAddr = inet_ntoa(clientSock.sin_addr);
				port	   = ntohs(clientSock.sin_port);
				LOG_WRITE(NETWORK, LOG_DEBUG, "accept client socket ip:%s,port:%d ", inet_ntoa(clientSock.sin_addr), port);
			}

			return sockClient;
		}
		
		sg_void_t ISocket::disConnect()
		{
			shutdown(_nSocket, SHUT_RDWR);
			close(_nSocket);
		}

		sg_void_t ISocket::disConnect(int sock)
		{
			shutdown(sock, SHUT_RDWR);
			close(sock);
		}
		
		sg_int32_t ISocket::sendMsg(const sg_int32_t sock, const sg_char_t* buff, const sg_int32_t len)
		{
			int ret = send(sock, buff, len, 0);
			if (ret < 0 || ret != len)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "tcp send data error");
				return ret;
			}

			LOG_WRITE(NETWORK, LOG_DEBUG, "tcp send %d :data %s", sock, buff);
			return ret;
		}
		
		sg_int32_t ISocket::readMsg(const sg_int32_t sock, sg_char_t* buff, sg_int32_t len)
		{
			int ret = recv(sock, buff, len, MSG_DONTWAIT);
			if (ret > 0)
			{
				LOG_WRITE(NETWORK, LOG_DEBUG, "tcp recv %d :data %s", sock, buff);
			}
			if(ret < 0)
				printf("error: %d\n",errno);

			return ret;
		}

		sg_int32_t ISocket::sendMsgTo(const sg_char_t* buff, const sg_int32_t len, const sg_int32_t port, const std::string destAddr = "")
		{
			struct sockaddr_in clientSock;
			socklen_t clientLen = sizeof(struct sockaddr_in);
			clientSock.sin_port = htons(port);
			clientSock.sin_addr.s_addr = inet_addr(destAddr.c_str());
			clientSock.sin_family = AF_INET;

			int ret = sendto(_nSocket, buff, len, 0, (struct sockaddr*)&clientSock, clientLen);
			if (ret < 0)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "udp send data error");
			}

			LOG_WRITE(NETWORK, LOG_DEBUG, "udp send %s :data %s", destAddr.c_str() ,buff);
		}
		
		sg_int32_t ISocket::readMsgfrom(sg_char_t* buff, sg_int32_t &len, sg_int32_t &port, std::string &destAddr)
		{
//			AutoLock lk(m_pLock);

			struct sockaddr_in clientSock;
			socklen_t clientLen = sizeof(struct sockaddr_in);

			int ret = recvfrom(_nSocket, buff, len, 0, (struct sockaddr*)&clientSock, &clientLen);
			if (ret < 0)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "udp recv data error");
				return -1;
			}

			LOG_WRITE(NETWORK, LOG_DEBUG, "udp recv %s :data %s", destAddr.c_str() ,buff);
			port  	 = ntohs(clientSock.sin_port);
			destAddr = inet_ntoa(clientSock.sin_addr);

			return ret;
		}
	}
}
