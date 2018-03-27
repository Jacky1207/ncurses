#ifndef _IRUNABLE_H_
#define _IRUNABLE_H_


namespace IObject
{
	namespace ISystemBase
	{
		class IRunable
		{
		public:
			IRunable(){};
			virtual ~IRunable(){};
			
			virtual void run() = 0;
		};
	}
}
#endif
