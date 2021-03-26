#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <set>
#include <algorithm>

template<class T>
class Factory
{
	public:
		typedef T value_type;

	private:
		std::set<value_type> _elements;
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
			for (typename std::set<value_type>::iterator it = other._elements.begin(); it != other._elements.end(); it++)
				_elements.insert((*it)->clone());
		}

		virtual ~Factory()
		{
			if (_elements.empty())
				return;
			for (typename std::set<value_type>::iterator it = _elements.begin(); it != _elements.end(); it++)
				delete *it;
		}

		Factory& operator=(const Factory& other)
		{
			for (typename std::set<value_type>::iterator it = other._elements.begin(); it != other._elements.end(); it++)
				_elements.insert((*it)->clone());
			return *this;
		}

		value_type createByType(std::string type) const
		{
			for (typename std::set<value_type>::iterator it = _elements.begin(); it != _elements.end(); it++)
				if ((*it)->getType() == type)
					return (*it)->clone();
			return NULL;
		}

		value_type createByType_ignore_case(std::string type) const
		{
			for (typename std::set<value_type>::iterator it = _elements.begin(); it != _elements.end(); it++)
				if (compare_type_lowercase((*it)->getType(), type))
					return (*it)->clone();
			return NULL;
		}

		value_type getByType(std::string type) const
		{
			for (typename std::set<value_type>::iterator it = _elements.begin(); it != _elements.end(); it++)
				if ((*it)->getType() == type)
					return *it;
			return NULL;
		}

		value_type getByType_ignore_case(std::string type) const
		{
			for (typename std::set<value_type>::iterator it = _elements.begin(); it != _elements.end(); it++)
				if (compare_type_lowercase((*it)->getType(), type))
					return *it;
			return NULL;
		}

		bool contains(std::string type) const
		{
			for (typename std::set<value_type>::iterator it = _elements.begin(); it != _elements.end(); it++)
				if ((*it)->getType() == type)
					return true;
			return false;
		}

		bool contains_ignore_case(std::string type) const
		{
			for (typename std::set<value_type>::iterator it = _elements.begin(); it != _elements.end(); it++)
				if (compare_type_lowercase((*it)->getType(), type))
					return true;
			return false;
		}

		bool hasCandidates(const std::string& type) const
		{
			for (typename std::set<value_type>::iterator it = _elements.begin(); it != _elements.end(); it++)
				if ((*it)->getType().substr(0, type.size()) == type)
					return true;
			return false;
		}

		void add(value_type elem)
		{
			_elements.insert(elem);
		}
};

#endif
