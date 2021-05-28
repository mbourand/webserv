#include "Request.hpp"
#include "Webserv.hpp"
#include <sstream>
#include "Utils.hpp"

/*
** --------------------------------------- CONSTRUCTOR ---------------------------------------
*/

Request::Request(const Request& other) : _raw(other._raw), _method(other._method->clone()), _url(other._url),
	_protocolVersion(other._protocolVersion), _body(other._body), _content_length(other._content_length), _max_body_size(other._max_body_size),
	_error_code(other._error_code), _port(other._port), _header_section_finished(other._header_section_finished), _url_finished(other._url_finished),
	_finished_parsing(other._finished_parsing), _chunked(other._chunked), _parse_start(other._parse_start)
{
	for (HeadersVector::const_iterator it = other._headers.begin(); it != other._headers.end(); it++)
		_headers.push_back((*it)->clone());
}

Request::~Request()
{
	for (HeadersVector::iterator it = _headers.begin(); it != _headers.end(); it++)
		delete *it;
}

Request::Request(int port) : _method(NULL), _content_length(0), _max_body_size(0), _error_code(0),
	_port(port), _header_section_finished(false), _url_finished(false), _finished_parsing(false), _chunked(false), _parse_start(0)
{
}

Request::Request() : _method(NULL), _content_length(0), _max_body_size(0), _error_code(0), _port(0),
	_header_section_finished(false), _url_finished(false), _finished_parsing(false), _chunked(false), _parse_start(0)
{
}



/*
** --------------------------------------- METHODS ---------------------------------------
*/


/**
 * @brief Ajoute le paramètre à la fin du raw de la Requête, puis parse aussi loin que possible avec le nouveau raw
 *
 * @param str
 * @return false si le parsing est terminé, sinon true
 */
bool Request::append(const std::string& raw)
{
	if (_finished_parsing)
		return false;
	_raw += raw;
	parse();
	return true;
}

void Request::parse()
{
	if (_finished_parsing)
		return;
	if (_method == NULL)
		parse_method();
	if (_method == NULL)
		return;

	if (!_url_finished)
	{
		if (_parse_start + 1 >= _raw.size())
			return;
		if (_raw[_parse_start + 1] == ' ')
		{
			_error_code = 400;
			throw std::invalid_argument("Bad whitespace after method.");
		}

		parse_uri();
	}
	if (!_url_finished)
		return;

	if (_protocolVersion.empty())
	{
		if (_parse_start + 1 >= _raw.size())
			return;
		if (_raw[_parse_start + 1] == ' ')
		{
			_error_code = 400;
			throw std::invalid_argument("Bad whitespace after uri.");
		}
		parse_protocol_version();
	}
	if (_protocolVersion.empty())
		return;

	while (!_header_section_finished && parse_headers())
		;

	if (_header_section_finished)
	{
		size_t cnt = 0;
		for (size_t i = 0; i < _headers.size(); i++)
		{
			if (_headers[i]->getType() == HostHeader().getType())
			{
				if (++cnt > 1)
					break ;
				if (_port && !_max_body_size)
				{
					try
					{
						VirtualHost vhost = VirtualHost::getServerByName(_headers[i]->getValue(), _port, g_webserv.vhosts);
						std::stringstream	convert;
						convert << vhost.getConfig().getConfigPath(_url._path).getParam("max_client_body_size").front();
						convert >> _max_body_size;
					}
					catch(const std::exception& e)
					{}
				}
			}
		}
		if (cnt > 1)
		{
			_error_code = 400;
			throw std::invalid_argument("Multiple Host header in request");
		}
		if (cnt == 0)
		{
			_error_code = 400;
			throw std::invalid_argument("No Host header in request");
		}

	}

	if (_header_section_finished && ((_raw.find("\r\n\r\n", _parse_start) != std::string::npos)
		|| (_content_length && ((_raw.size() - _parse_start) >= _content_length))))
	{
		if (_method->requestHasBody())
		{
			if (!_chunked && ((_raw.size() - _parse_start) > _max_body_size))
			{
				_error_code = 413;
				throw std::invalid_argument("Request Entity too large.");
			}
			if ((_raw.size() - _parse_start) == _content_length)
				_body = dechunk(_raw.substr(_parse_start, _raw.substr(_parse_start).size()));
			else
				_body = dechunk(_raw.substr(_parse_start, _raw.substr(_parse_start).size() - 2));
			if (_body.size() < 100)
				Logger::print("Request body is " + _body, NULL, INFO, VERBOSE);
			else
				Logger::print("Request body is <<<BODY TOO LONG>>>", NULL, INFO, VERBOSE);
		}
		_finished_parsing = true;
	}
	else if (_header_section_finished && _method->requestHasBody() && !_chunked && (_raw.size() - _parse_start) > _max_body_size)
	{
		_error_code = 413;
		throw std::invalid_argument("Request Entity too large.");
	}
}

