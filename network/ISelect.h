#ifndef _ISELECT_H_
#define _ISELECT_H_

#include "typedef.h"
#include "IRunable.h"
#include "IThread.h"
#include <sys/epoll.h>

namespace IObject
{
	namespace ISystemBase
	{
		#define EPOLL_LISTEN_CNT 		20
		#define EPOLL_LISTEN_TIMEOUT	100
		class ISelect : virtual public IRunable
		{
		public:
			ISelect();
			virtual ~ISelect();
			
			sg_int32_t addEvent(const sg_int32_t fd);
			sg_int32_t removeEvent(const sg_int32_t fd);
			sg_void_t  run();
			
			virtual sg_void_t  eventHandle(const sg_int32_t fd) = 0;
		private:
			sg_int32_t _epollfd;
			IThread _pThread;
			sg_bool_t _isActive;
		};
	}
}
#endif
