#ifndef CONFIG_UTILS_HPP
#define CONFIG_UTILS_HPP

#include <string>
#include <list>

namespace ft
{
	int toInt(const std::string& str);
	std::string toString(int i);

	std::list<std::string> split(const std::string& str, const std::string& charset);
	std::pair<std::string, int> complete_ip(const std::string& incomplete_ip);
	std::string getErrorMessage(int code);
}

#endif
