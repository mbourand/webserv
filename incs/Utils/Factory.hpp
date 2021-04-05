#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <list>

template<class T>
class Factory
{
	public:
		typedef T value_type;

	private:
		std::list<value_type> _elements;

	public:
		Factory() {}
		Factory(const Factory& other)
		{
			for (typename std::list<value_type>::iterator it = other._elements.begin(); it != other._elements.end(); it++)
				_elements.insert((*it)->clone());
		}

		virtual ~Factory()
		{
			if (_elements.empty())
				return;
			for (typename std::list<value_type>::iterator it = _elements.begin(); it != _elements.end(); it++)
				delete *it;
		}

		Factory& operator=(const Factory& other)
		{
			for (typename std::list<value_type>::iterator it = other._elements.begin(); it != other._elements.end(); it++)
				_elements.insert((*it)->clone());
			return *this;
		}

		value_type createByType(std::string type) const
		{
			for (typename std::list<value_type>::const_iterator it = _elements.begin(); it != _elements.end(); it++)
				if ((*it)->getType() == type)
					return (*it)->clone();
			return NULL;
		}

		value_type getByType(std::string type) const
		{
			for (typename std::list<value_type>::const_iterator it = _elements.begin(); it != _elements.end(); it++)
				if ((*it)->getType() == type)
					return *it;
			return NULL;
		}

		bool contains(std::string type) const
		{
			for (typename std::list<value_type>::const_iterator it = _elements.begin(); it != _elements.end(); it++)
				if ((*it)->getType() == type)
					return true;
			return false;
		}

		bool hasCandidates(const std::string& type) const
		{
			for (typename std::list<value_type>::const_iterator it = _elements.begin(); it != _elements.end(); it++)
				if ((*it)->getType().substr(0, type.size()) == type)
					return true;
			return false;
		}

		void add(value_type elem)
		{
			_elements.push_back(elem);
		}

		const std::list<value_type>& getRegistered() const
		{
			return _elements;
		}
};

#endif
