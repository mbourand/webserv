#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <algorithm>
#include <list>

template<class T>
class Factory
{
	public:
		typedef T value_type;

	private:
		std::list<value_type> _elements;

		bool	compare_type_lowercase(std::string str1, std::string str2) const
		{
			if (str1.length() != str2.length())
				return (false);
			for (std::string::size_type i = 0; i < str1.length(); i++)
				if (tolower(str1[i]) != tolower(str2[i]))
					return (false);
			return (true);
		}

	public:
		Factory() {}
		Factory(const Factory& other)
		{
			for (typename std::list<value_type>::iterator it = other._elements.begin(); it != other._elements.end(); it++)
				_elements.push_back((*it)->clone());
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
				_elements.push_back((*it)->clone());
			return *this;
		}

		value_type createByType(const std::string& type) const
		{
			for (typename std::list<value_type>::const_iterator it = _elements.begin(); it != _elements.end(); it++)
				if ((*it)->getType() == type)
					return (*it)->clone();
			return NULL;
		}

		value_type createByType_ignore_case(const std::string& type) const
		{
			for (typename std::list<value_type>::const_iterator it = _elements.begin(); it != _elements.end(); it++)
				if (compare_type_lowercase((*it)->getType(), type))
					return (*it)->clone();
			return NULL;
		}

		value_type getByType(const std::string& type) const
		{
			for (typename std::list<value_type>::const_iterator it = _elements.begin(); it != _elements.end(); it++)
				if ((*it)->getType() == type)
					return *it;
			return NULL;
		}

		value_type getByType_ignore_case(const std::string& type) const
		{
			for (typename std::list<value_type>::const_iterator it = _elements.begin(); it != _elements.end(); it++)
				if (compare_type_lowercase((*it)->getType(), type))
					return *it;
			return NULL;
		}

		bool contains(const std::string& type) const
		{
			for (typename std::list<value_type>::const_iterator it = _elements.begin(); it != _elements.end(); it++)
				if ((*it)->getType() == type)
					return true;
			return false;
		}

		bool contains_ignore_case(const std::string& type) const
		{
			for (typename std::list<value_type>::const_iterator it = _elements.begin(); it != _elements.end(); it++)
				if (compare_type_lowercase((*it)->getType(), type))
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
