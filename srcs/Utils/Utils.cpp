#include "Utils.hpp"
#include <stdexcept>
#include <sstream>

int ft::toInt(const std::string& str)
{
	for (size_t i = 0; i < str.size(); ++i)
		if (!std::isdigit(str[i]))
			throw std::invalid_argument("String is not an integer");

	std::istringstream iss(str);
	int ret = 0;
	iss >> ret;
	return ret;
}

std::string ft::toString(int i)
{
	std::stringstream ss;
	ss << i;
	return ss.str();
}

std::list<std::string> ft::split(const std::string& str, const std::string& charset)
{
	std::list<std::string> ret;

	size_t i = str.find_first_not_of(charset, 0);
	while (i < str.size())
	{
		size_t word_start = i;
		i = str.find_first_of(charset, i);
		size_t word_end = i;
		i = str.find_first_not_of(charset, i);
		ret.push_back(str.substr(word_start, word_end - word_start));
	}
	return ret;
}
