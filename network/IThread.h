#ifndef _ITHREAD_H_
#define _ITHREAD_H_

#include "typedef.h"
#include "IMutex.h"
#include "IRunable.h"

#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

namespace IObject
{
	namespace ISystemBase
	{
		class IThread
		{
		public:
			IThread();
			virtual ~IThread();
			
			sg_void_t start(IRunable *p);
			sg_void_t cancel();
			sg_void_t msleep(sg_int32_t milliseconds);
		private:
			//
			static sg_void_t* _start_route(void *p);
			
			pthread_t _pthread_t;
			IRunable *_runable;
		};
	}
}

#endif
