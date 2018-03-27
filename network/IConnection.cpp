#include "IConnection.h"
#include "ILog.h"

namespace IObject
{
	namespace INetwork
	{
		IConnection::IConnection(ISocket *pSocket) : 
				_isActive(true),
				_nConnSocket(-1)
		{
			_pSocket = pSocket;
#ifndef	SELECT
			_pThread.start(this);
#endif
		}
		
		IConnection::~IConnection()
		{
			_isActive = false;
			_pSocket->disConnect(_nConnSocket);

			LOG_WRITE(NETWORK, ISystemBase::LOG_INFO, "~IConnection");
		}

		sg_void_t IConnection::setSocket(sg_int32_t nSocket)
		{
			_nConnSocket = nSocket;
#ifdef SELECT
			ISelect::addEvent(_nConnSocket);
#endif
		}
		
		/*
		 *	there are two way for reading data.
		 * 1. epoll such as select.
		 * 2. by own thread
		 */
#ifdef SELECT
		sg_void_t IConnection::eventHandle(const sg_int32_t fd)
		{
			LOG_WRITE(NETWORK, LOG_DEBUG, "read data come %d",fd);
			if(fd !=  _nConnSocket)
				return ;
			char buf[1024] = {0};
			int len = _pSocket->readMsg(_nConnSocket,buf,1024);
			if(len > 0)
			{
				LOG_WRITE(NETWORK, LOG_DEBUG, "read data %s - %d",buf, len);
				_readyReadFun(_nConnSocket,buf,len);
			}
			else if(len == 0)
			{
				/**
				 * when a tcp connect is closing.
				 * we recv a event in eventhandle.but read nothing.
				 * so we remove the conenction.
				 */
				LOG_WRITE(NETWORK, LOG_INFO, "connection %d disconnect", _nConnSocket);
				ISelect::removeEvent(_nConnSocket);
				_disconnectCallbackFun(_nConnSocket);
			}
		}
#else
		/*recv thread*/
		sg_void_t IConnection::run()
		{
			LOG_WRITE(NETWORK, LOG_DEBUG, "socket[%d] connection thread start",_nConnSocket);
			while(_isActive)
			{
				char buf[1024] = {0};
				int len = _pSocket->readMsg(_nConnSocket,buf,1024);
				if(len > 0)
				{
					LOG_WRITE(NETWORK, LOG_DEBUG, "socket[%d] read data [%s]:%d",_nConnSocket, buf, len);
					_readyReadFun(_nConnSocket,buf,len);
				}
				_pThread.msleep(20);
			}
			LOG_WRITE(NETWORK, LOG_DEBUG, "socket[%d] connection thread exit!",_nConnSocket);
		}
#endif	

		sg_int32_t IConnection::sendData(const sg_char_t *buff, const sg_int32_t len)
		{
			return _pSocket->sendMsg(_nConnSocket,  buff,  len);
		}
	}
}
