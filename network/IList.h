#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

namespace IObject
{
	namespace ISystemBase
	{
		template <class T>
		class IList
		{
		public:
			IList();
			virtual ~IList();

			//return size of list.
			int size();
			//push item
			void append(const T &value);
			void append(const IList<T> &value);
			void insert(int i, const T &value);
			void replace(int i, const T &value);

			void clear();
			void erase(int pos);
			void remove(int pos);
			
			void pop_back();
			void pop_front();
			void prepend(const T &value);
			void push_back(const T &value);
			void push_front(const T &value);
			//return item at index
			const T at(int index);
			T first();
			const T first() const;
			T front();
			const T front() const;
			T last();
			const T last() const;
			
			bool isEmpty();
			
			//operators
			bool 		operator!=(const IList<T> &other) const;
			bool 		operator==(const IList<T> &other) const;
			IList<T>   &operator+(const IList<T> &other) const;
			IList<T>   &operator+=(const IList<T> &other);
			IList<T>   &operator+=(const T &value);
			IList<T>   &operator<<(const IList<T> &other);
			IList<T>   &operator<<(const T &value);
			IList<T>   &operator=(const IList<T> &other);
			T 		   &operator[](int i);
			const T    &operator[](int i) const;
		private:
			struct _Item
			{
				_Item *next;
				_Item *prev;
				T 	item;
				int index;
				
				_Item()
				{
					next = NULL;
					prev = NULL;
				}
			};

			_Item *_first;
			_Item *_last;
			
			unsigned int _size;

		private:
			struct _Item *listItem(int index)
			{
				if(index > size())
				{
					return NULL;
				}
				_Item *tmp = _last;
				for(int i=0;i<size();i++)
				{
					_Item *p = tmp;
					if(p->index == index)
						return p;
					else
					{
						tmp = p->prev;
					}
				}
				
				return NULL;
			}
		};
		
		
		template <class T>	IList<T>::IList() : 
			_last(NULL)
		{
			//new start location.
			_last = new _Item;
			_last->next = NULL;
			_last->prev = NULL;
			_size = 0;
			
			_first = _last;
		}
		
		template <class T>	IList<T>::~IList()
		{
			if(_size > 0)
				clear();
			
			delete _first;
			_first = NULL;
		}
		
		template <class T> 	void IList<T>::clear()
		{
			_Item *temp = _last;
			_Item *p = _last->prev;
			for(int i=0;i<size();i++)
			{
				delete temp;
				temp = p;
				p = p->prev;
			}
			
			//set _last pointer to the start location.
			_last = _first;
			
			_size = 0;
		}
		
		template <class T>	void IList<T>::erase(int pos)
		{
			_Item *p = listItem(pos);
			_Item *prev = p->prev;
			_Item *next = p->next;
			if(next != NULL)
				next->prev = prev;
			prev->next = next;

			for(int i=pos;i<size();i++)
			{
				if(next != NULL)
				{
					next->index = i;
					next = next->next;
					if(next->next == NULL)
					{
						next->index = i+1;
						break;
					}
				}
				else
				{
					_last = prev;
					break;
				}
			}
					
			delete p;
			
			_size --;
		}
		
		template <class T>	void IList<T>::remove(int pos)
		{
			erase(pos);
		}
		
		//return size of list.
		template <class T>	int IList<T>::size()
		{
			return _size;
		}
		//push item
		template <class T>	void IList<T>::append(const T &value)
		{
			_Item *p = new _Item;
			_last->next = p;
			
			p->prev = _last;
			p->next = NULL;
			p->item = value;
			p->index = size();
			_size++;
			_last = p;
		}
		
		template <class T> 	void IList<T>::append(const IList<T> &value)
		{
			for(int i=0; i<value.size(); i++)
			{
				append(value.at(i));
			}
		}
		
		template <class T>	void IList<T>::insert(int i, const T &value)
		{
			_Item *p = listItem(i);
			_Item *prev = p->prev;
			
			_Item *tmp = new _Item;
			//modify current
			tmp->item = value;
			tmp->index = i;
			tmp->next = p;
			tmp->prev = prev;
			
			//modify prev
			prev->next = tmp;
			p->prev = tmp;

			for(int n=i;n<size();n++)
			{
				p->index = n+1;
				p = p->next;
				if(p->next == NULL)
				{
					p->index = (n+1)+1;
					break;
				}
			}
			
			_size ++;
		}
		
		template <class T>	void IList<T>::replace(int i, const T &value)
		{
			_Item *p = listItem(i);

			p->item = value;
		}
		
		//return item at index
		template <class T>	const T IList<T>::at(int index)
		{
			_Item *tmp = _last;
			for(int i=0;i<size();i++)
			{
				_Item *p = tmp;
				if(p->index == index)
					return p->item;
				else
				{
					tmp = p->prev;
				}
			}
		}
		
		template <class T>	T IList<T>::first()
		{
			return at(0);
		}
		
		template <class T>	const T IList<T>::first() const
		{
			return at(0);
		}
		
		template <class T>	T IList<T>::front()
		{
			return at(0);
		}
		
		template <class T>	const T IList<T>::front() const
		{
			return at(0);
		}
		
		template <class T>	T IList<T>::last()
		{
			return _last->item;
		}
		
		template <class T>	const T IList<T>::last() const
		{
			return _last->item;
		}
		
		template <class T>	bool IList<T>::isEmpty()
		{
			if(size() > 0)
				return false;
			else
				return true;
		}
		
		//operators
		template <class T>
		bool		IList<T>::operator!=(const IList<T> &other) const
		{
			if(size() != other.size())
				return true;

			for(int i=0; i<size(); i++)
			{
				if(at(i) != other.at(i))
				{
					return true;
				}
			}

			return false;
		}
		
		template <class T>
		bool		IList<T>::operator==(const IList<T> &other) const
		{
			return !operator!=(other);
		}
		
		template <class T>
		IList<T>   &IList<T>::operator+(const IList<T> &other) const
		{
			this->append(other);
			return *this;
		}
		
		template <class T>
		IList<T>   &IList<T>::operator+=(const IList<T> &other)
		{
			return operator+(other);
		}
		
		template <class T>
		IList<T>   &IList<T>::operator+=(const T &value)
		{
			this->append(value);
			return *this;
		}
		
		template <class T>
		IList<T>   &IList<T>::operator<<(const IList<T> &other)
		{
			this->append(other);
			return *this;
		}
		
		template <class T>
		IList<T>   &IList<T>::operator<<(const T &value)
		{
			this->append(value);
			return *this;
		}
		
		template <class T>
		IList<T>   &IList<T>::operator=(const IList<T> &other)
		{
			this->clear();
			for(int i=0; i<other.size(); i++)
			{
				_Item *p = new _Item;
				p = other.listItem(0);
			}
			
			_size = other.size();
		}
		
		template <class T>
		T 		   &IList<T>::operator[](int i)
		{
			return listItem(i);
		}
		
		template <class T>
		const T    &IList<T>::operator[](int i) const
		{
			return listItem(i);
		}
	}
}

