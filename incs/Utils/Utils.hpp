#ifndef CONFIG_UTILS_HPP
#define CONFIG_UTILS_HPP

#include <string>
#include <list>
#include <limits>
#include <algorithm>

namespace ft
{
	int toInt(const std::string& str);
	std::string toString(int i);
	std::string toHex(int i);

	std::list<std::string> split(const std::string& str, const std::string& charset);
	std::pair<std::string, int> complete_ip(const std::string& incomplete_ip);
	std::string getErrorMessage(int code);
	bool is_directory(std::string realPath);
	bool is_regular_file(std::string realPath);
	bool is_executable(std::string realPath);
	std::string get_cwd();
	std::string simplify_path(const std::string& input, bool safe = false, int base_depth = 0);
	char		*strncpy(char *dest, const char *src, size_t n);

	template<class IntegralType>
	bool is_integer(const std::string& str)
	{
		if (str.size() == 0)
			return false;

		size_t res = 0;
		for (size_t i = (str[0] == '-' || str[0] == '+'); i < str.size(); i++)
		{
			if (!std::isdigit(str[i]))
				return false;
			res = res * 10 + (str[i] - '0');
			if ((str[0] != '-' && res > static_cast<unsigned int>(std::numeric_limits<IntegralType>::max())) ||
				(str[0] == '-' && res > static_cast<unsigned int>(std::numeric_limits<IntegralType>::min() * -1)))
				return false;
		}
		return true;
	}

	template<class ContainerType, class ValueType>
	inline bool contains(const ContainerType& container, const ValueType& val)
	{
		return std::find(container.begin(), container.end(), val) != container.end();
	}
}

#endif
