#ifndef _ITYPEDEF_H_
#define _ITYPEDEF_H_

#include <string>
#define sg_void_t	void
#define sg_bool_t	bool
#define sg_char_t	char
#define sg_uchar_t	unsigned char
#define sg_int16_t	short
#define sg_int32_t	int
#define sg_uint16_t unsigned short
#define sg_uint32_t unsigned int
#define sg_long_t	long
#define sg_ulong_t  unsigned long
#define sg_longlong_t long long
#define sg_string_t	std::string

#if defined(_WIN32) || defined(_WIN64)
#else
#endif

#define RUN_DECLSPEC_EXPORT __declspec(dllexport)
#define RUN_DECLSPEC_IMPORT __declspec(dllimport)

namespace IObject
{
#define NETWORK			"Network"
#define ITCP			"ITcp"
#define IUDP			"IUdp"
#define STREAM			"Stream"
#define IFILE			"File"
#define TFTP			"Tftp"
#define ILIST			"List"
#define ISELECT			"ISelect"
#define ISHARED			"Shared"
#define ISIGNAL			"Signal"

	enum CaseSensitivity
	{
		CaseInsensitive = 0,
		CaseSensitive = 1
	};
	
	
}

#define SEGBASE_API

#endif
