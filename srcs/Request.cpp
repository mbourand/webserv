#include "Request.hpp"

Request::Request(const Request& other) : _raw(other._raw), _method(other._method->clone()), _path(other._path), _protocolVersion(other._protocolVersion), _body(other._body) {}
Request::~Request()
{
	if (_method)
		delete _method;
}

Request::Request(std::string raw)
{
	size_t i = 0;
	_raw = raw;
	if (!isValidRequestFormat())
		throw std::invalid_argument("Request is not in a valid format.");

	// Method
	std::pair<std::string, size_t> read_ret = read_until_charset(i, " ");
	_method = _methodSource.createByType(read_ret.first);
	i = read_charset(read_ret.second, " ").second;
	Logger::print("Request method is " + _method->getType(), true, INFO, VERBOSE);


	// Path
	read_ret = read_until_charset(i, " ");
	if (read_ret.first.find("/", 7) != std::string::npos)
		_path = read_ret.first.substr(read_ret.first.find("/", 7));
	else
		_path = "/";
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
		std::string header_name, header_val;
		while (!is_next_paragraph(i))
		{
			header_name = read_ret.first.substr(0, read_ret.first.find(":"));
			header_val = read_ret.first.substr(read_ret.first.find(":") + 1);
			header_val = header_val.substr(header_val.find_first_not_of(" "));
			_headers[header_name] = header_val;
			Logger::print("Request header: " + header_name + ": " + _headers[header_name], true, INFO, VERBOSE);
			read_ret = read_until_charset(i, "\n");
			i = read_charset(read_ret.second, "\n").second;
		}
		header_name = read_ret.first.substr(0, read_ret.first.find(":"));
		header_val = read_ret.first.substr(read_ret.first.find(":") + 1);
		header_val = header_val.substr(header_val.find_first_not_of(" "));
		_headers[header_name] = header_val;
		Logger::print("Request header: " + header_name + ": " + _headers[header_name], true, INFO, VERBOSE);
	}


	// Body
	_body = _raw.substr(i);
	Logger::print("Request body is " + _body, true, INFO, VERBOSE);


	Logger::print("Request was parsed successfully.", true, SUCCESS, NORMAL);
}

bool Request::is_valid_URI(const std::string& uri, int mask) const
{
	if (mask & ABSOLUTE_PATH)
		if (uri[0] == '/')
			return true;

	if (mask & COMPLETE_URL)
		if (uri.substr(0, 7) == "http://" && uri.size() > 7)
			return true;

	if (mask & URL_AUTHORITY)
		return true;

	if (mask & ASTERISK_URI)
		if (uri == "*")
			return true;
	return false;
}

bool Request::isValidRequestFormat()
{
	static const size_t ver_size = 1;
	static const std::string versions[ver_size] = { "HTTP/1.1" };


	// Line ending check
	if (_raw.size() < 4)
		return Logger::print("Request is too short to be valid.", false, ERROR, VERBOSE);
	if (_raw.substr(_raw.size() - 4) != "\r\n\r\n")
		return Logger::print("Request doesn't end with CRLFCRLF.", false, ERROR, VERBOSE);
	if (_raw[0] == '\n')
		return Logger::print("Request contains a LF line ending.", false, ERROR, VERBOSE);
	size_t at = 0;
	while ((at = _raw.find('\n', at)) != std::string::npos)
		if (_raw[_raw.find('\n', at++) - 1] != '\r')
			return Logger::print("Request contains a LF line ending.", false, ERROR, VERBOSE);
	while (_raw.find("\r\n") != std::string::npos)
		_raw.replace(_raw.find("\r\n"), 2, "\n");


	// Method
	std::pair<std::string, size_t> token = read_until_charset(0, " ");
	if (!_methodSource.contains(token.first))
		return Logger::print("Request method \"" + token.first + "\" could not be recognized.", false, ERROR, VERBOSE);
	std::string method = _methodSource.getByType(token.first)->getType();


	// Space
	if (count_concurrent_occurences(token.second, ' ') != 1)
		return Logger::print("Bad space amount in request.", false, ERROR, VERBOSE);


	// Path
	token = read_until_charset(read_charset(token.second, " ").second, " ");
	int mask = 0;
	if (method == "GET" || method == "POST" || method == "HEAD" || method == "OPTIONS")
		mask += ABSOLUTE_PATH;
	if (method == "GET")
		mask += COMPLETE_URL;
	if (method == "CONNECT")
		mask += URL_AUTHORITY;
	if (method == "OPTIONS")
		mask += ASTERISK_URI;
	if (!is_valid_URI(token.first, mask))
		return Logger::print("Bad URI in request.", false, ERROR, VERBOSE);


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
	bool found_host = false;
	if (read_ret.first[0])
	{
		while (!is_next_paragraph(i))
		{
			if (!header_line_valid(read_ret.first))
				return false;

			if (!found_host)
			{
				std::string header_name = read_ret.first.substr(0, read_ret.first.find(':'));
				for (size_t i = 0; i < header_name.size(); i++)
					header_name[i] = std::toupper(header_name[i]);
				if (header_name == "HOST")
					found_host = true;
			}

			read_ret = read_until_charset(i, "\n");
			i = read_charset(read_ret.second, "\n").second;
		}
		if (!header_line_valid(read_ret.first))
			return false;
		if (!found_host)
		{
			std::string header_name = read_ret.first.substr(0, read_ret.first.find(':'));
			for (size_t i = 0; i < header_name.size(); i++)
				header_name[i] = std::toupper(header_name[i]);
			if (header_name == "HOST")
				found_host = true;
		}
		if (!found_host)
			return Logger::print("Request doesn't contain Host header field.", false, ERROR, VERBOSE);
	}
	return Logger::print("Request format is valid.", true, SUCCESS, VERBOSE);
}

bool Request::header_line_valid(const std::string& line) const
{
	size_t del_pos = line.find(":");
	if (del_pos == std::string::npos)
		return Logger::print("Header line \"" + line + "\" doesn't contain ':' delimiter.", false, ERROR, VERBOSE);
	if (del_pos == 0)
		return Logger::print("Header line \"" + line + "\" Has nothing before ':' delimiter.", false, ERROR, VERBOSE);
	if (del_pos == line.size() - 1)
		return Logger::print("Header line \"" + line + "\" Has nothing after ':' delimiter.", false, ERROR, VERBOSE);
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
