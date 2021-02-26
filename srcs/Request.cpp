#include "Request.hpp"

Request::Request(const Request& other) : _raw(other._raw), _method(other._method->clone()), _path(other._path),
	_protocolVersion(other._protocolVersion), _body(other._body), _header_section_finished(other._header_section_finished),
	_parse_start(other._parse_start), _finished_parsing(other._finished_parsing)
{
	init_factories();
}

Request::~Request()
{
	if (_method)
		delete _method;
	for (HeadersVector::iterator it = _headers.begin(); it != _headers.end(); it++)
		if (*it)
			delete *it;
}

Request::Request() : _method(NULL), _header_section_finished(false), _parse_start(0), _finished_parsing(false)
{
	init_factories();
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

	if (_path == "")
	{
		if (_parse_start + 1 >= _raw.size())
			return;
		if (_raw[_parse_start + 1] == ' ')
			throw std::invalid_argument("Bad whitespace after method.");

		parse_uri();
	}
	if (_path == "")
		return;

	if (_protocolVersion == "")
	{
		if (_parse_start + 1 >= _raw.size())
			return;
		if (_raw[_parse_start + 1] == ' ')
			throw std::invalid_argument("Bad whitespace after uri.");
		parse_protocol_version();
	}
	if (_protocolVersion == "")
		return;

	while (!_header_section_finished && parse_headers())
		;

	if (_header_section_finished)
	{
		size_t cnt = 0;
		for (size_t i = 0; i < _headers.size(); i++)
		{
			if (_headers[i]->getType() == HostHeader().getType())
				if (++cnt > 1)
					break ;
		}
		if (cnt > 1)
			throw std::invalid_argument("Multiple Host header in request");
		if (cnt == 0)
			throw std::invalid_argument("No Host header in request");

	}

	if (_header_section_finished && _raw.find("\r\n\r\n", _parse_start) != std::string::npos && _method->requestHasBody())
	{
		_body = _raw.substr(_parse_start, _raw.substr(_parse_start).size() - 2);
		Logger::print("Request body is " + _body, NULL, INFO, VERBOSE);
		_finished_parsing = true;
	}
}

void Request::parse_method()
{
	std::string method = _raw.substr(0, _raw.find(' '));
	if (!_methodFactory.hasCandidates(method))
		throw std::invalid_argument("Method could not be recognized.");
	if (_raw.find(' ') != std::string::npos && _methodFactory.getByType(method) != NULL)
	{
		Logger::print("Request method is " + method + ".", true, INFO, VERBOSE);
		_method = _methodFactory.createByType(method);
		_parse_start = _raw.find(' ');
	}
}

void Request::parse_uri()
{
	if (_raw.find(' ', _parse_start + 1) != std::string::npos)
	{
		if (!is_valid_URI(_raw.substr(_parse_start + 1, _raw.find(' ', _parse_start + 1))))
			throw std::invalid_argument("Bad URI format in request.");
		else
		{
			_path = _raw.substr(_parse_start + 1, _raw.find(' ', _parse_start + 1) - (_parse_start + 1));
			_parse_start = _raw.find(' ', _parse_start + 1);
			Logger::print("URI is " + _path + ".", true, INFO, VERBOSE);
		}
	}
}

void Request::parse_protocol_version()
{
	// _parse_start + 1 is the first character of the protocol version
	if (_raw.find("\r", _parse_start + 1) != std::string::npos && _raw.find("\r\n", _parse_start + 1) == std::string::npos)
		return;
	std::string version = _raw.substr(_parse_start + 1, _raw.find("\r\n", _parse_start + 1) - (_parse_start + 1));
	if (std::string("HTTP/1.1").substr(0, version.size()) != version)
		throw std::invalid_argument("Request protocol version could not be recognized.");
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
		if (!str[str.find("\r\n", i) + 2])
			return Logger::print("Header field is not finished", -1, INFO, VERBOSE);
		if (str[str.find("\r\n", i) + 2] != ' ' && str[str.find("\r\n", i) + 2] != '\t')
			return Logger::print("Header field is finished", str.find("\r\n", i) + 2, INFO, VERBOSE);
		i = str.find("\r\n", i) + 2;
	}
	return Logger::print("Header field is not finished", -1, INFO, VERBOSE);
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
bool Request::parse_headers()
{
	size_t header_len = is_header_field_finished(_raw.substr(_parse_start));

	if (header_len == size_t(-1))
		return false;
	if (_raw.find(':', _parse_start) == std::string::npos)
		throw std::invalid_argument("Invalid header field in request");
	std::string header_name = _raw.substr(_parse_start, _raw.find(':', _parse_start) - _parse_start);
	if (_headerFactory.contains(header_name))
	{
		Header* header = _headerFactory.createByType(header_name);
		if (header == NULL)
			throw std::invalid_argument("Out of memory");
		try
		{
			_parse_start += header_name.size() + 1;
			_parse_start += header->parse(_raw.substr(_parse_start));
			Logger::print("Request header : " + header->getType() + " -> " + header->getValue(), true, INFO, VERBOSE);
			_headers.push_back(header);
		}
		catch(const std::exception& e)
		{
			throw std::invalid_argument("Invalid header field in request");
		}
	}
	else
		_parse_start += header_len;
	if (_raw[_parse_start] == '\r' && _raw[_parse_start + 1] == '\n')
		return (_header_section_finished = Logger::print("Header section is finished", false, INFO, VERBOSE));
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
		if (uri.substr(0, 7) == "http://" && uri.size() > 7)
			return true;

	if (_method->allowAuthorityURI())
		return true;

	if (_method->allowAsteriskURI())
		if (uri == "*")
			return true;
	return false;
}

size_t Request::count_concurrent_occurences(size_t index, char c) const
{
	size_t cnt = 0;
	while (_raw[index + cnt] == c)
		cnt++;
	return cnt;
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
	for (Request::HeadersVector::const_iterator it = request._headers.begin(); it != request._headers.end(); it++)
		out << "  \"" << (*it)->getType() << "\" -> \"" << (*it)->getValue() << '"' << std::endl;
	out << "Body:\n" << request._body << std::endl;
	return out;
}
