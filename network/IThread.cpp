#include "IThread.h"

namespace IObject
{
	namespace ISystemBase
	{
		IThread::IThread()
		{
			
		}

		IThread::~IThread()
		{
			_runable = NULL;
		}

		sg_void_t* IThread::_start_route(void *p)
		{
			IThread *impl = (IThread *)p;
			impl->_runable->run();
			
			pthread_exit( NULL);
		}

		sg_void_t IThread::start(IRunable *p)
		{
			_runable = p;
			int ret = pthread_create(&_pthread_t,NULL,_start_route,(void*)this);
			if(ret != 0)
			{
				perror("fail to create thread!\n");
			}
		}

		sg_void_t IThread::cancel()
		{
			if(_pthread_t <= 0)
				return;
				
			sg_int32_t ret = pthread_cancel(_pthread_t);
			
			pthread_detach( pthread_self());
			_pthread_t = 0;
		}

		sg_void_t IThread::msleep(sg_int32_t milliseconds)
		{
			usleep(milliseconds *1000);
		}
	}
}