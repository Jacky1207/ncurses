#pragma once

/*
	 The IMap class is a template class that provides a red-black-tree-based dictionary.
	 It stores (key, value) pairs and provides fast lookup of the value associated with a key.
	 Here's an example QMap with QString keys and int values:
	 
	   IMap<IString, int> map;
	 
	 To insert a (key, value) pair into the map, you can use operator[]():
	 
	   map["one"] = 1;
	   map["three"] = 3;
	   map["seven"] = 7;
 */
namespace IObject
{
	namespace ISystemBase
	{
		template <class Key, typename T>
		class IMap
		{
		public :	
			IMap()
			{
				_last = new _Item;
				_last->prev = NULL;
				_last->next = NULL;
				//save first pointer
				_first = _last;
				
				_size = 0;
			}
			
			IMap(const IMap<Key, T> &other)
			{
				
			}
			
			virtual ~IMap()
			{
				if(size() > 0)
					clear();
				delete _first;
				_first = NULL;
			}
			
			void clear()
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
			
			int  count()
			{
				return  _size;
			}
			
			int  size()
			{
				return  _size;
			}
			
			void append(const Key &key, const T &value)
			{
				_Item *p = new _Item;
				_last->next = p;
			
				p->prev = _last;
				p->next = NULL;
				p->key 	= key;
				p->value = value;
				p->index = size();
				_size++;
				_last = p;
			}
			/*
			 * Inserts a new item with the key key and a value of value.
			 * If there is already an item with the key key, that item's value is replaced with value.
			 * If there are multiple items with the key key, the most recently inserted item's value is replaced with value.
			 */
			void insert(const Key &key, const T &value)
			{
				_Item *p = existKey(key);
				if(p == NULL)
					append(key, value);
				else
					p->value = value;
			}
			
			void insert(int pos, const Key &key, const T &value)
			{
				_Item *p = indexItem(pos);
				_Item *prev = p->prev;
				
				_Item *tmp = new _Item;
				//modify current
				tmp->value = value;
				tmp->key  = key;
				tmp->index = pos;
				tmp->next = p;
				tmp->prev = prev;
				
				//modify prev
				prev->next = tmp;
				p->prev = tmp;

				for(int n=pos;n<size();n++)
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
			
			void setValue(const Key &key, const T &value)
			{
				_Item *p = existKey(key);
				if(p != NULL)
				{
					p->value = value;
				}
			}
			/*
			 * Inserts a new item with the key key and a value of value.
			 * If there is already an item with the same key in the map, 
			 * this function will simply create a new one. 
			 *(This behavior is different from insert(), which overwrites the value of an existing item.)
			 */
			void insertMulti(const Key &key, const T &value)
			{
				
			}
			void insertMulti(const int pos, const Key &key, const T &value)
			{
			}
			
			const T value(const Key &key)
			{
				for(int i=0;i<size();i++)
				{
					_Item *p = indexItem(i);
					if(p->key == key)
					{
						return p->value;
					}
				}
				return (T)NULL;
			}

			const Key key(const T &value)
			{
				_Item *p;
				for(int i=0;i<size();i++)
				{
					p = indexItem(i);
					if(p->value == value)
					{
						return p->key;
					}
				}
				return NULL;
			}
			
			/*
			 * Returns true if the map contains an item with key key; otherwise returns false.
			 */
			bool contains(const Key &key) const
			{
				if(value(key) != NULL)
					return true;
				else
					return false;
			}
			
			bool 	operator!=(const IMap<Key, T> &other) const
			{
				if(size() != other.size())
					return true;
				for(int i=0;i<size();i++)
				{
					if(this->indexItem(i) != other.indexItem(i))
						return true;
					else
						return false;
				}
			}
			bool 	operator==(const IMap<Key, T> &other) const
			{
				return !(operator!=(other));
			}
			
			IMap<Key, T> &operator=(const IMap<Key, T> &other)
			{
			}
			
			T 		&operator[](const Key &key)
			{
				return value(key);
			}
			const T	operator[](const Key &key) const
			{
				return value(key);
			}

		private:
			struct _Item
			{
				_Item *next;	//pointer of next 
				_Item *prev;	//pointer of prev
				Key key;		//map key
				T 	value;		//map value
				int index;		//index of item
				
				_Item()
				{
					next = NULL;
					prev = NULL;
				}
			};
			//last item of map
			_Item *_last;
			_Item *_first;
			//size of map
			unsigned int _size;
			
			//item of index
			_Item *indexItem(int index)
			{
				if(index >= size())
					return NULL;
				_Item *p = _last;
				for(int i=0;i<size();i++)
				{
					if(p->index == index)
						return p;
					p = p->prev;
				}
				
				return NULL;
			}
			
			_Item *existKey(const Key key)
			{
				_Item *p = _last;
				for(int i=0;i<size();i++)
				{
					if(p->key == key)
						return p; 
					p = p->prev;
				}
				
				return NULL;
			}
		};
	}
}
