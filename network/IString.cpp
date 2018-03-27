#include "IString.h"

#include <stdlib.h>
namespace IObject
{
	namespace ISystemBase
	{
		IString::IString()
		{
			_buff = NULL;
			_size = 0;
		}
		
		IString::IString(const char* str)
		{
			_size = strlen(str);
			_buff = new char[_size+1];
			memcpy(_buff, str, _size);
			_buff[_size] = '\0';
		}
		
		IString::IString(const IString &other)
		{
			_size = other.size();
			_buff = new char[_size+1];
			memcpy(_buff, other.data(), _size);
			_buff[_size] = '\0';
		}
		
		IString::~IString()
		{
			if(_buff != NULL)
			{
				delete [] _buff;
				_buff = NULL;
			}
			_size = 0;
		}
		
		void IString::clear()
		{
			delete[] _buff;
			_buff = NULL;
			_size = 0;
		}
			
		IString &IString::append(const IString &str)
		{
			return append(str.data());
		}
		
		IString &IString::append(const char *str)
		{
			int leng = _size + strlen(str);
			char *tmp = new char [leng+1];
			memcpy(tmp, _buff, _size);
			memcpy(tmp + _size, str, strlen(str));
			if(_buff != NULL)
			{
				delete[] _buff;
				_buff = NULL;
			}
			
			tmp[leng] = '\0';
			_buff = tmp;
			_size = leng;
			
			return *this;
		}
		
		char *IString::data()
		{
			return _buff;
		}

		const char *IString::data() const
		{
			return _buff;
		}
		
		int  IString::size() const
		{
			return _size;
		}
		
		int  IString::length() const
		{
			return _size;
		}

		const char IString::at(int pos)
		{
			return _buff[pos];
		}
		
		bool IString::compare(const IString &other, CaseSensitivity cs) const
		{
			if(size() != other.size())
				return false;
			for(int i=0; i<other.size();i++)
			{
				if(cs == CaseSensitive)
				{
					if(_buff[i] != other.data()[i])
						return false;
				}
				else if(cs == CaseInsensitive)
				{
					int tmp = _buff[i] - other.data()[i];
					if(tmp != 0 && (tmp != 32 || tmp != -32))
						return false;
				}
			}
			return true;
		}
			
		bool IString::contains(const IString &str, CaseSensitivity cs) const
		{
			if(strstr(this->data(), str.data()) != NULL)
				return true;
			else
				return false;
		}
		
		bool IString::contains(const char *str, CaseSensitivity cs) const
		{
			printf("contains %s\n", str);
			if(strstr(this->data(), str) != NULL)
				return true;
			else
				return false;
		}
		
		int IString::indexOf(const IString &str, int from, CaseSensitivity cs) const
		{
			if(!contains(str, cs))
				return -1;
			
			int index = 0;
			char *p = _buff+from;
			const char *q = str.data();
			int qlen = str.size();
			while(*p)
			{
				int n;
				for(n=0; *(p+n) == *(q+n); n++)
				{
				}

				if(n == str.size())
				{
					return index+from;
				}
				p++;
				index++;
			}
			return -1;
		}
		
		int IString::indexOf(const char *str, int from, CaseSensitivity cs) const
		{
			return indexOf(IString(str), from, cs);
		}
		
		int IString::indexOf(const char str, int from, CaseSensitivity cs) const
		{
			/*here can't use &str, because the result is unconfirm*/
			char tmp[2] = {0};
			tmp[0] = str;
			if(!contains(tmp, cs))
				return -1;
			
			char *p = _buff+from;
			int index = 0;
			while(*p)
			{
				if(*p == str)
				{
					return index+from;
				}
				p++;
				index++;
			}
			return -1;
		}
		
		IString &IString::remove(int position, int n)
		{
			if(position > _size)
				return *this;
			
			int start = position + n;
			if(start >= _size)
			{
				_buff[position] = '\0';
			}
			else
			{
				memmove(_buff+position, _buff+start, _size - start);
				_buff[_size - n] = '\0';
			}
			
			_size -= n;
			_buff[_size] = '\0';
			
			return *this;
		}
		
		IString &IString::remove(const IString &str)
		{
			int index = indexOf(str);
			if(index < 0)
				return *this;
			return remove(index, str.size());
		}

		
		IString &IString::insert(int position, const IString &str)
		{
			return insert(position, str.data());
		}
		
		IString &IString::insert(int position, const char *str)
		{
			return insert(position, str, strlen(str));
		}
		
