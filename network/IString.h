#pragma once

/*
	The IString class provides a Unicode character string.
	Unicode is an international standard that supports most of the writing systems in use today. 
	It is a superset of US-ASCII (ANSI X3.4-1986) and Latin-1 (ISO 8859-1), and all the US-ASCII/Latin-1 characters are available at the same code positions.
	Behind the scenes, IString uses implicit sharing (copy-on-write) to reduce memory usage and to avoid the needless copying of data. 
	This also helps reduce the inherent overhead of storing 16-bit characters instead of 8-bit characters.
*/
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
//#include "typedef.h"

using namespace std;
namespace IObject
{
	namespace ISystemBase
	{
		enum CaseSensitivity
		{
			CaseInsensitive = 0,
			CaseSensitive = 1
		};
		class IString
		{
		public:
			IString();
			IString(const char* str);
			IString(const IString &other);
			~IString();
			
			IString &append(const IString &str);
			IString &append(const char *str);
			
			void clear();
			bool compare(const IString &other, CaseSensitivity cs = CaseSensitive) const;
			//bool contain str in string
			bool contains(const IString &str, CaseSensitivity cs = CaseSensitive) const;
			bool contains(const char *str, CaseSensitivity cs) const;
			//count of str in string
			int count(const IString &str, CaseSensitivity cs = CaseSensitive) const;
			int count() const;
			
			int size() const;
			int length() const;
			const char at(int pos);
			char *data();
			const char *data() const;
			
			int indexOf(const IString &str, int from = 0, CaseSensitivity cs = CaseSensitive) const;
			int indexOf(const char *str, int from = 0, CaseSensitivity cs = CaseSensitive) const;
			int indexOf(const char str, int from = 0, CaseSensitivity cs = CaseSensitive) const;

			IString &insert(int position, const IString &str);
			IString &insert(int position, const char *str);
			IString &insert(int position, const char *str, int len);
			
			IString &remove(int position, int n);
			IString &remove(const IString &str);
			
			IString &replace(int i, int len, const char *s, int slen);
			IString &replace(int i, int len, const IString &after);
			IString &replace(const char *before, int blen, const char *after, int alen);
			IString &replace(const char before, int blen, const char *after, int alen);
			IString &replace(const IString &before, const IString &after);
			IString &replace(char c, const IString &after);
			
			IString arg(unsigned long long a, int base = 10);
			IString arg(long a, int base = 10);
			IString arg(unsigned long a, int base = 10);
			IString arg(int a, int base = 10);
			IString arg(unsigned int a, int base = 10);
			IString arg(short a, int base = 10);
			IString arg(unsigned short a, int base = 10);
			IString arg(double a, char format = 'g');
			IString arg(char a);
			IString arg(const IString &a, int fieldWidth = 0);
			IString arg(const IString &a1, const IString &a2);
			IString arg(const IString &a1, const IString &a2, const IString &a3);
			IString arg(const IString &a1, const IString &a2, const IString &a3, const IString &a4);
			IString arg(const IString &a1, const IString &a2, const IString &a3, const IString &a4, const IString &a5);
			IString arg(const IString &a1, const IString &a2, const IString &a3, const IString &a4, const IString &a5, const IString &a6);
			IString arg(const IString &a1, const IString &a2, const IString &a3, const IString &a4, const IString &a5, const IString &a6, const IString &a7);
			IString arg(const IString &a1, const IString &a2, const IString &a3, const IString &a4, const IString &a5, const IString &a6, const IString &a7, const IString &a8);
			IString arg(const IString &a1, const IString &a2, const IString &a3, const IString &a4, const IString &a5, const IString &a6, const IString &a7, const IString &a8, const IString &a9);
			
			IString &setNum(int n, int base = 10);
			IString &setNum(unsigned short n, int base = 10);
			IString &setNum(short n, int base = 10);
			IString &setNum(unsigned int n, int base = 10);
			IString &setNum(long n, int base = 10);
			IString &setNum(unsigned long n, int base = 10);
			IString &setNum(long long n, int base = 10);
			IString &setNum(unsigned long long n, int base = 10);
			IString &setNum(float n, char format = 'g', int precision = 6);
			IString &setNum(double n, char format = 'g', int precision = 6);
			
			IString toLower() const;
			IString toUpper() const;
			double toDouble() const;
			float toFloat() const;
			int toInt(int base = 10) const;
			long toLong(int base = 10) const;
			long long toLongLong(int base = 10) const;
			short toShort(int base = 10) const;
			std::string toStdString() const;
			
			bool operator!=(const char *other) const;
			bool operator!=(const IString &other) const;
			IString &operator+=(const IString &other);
			IString &operator+=(const char *str);
			IString &operator+=(char ch);
			IString &operator=(const IString &other);
			IString &operator=(const char *str);
			IString &operator=(char ch);
			bool operator==(const IString &other) const;
			bool operator==(const char *other) const;
			const char operator[](int position);

		private: //members
			//contain string character
			char *_buff;
			unsigned int _size;
		private: //function
			IString &replaceArgEscapes(int num, const IString **str)
			{
				for(int i=0;i<num;i++)
				{
					char ch = '%';
					int index = indexOf(ch);
					printf("index = %d\n", index);
					if(index < 0)
					{
						printf("missing some arg in IString!");
						return *this;
					}
					replace(ch, 2, str[i]->data(), str[i]->size());
					printf("replaceArgEscapes  %s\n", this->data());
				}
				return *this;
			}
			IString &replaceArg(const IString &str)
			{
				const IString *args[1] = { &str };
				printf("replaceArg  %s\n", this->data());
				return replaceArgEscapes(1, args);
			}
		};
	}
}
