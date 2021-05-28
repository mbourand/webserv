#include "Header.hpp"
#include <stdexcept>

#include <iostream>
size_t Header::parse(std::string content)
{
	size_t len = 0;

	_value = content.substr(1);
	if (_value.find_first_not_of(" \t\v\f\r\n") || _value.empty())
		throw std::invalid_argument("Bad line folding");
	_value.erase(_value.find_last_not_of(" \t\v\f\r\n") + 1);
	while ((len = _value.find("\r\n", len)) != std::string::npos)
		_value.erase(len, _value.find_first_not_of(" \t\v\f\r\n", len) - len);
	return (content.size() - 2);
}

const std::string& Header::getValue() const
{
	return _value;
}