std::string		Request::dechunk(const std::string& str)
{
	if (!_chunked)
		return (str);
	size_t		start = 0;
	size_t		end = 0;
	size_t		len;
	size_t		total_len = 0;
	std::string	output;

	while ((end = str.find("\r\n", start)) != std::string::npos)
	{
		len = ft::hexToInt(str.substr(start, (end - start)));
		end += 2;
		if (!len)
		{
			for (Request::HeadersVector::iterator it = _headers.begin(); it != _headers.end(); it++)
				if ((*it)->getType() == TransferEncodingHeader().getType())
				{
					delete (*it);
					_headers.erase(it);
					Header* header = g_webserv.headers.createByType_ignore_case(ContentLengthHeader().getType());
					std::stringstream	ss;
					std::string			converted;
					ss << total_len;
					ss >> converted;
					header->parse(converted+"\r\n");
					_headers.push_back(header);
					return (output);
				}
			return (output);
		}
		else if (str.find("\r\n", (end + len)) != (end + len))
		{
			_error_code = 400;
			return ("");
		}
		total_len += len;
		if (total_len > _max_body_size)
		{
			_error_code = 413;
			return ("");
		}
		output += str.substr(end, len);
		start = end + len + 2;
	}
	_error_code = 400;
	return ("");
}

void Request::parse_method()
{
	std::string method = _raw.substr(0, _raw.find(' '));
	if (!g_webserv.methods.hasCandidates(method))
	{
		_error_code = 400;
		throw std::invalid_argument("Method could not be recognized.");
	}
	if (ft::contains(_raw, ' ') && g_webserv.methods.getByType(method) != NULL)
	{
		Logger::print("Request method is " + method + ".", true, INFO, VERBOSE);
		_method = g_webserv.methods.getByType(method);
		_parse_start = _raw.find(' ');
	}
}

void Request::parse_uri()
{
	if (_raw.find(' ', _parse_start + 1) != std::string::npos)
	{
		std::string urlRaw = _raw.substr(_parse_start + 1, _raw.find(' ', _parse_start + 1) - (_parse_start + 1));;
		if (!is_valid_URI(urlRaw))
		{
			_error_code = 400;
			throw std::invalid_argument("Bad URI format in request.");
		}
		_url = URL(urlRaw);
		_parse_start = _raw.find(' ', _parse_start + 1);
		_url_finished = true;
	}
}

void Request::parse_protocol_version()
{
	// _parse_start + 1 is the first character of the protocol version
	if (_raw.find('\r', _parse_start + 1) != std::string::npos && _raw.find("\r\n", _parse_start + 1) == std::string::npos)
		return;
	std::string version = _raw.substr(_parse_start + 1, _raw.find("\r\n", _parse_start + 1) - (_parse_start + 1));
	if (std::string("HTTP/1.1").substr(0, version.size()) != version)
	{
		_error_code = 400;
		throw std::invalid_argument("Request protocol version could not be recognized.");
	}
	if (_raw.find("\r\n", _parse_start + 1) != std::string::npos && version == "HTTP/1.1")
	{
		Logger::print("Request protocol version is HTTP/1.1.", true, INFO, VERBOSE);
		_protocolVersion = version;
		_parse_start = _raw.find("\r\n") + 2;
	}
}

size_t is_header_field_finished(std::string str)
{
	size_t i = 0;

	while (str.find("\r\n", i) != std::string::npos)
	{
		if (str.find("\r\n", i) == 0)
			return (0);
		if (!str[str.find("\r\n", i) + 2])
			return Logger::print("Header field is not finished", -1, INFO, VERBOSE);
		if (str[str.find("\r\n", i) + 2] != ' ' && str[str.find("\r\n", i) + 2] != '\t')
			return Logger::print("Header field is finished", str.find("\r\n", i) + 2, INFO, VERBOSE);
		i = str.find("\r\n", i) + 2;
	}
	return Logger::print("Header field is not finished", -1, INFO, VERBOSE);
}

