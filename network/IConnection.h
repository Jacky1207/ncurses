/*******************************************************************
 *
 *		IConnection class
 *	this class is used for net connection.
 *	it will start a thread for recv net data.
 *	and function members for send net data.
 *
 *
 ******************************************************************/

#ifndef _ICONNECTION_H_
#define _ICONNECTION_H_

#include <tr1/memory>
#include "ITcpSocket.h"
#include "ISocket.h"
#include "IRunable.h"

#include "ISelect.h"

namespace IObject
{
	namespace INetwork
	{
		#define SELECT
		
		using namespace ISystemBase;
		typedef sg_int32_t(* ProReadyReadFun)(sg_int32_t nSock ,sg_char_t *pData, sg_int32_t len);
		//client disconnect callback
		typedef sg_void_t(* DisConnectCallbackFun)(const sg_int32_t nSock);
#ifdef SELECT
		class IConnection : virtual public ISelect
#else
		class IConnection : virtual public IRunable
#endif
		{
		public:
			IConnection(ISocket *pSocket);
			virtual ~IConnection();
#ifndef SELECT
			void run();
#endif
			//set current socket of connection
			sg_void_t setSocket(sg_int32_t nSocket);
			sg_int32_t getSocket(){return _nConnSocket;}
			sg_int32_t sendData(const sg_char_t *buff, const sg_int32_t len);
			
			sg_void_t registerReadFun(ProReadyReadFun fun){ _readyReadFun = fun;}
			sg_void_t registerDisconnectFun(DisConnectCallbackFun fun){ _disconnectCallbackFun = fun;}
#ifdef SELECT
			void eventHandle(const sg_int32_t fd);
#endif
		private:
			ProReadyReadFun _readyReadFun;

			DisConnectCallbackFun _disconnectCallbackFun;
			//socket of connection
			sg_int32_t _nConnSocket;
			//
			ISocket *_pSocket;
			
			sg_bool_t _isActive;
			//enable heart beat
			sg_bool_t _isEnableHeartBeat;
			
			//Thread
			IThread _pThread;
		};
		typedef std::tr1::shared_ptr< IConnection >	IConnectionPtr; 
	}
}
#endif
