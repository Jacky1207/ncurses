/*******************************************************************
 *
 *		ISocket class
 *	this class is used for network.
 *	member functions:
 *		initSocket.				init socket
 *		bindSocket.				bind socket
 * 		listenSocket.			listen socket
 *		acceptSocket.			accept client
 *		connectSocket.			connect server
 *		disconnectSocket.		disconnect 
 *
 *
 ******************************************************************/

#ifndef _ISOCKET_H_
#define _ISOCKET_H_

#include <sys/types.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <error.h>
#include "typedef.h"
#include "IMutex.h"

#include "ILog.h"

namespace IObject
{
	namespace INetwork
	{
		#define NETWORK					"Network"
		#define SERVER_MAX_CONNECTED	20
		using namespace ISystemBase;
		
		
		typedef enum
		{
			TcpSocket = 0,
			UdpSocket = 1,
			UnknownType = -1
		}SocketType;

		typedef enum
		{
			UnconnectedState = 0,
			HostLookupState,
			ConnectingState,
			ConnectedState,
			BoundState,
			ClosingState,
			ListeningState
		}SocketState;

		typedef enum
		{
			SocketNoError= 0,
			SocketError = -1,
			Timeout = -2,
			SocketClose = -3
		}ErrorType;

		class ISocket
		{
		public:
			ISocket();
			virtual ~ISocket();
		
			sg_int32_t initSocket(SocketType type);
			sg_int32_t bindSocket();
			sg_int32_t connectSocket(const std::string serverIp, const sg_int32_t port);
			sg_int32_t listenSocket();
			sg_int32_t acceptSocket(std::string &clientAddr, sg_int32_t &port);
			sg_void_t  setLocalInfo(const std::string serverIp, const sg_uint32_t port);
			
			virtual sg_int32_t sendMsg(const sg_int32_t sock, const sg_char_t* buff, const sg_int32_t len);	//for tcp
			virtual sg_int32_t readMsg(const sg_int32_t sock, sg_char_t* buff, sg_int32_t len);
			
			virtual sg_int32_t sendMsgTo(const sg_char_t* buff, const sg_int32_t len, const sg_int32_t port, const std::string destAddr);	//for udp
			virtual sg_int32_t readMsgfrom(sg_char_t* buff, sg_int32_t &len, sg_int32_t &port, std::string &destAddr);
			
			sg_void_t  disConnect();			
			sg_void_t  disConnect(int sock);
			
			SocketState getSocketState(){return _nSocketState;}
			ErrorType   getSocketError(){return _nSocketError;}
			sg_int32_t  getSocket()		{return _nSocket;}
		private:
			sg_int32_t 	_nSocket;
			SocketState _nSocketState;
			SocketType 	_nSocketType;
			ErrorType 	_nSocketError;
			
			struct sockaddr_in _localAddr;
			//lock
			IMutex m_pLock;
		};
	}
}
#endif
