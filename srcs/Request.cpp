#include "Request.hpp"

Request::Request(const Request& other) : _raw(other._raw), _method(other._method), _path(other._path), _protocolVersion(other._protocolVersion), _body(other._body) {}
Request::~Request() {}

Request::Request(const std::string& raw)
{
	size_t i = 0;
	_raw = raw;
	if (!isValidRequestFormat())
		throw std::invalid_argument("Request is not in a valid format.");

	// Method
	std::pair<std::string, size_t> read_ret = read_until_charset(i, " ");
	_method = read_ret.first;
	i = read_charset(read_ret.second, " ").second;
	Logger::print("Request method is " + _method, true, INFO, VERBOSE);

	// Path
	read_ret = read_until_charset(i, " ");
	_path = read_ret.first;
	i = read_charset(read_ret.second, " ").second;
	Logger::print("Request path is " + _path, true, INFO, VERBOSE);

	// Protocol version
	read_ret = read_until_charset(i, "\n");
	_protocolVersion = read_ret.first;
	i = read_charset(read_ret.second, "\n").second;
	Logger::print("Request protocol version is " + _protocolVersion, true, INFO, VERBOSE);

	// Headers
	read_ret = read_until_charset(i, "\n");
	i = read_charset(read_ret.second, "\n").second;
	if (read_ret.first[0])
	{
		while (!is_next_paragraph(i))
		{
			std::string header_name = read_ret.first.substr(0, read_ret.first.find(": "));
			std::string header_val = read_ret.first.substr(read_ret.first.find(": ") + 2, read_ret.first.size());
			_headers[header_name] = header_val;
			read_ret = read_until_charset(i, "\n");
			i = read_charset(read_ret.second, "\n").second;
			Logger::print("Request header: " + header_name + ": " + _headers[header_name], true, INFO, VERBOSE);
		}
		std::string header_name = read_ret.first.substr(0, read_ret.first.find(": "));
		std::string header_val = read_ret.first.substr(read_ret.first.find(": ") + 2, read_ret.first.size());
		_headers[header_name] = header_val;
		Logger::print("Request header: " + header_name + ": " + _headers[header_name], true, INFO, VERBOSE);
	}
	_body = _raw.substr(i, _raw.size());
	Logger::print("Request body is " + _body, true, INFO, VERBOSE);
	Logger::print("Request was parsed successfully.", true, SUCCESS, NORMAL);
}

bool Request::isValidRequestFormat() const
{
	static const size_t mth_size = 8;
	static const std::string methods[mth_size] = { "GET", "POST", "TRACE", "HEAD", "OPTIONS", "CONNECT", "PUT", "DELETE" };
	static const size_t ver_size = 1;
	static const std::string versions[ver_size] = { "HTTP/1.1" };

	// Method
	std::pair<std::string, size_t> token = read_until_charset(0, " ");
	for (size_t i = 0; i < mth_size; i++)
	{
		if (methods[i] == token.first)
			break ;
		if (i == mth_size - 1)
			return Logger::print("Request method \"" + token.first + "\" could not be recognized.", false, ERROR, VERBOSE);
	}

	// Space
	if (count_concurrent_occurences(token.second, ' ') != 1)
		return Logger::print("Bad space amount in request.", false, ERROR, VERBOSE);

	// Path
	token = read_until_charset(read_charset(token.second, " ").second, " ");

	// Space
	if (count_concurrent_occurences(token.second, ' ') != 1)
		return Logger::print("Bad space amount in request.", false, ERROR, VERBOSE);

	// Protocol Version
	token = read_until_charset(read_charset(token.second, " ").second, "\n");
	for (size_t i = 0; i < ver_size; i++)
	{
		if (versions[i] == token.first)
			break ;
		if (i == ver_size - 1)
			return Logger::print("Protocol version \"" + token.first + "\" could not be recognized.", false, ERROR, VERBOSE);
	}

	//Headers
	size_t i = read_charset(token.second, "\n").second;
	std::pair<std::string, size_t> read_ret = read_until_charset(i, "\n");
	if (read_ret.first[0])
	{
		while (!is_next_paragraph(i))
		{
			if (!header_line_valid(read_ret.first))
				return false;
			read_ret = read_until_charset(i, "\n");
			i = read_charset(read_ret.second, "\n").second;
		}
		if (!header_line_valid(read_ret.first))
			return false;
	}

	return Logger::print("Request format is valid.", true, SUCCESS, VERBOSE);
}

bool Request::header_line_valid(const std::string& line) const
{
	size_t del_pos = line.find(": ");
	if (del_pos == std::string::npos)
		return Logger::print("Header line \"" + line + "\" doesn't contain ': ' delimiter.", false, ERROR, VERBOSE);
	if (del_pos == 0)
		return Logger::print("Header line \"" + line + "\" Has nothing before ': ' delimiter.", false, ERROR, VERBOSE);
	if (del_pos == line.size() - 2)
		return Logger::print("Header line \"" + line + "\" Has nothing after ': ' delimiter.", false, ERROR, VERBOSE);
	return true;
}

bool Request::is_next_paragraph(size_t i) const
{
	return _raw[i - 1] == '\n' && _raw[i - 2] == '\n';
}

size_t Request::count_concurrent_occurences(size_t index, char c) const
{
	size_t cnt = 0;
	while (_raw[index + cnt] == c)
		cnt++;
	return cnt;
}

std::pair<std::string, size_t> Request::read_charset(size_t index, const std::string& charset) const
{
	std::string res;
	while (_raw[index] && charset.find(_raw[index]) != std::string::npos)
		res += _raw[index++];
	return std::make_pair(res, index);
}

std::pair<std::string, size_t> Request::read_until_charset(size_t index, const std::string& charset) const
{
	std::string res;
	while (_raw[index] && charset.find(_raw[index]) == std::string::npos)
		res += _raw[index++];
	return std::make_pair(res, index);
}

Request& Request::operator=(const Request& other)
{
	_raw = other._raw;
	_method = other._method;
	_path = other._path;
	_protocolVersion = other._protocolVersion;
	_body = other._body;
	return *this;
}

bool Request::operator==(const Request& other) const
{
	return _raw == other._raw;
}

bool Request::operator!=(const Request& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& out, const Request& request)
{
	out <<	"Method: " << request._method << '\n' <<
			"Path: " << request._path << '\n' <<
			"Protocol Version: " << request._protocolVersion << '\n' <<
			"Headers: " << std::endl;
	for (Request::HeadersMap::const_iterator it = request._headers.begin(); it != request._headers.end(); it++)
		out << "  \"" << it->first << "\" -> \"" << it->second << '"' << std::endl;
	out << "Body:\n" << request._body << std::endl;
	return out;
}