bool Request::parse_headers()
{
	size_t header_len = is_header_field_finished(_raw.substr(_parse_start));

	if (header_len == size_t(-1))
		return false;
	else if (header_len == 0)
	{
		if (_method->requestHasBody())
			_parse_start += 2;
		else
			_parse_start -= 2;
		Logger::print("Header section is finished", false, INFO, VERBOSE);
		return (_header_section_finished = true);
	}
	if (_raw.find(':', _parse_start) == std::string::npos)
	{
		_error_code = 400;
		throw std::invalid_argument("Invalid header field in request");
	}
	std::string header_name = _raw.substr(_parse_start, _raw.find(':', _parse_start) - _parse_start);
	if ((header_name.find("X-") == 0) || g_webserv.headers.contains_ignore_case(header_name))
	{
		Header* header;
		if (header_name.find("X-") == 0)
			header = CustomHeader(header_name).clone();
		else
			header = g_webserv.headers.createByType_ignore_case(header_name);
		if (header == NULL)
		{
			_error_code = 500;
			throw std::invalid_argument("Out of memory");
		}
		try
		{
			_parse_start += header_name.size() + 1;
			_parse_start += header->parse(_raw.substr(_parse_start));
			Logger::print("Request header : " + header->getType() + " -> " + header->getValue(), true, INFO, VERBOSE);
			_headers.push_back(header);
			if (header->getType() == ContentLengthHeader().getType())
				_content_length = ft::toInt(header->getValue());
			if (header->getType() == TransferEncodingHeader().getType() && header->getValue() == "chunked")
				_chunked = true;
		}
		catch(const std::exception& e)
		{
			_error_code = 400;
			throw std::invalid_argument("Invalid header field in request");
		}
	}
	else
		_parse_start += header_len;
	if (_raw[_parse_start] == '\r' && _raw[_parse_start + 1] == '\n')
	{
		_parse_start -= 2;
		if (_method->requestHasBody())
			_parse_start += 4;
		Logger::print("Header section is finished", false, INFO, VERBOSE);
		return (_header_section_finished = true);
	}
	return true;
}

void Request::parse_body()
{

}

bool Request::is_valid_URI(const std::string& uri) const
{
	if (uri.size() > 8000)
		return false;

	if (_method->allowAbsolutePath())
		if (uri[0] == '/')
			return true;

	if (_method->allowCompleteURL())
		if (uri.size() > 7 && uri.substr(0, 7) == "http://")
			return true;

	if (_method->allowAuthorityURI())
		return true;

	if (_method->allowAsteriskURI())
		if (uri == "*")
			return true;
	return false;
}

bool Request::isfinished(void) const
{
	return (_finished_parsing || _error_code != 0);
}

size_t Request::count_concurrent_occurences(size_t index, char c) const
{
	size_t cnt = 0;
	while (_raw[index + cnt] == c)
		cnt++;
	return cnt;
}



/*
** --------------------------------------- ACCESSOR ---------------------------------------
*/



std::string	Request::getHeaderValue(const std::string& name) const
{
	for (Request::HeadersVector::const_iterator header_it = _headers.begin(); header_it != _headers.end(); header_it++)
		if ((*header_it)->getType() == name)
		{
			return (*header_it)->getValue().substr(0, std::min((*header_it)->getValue().find(':'), (*header_it)->getValue().size()));
			break;
		}
	return ("");
}



/*
** --------------------------------------- OPERATOR ---------------------------------------
*/



Request& Request::operator=(const Request& other)
{
	_raw = other._raw;
	_method = other._method;
	_url = other._url;
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
	out <<	"Method: " << request._method->getType() << '\n' <<
			"Path: " << request._url._path << '\n' <<
			"Protocol Version: " << request._protocolVersion << '\n' <<
			"Headers: " << std::endl;
	for (Request::HeadersVector::const_iterator it = request._headers.begin(); it != request._headers.end(); it++)
		out << "  \"" << (*it)->getType() << "\" -> \"" << (*it)->getValue() << '"' << std::endl;
	if (request._body.size() < 100)
		out << "Body:\n\"" << request._body << "\"" << std::endl;
	else
		out << "Body:\n\"" << "<<<BODY TOO LONG>>>" << "\"" << std::endl;
	return out;
}
