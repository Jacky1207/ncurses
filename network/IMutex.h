#ifndef _IMUTEX_H_
#define _IMUTEX_H_

#include "typedef.h"
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <stdio.h>

namespace IObject
{
	namespace ISystemBase
	{
		class IMutex
		{
		public:
			IMutex();
			virtual ~IMutex();

			void lock();
			void unLock();
		protected:
			pthread_mutex_t _mutex;
		};
		
		class AutoLock : public IMutex
		{
		public:
			AutoLock(IMutex &lock);

			virtual ~AutoLock();
		protected:
			IMutex& _lock;
		};
	}
}
#endif
