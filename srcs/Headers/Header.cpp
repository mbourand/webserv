#include "Header.hpp"
#include <stdexcept>

size_t Header::parse(std::string content)
{
	size_t len = 0;
	do
	{
		size_t start_len = len;
		while (content[len] == ' ' || content[len] == '\t')
			len++;
		while (len < content.find("\r\n", start_len))
			_value += content[len++];
		if (_value == "")
			throw std::invalid_argument("Bad line folding");
		len += 2;
	} while (content[len] == ' ' || content[len] == '\t');
	while (_value[_value.size() - 1] == ' ' || _value[_value.size() - 1] == '\t')
		_value.erase(_value.size() - 1);

	return len;
}

std::string Header::parseValue(std::string content)
{
	size_t len = 0;
	std::string value;

	do
	{
		size_t start_len = len;
		while (content[len] == ' ' || content[len] == '\t')
			len++;
		while (len < content.find("\r\n", start_len))
			value += content[len++];
		if (value == "")
			throw std::invalid_argument("Bad line folding");
		len += 2;
	} while (content[len] == ' ' || content[len] == '\t');
	while (value[value.size() - 1] == ' ' || value[value.size() - 1] == '\t')
		value.erase(value.size() - 1);

	return value;
}


const std::string& Header::getValue() const
{
	return _value;
}
