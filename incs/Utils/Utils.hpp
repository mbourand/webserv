#ifndef CONFIG_UTILS_HPP
#define CONFIG_UTILS_HPP

#include <string>
#include <list>
#include <limits>

namespace ft
{
	int toInt(const std::string& str);
	std::string toString(int i);

	std::list<std::string> split(const std::string& str, const std::string& charset);
	std::pair<std::string, int> complete_ip(const std::string& incomplete_ip);
	std::string getErrorMessage(int code);
	bool is_directory(std::string realPath);
	std::string get_cwd();
	std::string simplify_path(const std::string& input, bool safe = false, int base_depth = 0);

	template<class IntegralType>
	bool is_integer(const std::string& str)
	{
		if (str.size() == 0)
			return false;

		size_t res = 0;
		for (size_t i = (str[0] == '-' || str[0] == '+'); i < str.size(); i++)
		{
			res = res * 10 + (str[i] - '0');
			if ((str[0] != '-' && res > std::numeric_limits<IntegralType>::max()) || (str[0] == '-' && res < std::numeric_limits<IntegralType>::min()))
				return false;
		}
		return true;
	}
}

#endif