		IString &IString::insert(int position, const char *str, int len)
		{
			int leng = len + _size;
			int pos =  len + position;
			char *p = new char[leng + 1];
			
			memcpy(p, _buff , position);
			memcpy(p + position, str, len);
			memcpy(p + pos, _buff + position, _size-position);
			p[leng] = '\0';
			
			delete[] _buff;
			
			_buff = p;
			_size = leng;
			
			return *this;			
		}
		
		IString &IString::replace(int i, int len, const char *s, int slen)
		{
			if(i > _size)
				return *this;
			
			remove(i, len);
			printf("remove %s\n", _buff);
			insert(i, s, slen);
			printf("insert %s\n", _buff);
			return *this;
		}
		
		IString &IString::replace(int i, int len, const IString &after)
		{
			return replace(i, len, after.data(), after.size());
		}
		
		IString &IString::replace(const char before, int blen, const char *after, int alen)
		{
			int index = indexOf(before);
			if(index < 0)
				return *this;
			
			return replace(index, blen, after, alen);
		}
		
		IString &IString::replace(const char *before, int blen, const char *after, int alen)
		{
			int index = indexOf(before);
			if(index < 0)
				return *this;
			
			return replace(index, blen, after, alen);
		}
		
		IString &IString::replace(const IString &before, const IString &after)
		{
			return replace(before.data(), before.size(), after.data(), after.size());
		}
		
		IString &IString::replace(char c, const IString &after)
		{
			return replace(&c, 1, after.data(), after.size());
		}
		
		/*
		 *	replace %1 %2 %3... to other format
		 */
		IString IString::arg(unsigned long long a, int base)
		{
			IString str;
			str.setNum(a);
			return replaceArg(str);
		}
		IString IString::arg(long a, int base)
		{
			IString str;
			str.setNum(a);
			return replaceArg(str);
		}
		IString IString::arg(unsigned long a, int base)
		{
			IString str;
			str.setNum(a);
			return replaceArg(str);
		}
		IString IString::arg(int a, int base)
		{
			IString str;
			str.setNum(a);
			return replaceArg(str);
		}
		IString IString::arg(unsigned int a, int base)
		{
			IString str;
			str.setNum(a);
			return replaceArg(str);
		}
		IString IString::arg(short a, int base)
		{
			IString str;
			str.setNum(a);
			return replaceArg(str);
		}
		IString IString::arg(unsigned short a, int base)
		{
			IString str;
			str.setNum(a);
			return replaceArg(str);
		}
		IString IString::arg(double a, char format)
		{
			IString str;
			str.setNum(a);
			return replaceArg(str);
		}
		IString IString::arg(char a)
		{
			IString str;
			str.setNum(a);
			return replaceArg(str);
		}
		IString IString::arg(const IString &a, int fieldWidth)
		{
			IString string(_buff);
			return replaceArg(a);
		}
		IString IString::arg(const IString &a1, const IString &a2)
		{
			const IString *args[2] = { &a1, &a2 };
			return replaceArgEscapes(2, args);
		}
		IString IString::arg(const IString &a1, const IString &a2, const IString &a3)
		{
			const IString *args[3] = { &a1, &a2, &a3 };
			IString string(_buff);
			return replaceArgEscapes(3, args);
		}
		IString IString::arg(const IString &a1, const IString &a2, const IString &a3, const IString &a4)
		{
			const IString *args[4] = { &a1, &a2, &a3, &a4 };
			return replaceArgEscapes(4, args);
		}
		IString IString::arg(const IString &a1, const IString &a2, const IString &a3, const IString &a4, const IString &a5)
		{
			const IString *args[5] = { &a1, &a2, &a3, &a4, &a5 };
			return replaceArgEscapes(5, args);
		}
		IString IString::arg(const IString &a1, const IString &a2, const IString &a3, const IString &a4, const IString &a5, const IString &a6)
		{
			const IString *args[6] = { &a1, &a2, &a3, &a4, &a5, &a6 };
			return replaceArgEscapes(6, args);
		}
		IString IString::arg(const IString &a1, const IString &a2, const IString &a3, const IString &a4, const IString &a5, const IString &a6, const IString &a7)
		{
			const IString *args[7] = { &a1, &a2, &a3, &a4, &a5, &a6, &a7 };
			return replaceArgEscapes(7, args);
		}
		IString IString::arg(const IString &a1, const IString &a2, const IString &a3, const IString &a4, const IString &a5, const IString &a6, const IString &a7, const IString &a8)
		{
			const IString *args[8] = { &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8 };
			return replaceArgEscapes(8, args);
		}
		IString IString::arg(const IString &a1, const IString &a2, const IString &a3, const IString &a4, const IString &a5, const IString &a6, const IString &a7, const IString &a8, const IString &a9)
		{
			const IString *args[9] = { &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9 };
			return replaceArgEscapes(9, args);
		}

		
		IString &IString::setNum(int n, int base)
		{
			char str[32] = {0};
			sprintf(str, "%d", n);
			*this = IString(str);
			return *this;
		}
		IString &IString::setNum(unsigned short n, int base)
		{
			char str[32] = {0};
			sprintf(str, "%d", n);
			*this = IString(str);
			return *this;
		}
		IString &IString::setNum(short n, int base)
		{
			char str[32] = {0};
			sprintf(str, "%d", n);
			*this = IString(str);
			return *this;
		}
		IString &IString::setNum(unsigned int n, int base)
		{
			char str[32] = {0};
			sprintf(str, "%d", n);
			*this = IString(str);
			return *this;
		}
		IString &IString::setNum(long n, int base)
		{
			char str[32] = {0};
			sprintf(str, "%ld", n);
			*this = IString(str);
			return *this;
		}
		IString &IString::setNum(unsigned long n, int base)
		{
			char str[32] = {0};
			sprintf(str, "%ld", n);
			*this = IString(str);
			return *this;
		}
		IString &IString::setNum(long long n, int base)
		{
			char str[32] = {0};
			sprintf(str, "%ld", n);
			*this = IString(str);
			return *this;
		}
		IString &IString::setNum(unsigned long long n, int base)
		{
			char str[32] = {0};
			sprintf(str, "%ld", n);
			*this = IString(str);
			return *this;
		}
		IString &IString::setNum(float n, char format, int precision)
		{
			char str[32] = {0};
			sprintf(str, "%f", n);
			*this = IString(str);
			return *this;
		}
		IString &IString::setNum(double n, char format, int precision)
		{
			char str[32] = {0};
			sprintf(str, "%lf", n);
			*this = IString(str);
			return *this;
		}

