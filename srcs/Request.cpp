#include "Request.hpp"

Request::Request(const Request& other) : _raw(other._raw), _method(other._method->clone()), _path(other._path), _protocolVersion(other._protocolVersion), _body(other._body)
{
	init_factories();
}

Request::~Request()
{
	if (_method)
		delete _method;
}

Request::Request(std::string raw) : _method(NULL), _path(""), _protocolVersion(""), _body(""), parse_start(0)
{
	init_factories();
	if (raw == "")
		return;
	append(raw);
}

void Request::init_factories()
{
	_methodFactory.add(new ConnectMethod());
	_methodFactory.add(new DeleteMethod());
	_methodFactory.add(new GetMethod());
	_methodFactory.add(new HeadMethod());
	_methodFactory.add(new OptionsMethod());
	_methodFactory.add(new PostMethod());
	_methodFactory.add(new PutMethod());
	_methodFactory.add(new TraceMethod());

	_headerFactory.add(new AcceptCharsetsHeader());
	_headerFactory.add(new AcceptLanguageHeader());
	_headerFactory.add(new AllowHeader());
	_headerFactory.add(new AuthorizationHeader());
	_headerFactory.add(new ContentLanguageHeader());
	_headerFactory.add(new ContentLengthHeader());
	_headerFactory.add(new ContentLocationHeader());
	_headerFactory.add(new ContentTypeHeader());
	_headerFactory.add(new DateHeader());
	_headerFactory.add(new HostHeader());
	_headerFactory.add(new LastModifiedHeader());
	_headerFactory.add(new LocationHeader());
	_headerFactory.add(new RefererHeader());
	_headerFactory.add(new RetryAfterHeader());
	_headerFactory.add(new ServerHeader());
	_headerFactory.add(new TransferEncodingHeader());
	_headerFactory.add(new UserAgentHeader());
	_headerFactory.add(new WWWAuthenticateHeader());
}

bool Request::append(const std::string& raw)
{
	_raw += raw;
	parse();
	return true;
}

void Request::parse()
{
	if (_method == NULL)
		parse_method();
	if (_method == NULL)
		return;

	if (_raw.size() <= parse_start + 1)
		return;
	if (_raw[parse_start + 1] == ' ')
	{
		Logger::print("Bad whitespace after method.", false, ERROR, VERBOSE);
		throw std::invalid_argument("Bad whitespace after method.");
	}
	parse_start++;

	if (_path == "")
		parse_uri();
	if (_path == "")
		return;

	if (_raw.size() <= parse_start + 1)
		return;
	if (_raw[parse_start + 1] == ' ')
	{
		Logger::print("Bad whitespace after uri.", false, ERROR, VERBOSE);
		throw std::invalid_argument("Bad whitespace after uri.");
	}
	parse_start++;

	if (_protocolVersion == "")
		parse_protocol_version();
	if (_protocolVersion == "")
		return;
}

void Request::parse_method()
{
	std::string method = _raw.substr(0, _raw.find(' '));
	if (!_methodFactory.hasCandidates(method))
	{
		Logger::print("Request method could not be recognized.", false, ERROR, VERBOSE);
		throw std::invalid_argument("Method could not be recognized.");
	}
	if (_raw.find(' ') != std::string::npos && _methodFactory.getByType(method) != NULL)
	{
		Logger::print("Request method is " + method + ".", true, INFO, VERBOSE);
		_method = _methodFactory.createByType(method);
		parse_start = _raw.find(' ');
	}
}

void Request::parse_uri()
{
	if (_raw.find(' ', parse_start))
	{
		if (!is_valid_URI(_raw.substr(parse_start, _raw.find(' ', parse_start))))
		{
			Logger::print("Bad URI format in request.", false, ERROR, VERBOSE);
			throw std::invalid_argument("Bad URI format in request.");
		}
		else
		{
			_path = _raw.substr(parse_start, _raw.find(' ', parse_start));
			parse_start = _raw.find(' ', parse_start);
		}
	}
}

void Request::parse_protocol_version()
{
	std::string version = _raw.substr(0, _raw.find("\r\n", parse_start));
	if (std::string("HTTP/1.1").substr(0, version.size()) != version)
	{
		Logger::print("Request protocol version could not be recognized.", false, ERROR, VERBOSE);
		throw std::invalid_argument("Request protocol version could not be recognized.");
	}
	if (_raw.find("\r\n", parse_start) != std::string::npos && version == "HTTP/1.1")
	{
		Logger::print("Request protocol version is HTTP/1.1.", true, INFO, VERBOSE);
		_protocolVersion = version;
		parse_start = _raw.find("\r\n") + 2;
	}
}

/*
** header-field = field-name ":" OWS field-value OWS
** field-name = token
** field-value = *( field-content / obs-fold )
** field-content = field-vchar [ 1*( SP / HTAB ) field-vchar ]
** field-vchar = VCHAR / obs-text
** obs-fold = CRLF 1*( SP / HTAB )
** obsolete line folding
** see Section 3.2.4
*/
void Request::parse_headers()
{
	if (_raw.find(':', parse_start) == std::string::npos)
		return;
	std::string header_name = _raw.substr(parse_start, _raw.find(':', parse_start));
	if (_headerFactory.contains(header_name))
	{
		
	}
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
		if (uri.substr(0, 7) == "http://" && uri.size() > 7)
			return true;

	if (_method->allowAuthorityURI())
		return true;

	if (_method->allowAsteriskURI())
		if (uri == "*")
			return true;
	return false;
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
