#include "IMutex.h"

namespace IObject
{
	namespace ISystemBase
	{
		IMutex::IMutex()
		{
			pthread_mutex_init(&_mutex,NULL);
		}

		IMutex::~IMutex()
		{
			pthread_mutex_destroy(&_mutex);
		}

		void IMutex::lock()
		{
			pthread_mutex_lock(&_mutex);
		}

		void IMutex::unLock()
		{
			pthread_mutex_unlock(&_mutex);
		}
		
		AutoLock::AutoLock(IMutex &lock) : _lock(lock)
		{
			_lock.lock();
		}
		
		 AutoLock::~AutoLock()
		{
			_lock.unLock();
		}
	}
}