		/*
		 *   LOWER:  A  B  C  D  E ... Y  Z
		 *	 .16     41 42 43 44 45    59 5A
		 *   UPPER:  a  b  c  d  e ... y  z
		 *           61 62 63 64 65    79 7A
		 */
		IString IString::toLower() const
		{
			for(int i=0;i<size();i++)
			{
				if(_buff[i] >= 'A' &&  _buff[i] <= 'Z')
					_buff[i] += 0x20;
			}
			
			return *this;
		}
		
		IString IString::toUpper() const
		{
			for(int i=0;i<size();i++)
			{
				if(_buff[i] >= 'a' &&  _buff[i] <= 'z')
					_buff[i] -= 0x20;
			}
			
			return *this;
		}
		
		/*double is 8 byte, so it has 16 bit*/
		double IString::toDouble() const
		{
			double doub = atof(data());
			return doub;
		}
		
		/*double is 4 byte, so it has 8 bit*/
		float IString::toFloat() const
		{
			float flo;
			sscanf(data(), "%1f", &flo);
			return flo;
		}
		
		int IString::toInt(int base) const
		{
			return atoi(data());
		}
		
		long IString::toLong(int base) const
		{
			return atol(data());
		}
		
		long long IString::toLongLong(int base) const
		{
			return atol(data());
		}
		
		short IString::toShort(int base) const
		{
			return (short)atoi(data());
		}
		
		std::string IString::toStdString() const
		{
			return std::string(data());
		}
		
		bool IString::operator!=(const char *other) const
		{
			int ret = strcmp(data(), other);
			if(!ret)
				return false;
			else
				return true;
		}
		
		bool IString::operator!=(const IString &other) const
		{
			return operator!=(other.data());
		}
		
		IString &IString::operator+=(const IString &other)
		{
			append(other);
			return *this;
		}
		
		IString &IString::operator+=(const char *str)
		{
			append(str);
			return *this;
		}
		
		IString &IString::operator+=(char ch)
		{
			append(&ch);
			return *this;
		}
		
		IString &IString::operator=(const IString &other)
		{
			clear();
			this->append(other);
			
			return *this;
		}
				
		IString &IString::operator=(const char *str)
		{
			clear();
			this->append(str);
			
			return *this;
		}
		
		IString &IString::operator=(char ch)
		{
			return operator=(&ch);
		}
		
		bool IString::operator==(const IString &other) const
		{
			return !operator!=(other);
		}
		
		bool IString::operator==(const char *other) const
		{
			return !operator!=(other);
		}
		
		const char IString::operator[](int position) 
		{
			return at(position);
		}
	}
}
