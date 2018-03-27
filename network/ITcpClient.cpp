#include "ITcpClient.h"

namespace IObject
{
	namespace INetwork
	{
		ITcpClient *ITcpClient::_pClient = NULL;
		ITcpClient::ITcpClient() : 
				_pSocket(NULL),
				_pConnection(NULL)
		{
			_pSocket = new ITcpSocket();
			if(_pSocket->initTcpSocket() < 0)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "tcp client init fail!");
			}

		}
		
		ITcpClient::~ITcpClient()
		{
			if(_pSocket != NULL)
			{
				_pSocket->disConnect();
				delete _pSocket;
				_pSocket = NULL;
			}
			
			if(_pConnection != NULL)
			{
				delete _pConnection;
				_pConnection = NULL;
			}
			LOG_WRITE(NETWORK, LOG_INFO, "~ITcpClient");
		}
		
		sg_int32_t ITcpClient::connectTcpServer(const std::string serverIp, const sg_int32_t port)
		{
			int ret = _pSocket->connectServer(serverIp, port);
			if(ret < 0)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "tcp client connecting fail!");
				return ret;
			}
			
			_pConnection = new IConnection(_pSocket);
			_pConnection->setSocket(_pSocket->getSocketHandle());
			_pClient = this;
			_pConnection->registerReadFun(&ITcpClient::readyReadFun);
			LOG_WRITE(NETWORK, LOG_DEBUG, "create client connection");
			
			return ret;
		}
		
		sg_int32_t ITcpClient::send(const sg_char_t *buff, const sg_int32_t len)
		{
			if(_pConnection == NULL)
			{
				LOG_WRITE(NETWORK, LOG_ERROR, "connection is not ready!");
				return -1;
			}
			return _pConnection->sendData(buff,len);
		}
		
		sg_int32_t ITcpClient::readyReadFun(sg_int32_t sock, sg_char_t *pData,  sg_int32_t len)
		{
			_pClient->readyRead(pData,len);
		}
	}
}
