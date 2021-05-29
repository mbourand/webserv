#include "Header.hpp"
#include <stdexcept>

size_t Header::parse(std::string content)
{
	size_t len = 0;
	size_t result = 0;

	do
	{
		result = content.find("\r\n", result) + 2;
	} while (!result && content.find_first_not_of(" \t\v\f\r\n", result) != 0);
	_value = content.substr(1);
	if (_value.find_first_not_of(" \t\v\f\r\n") || _value.empty())
		throw std::invalid_argument("Bad line folding");
	_value.erase(_value.find_last_not_of(" \t\v\f\r\n") + 1);
	while ((len = _value.find("\r\n", len)) != std::string::npos)
		_value.erase(len, _value.find_first_not_of(" \t\v\f\r\n", len) - len);
	return (result);
}

const std::string& Header::getValue() const
{
	return _value;
}
