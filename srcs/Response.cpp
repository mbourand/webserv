#include "Response.hpp"
#include <sstream>
#include <sys/time.h>
#include "Utils.hpp"
#include <iostream>
#include "Compress.hpp"
#include "Logger.hpp"

#define MIN_COMPRESS_SIZE 100
#define CHUNK_SIZE 100ul

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response()
	: _code(0), _caninflate(false), _cangzip(false)
{}

Response::~Response()
{}

Response::Response(const Response& other)
{
	*this = other;
}

Response::Response(int code, const std::string& path)
	: _code(code), _path(path), _caninflate(false), _cangzip(false)
{

}

Response& Response::operator=(const Response& other)
{
	_code = other._code;
	_message = other._message;
	_headers = other._headers;
	_body = other._body;
	_path = other._path;
	_cangzip = other._cangzip;
	_caninflate = other._caninflate;
	return *this;
}



/*
** ------------------------------- METHODS --------------------------------
*/



void Response::addHeader(const std::string& header_name, const std::string& header_value)
{
	_headers[header_name] = header_value;
}

void Response::removeHeader(const std::string& header_name)
{
	_headers.erase(header_name);
}

std::string	Response::Chunk(const std::string& str)
{
	std::string result;

	for (size_t start = 0; start < str.size(); start += CHUNK_SIZE)
	{
		std::string line = str.substr(start, std::min(CHUNK_SIZE, str.size() - start));
		result += ft::toHex(line.size()) + "\r\n" + line + "\r\n";
	}
	result += "0\r\n\r\n";

	return (result);
}

void Response::addDateHeader(void)
{
	struct timeval		tv;
	char				buffer[1024];
	std::stringstream	ss;
	struct tm			time;

	gettimeofday(&tv, NULL);
	tv.tv_sec -= 7200;
	ss << tv.tv_sec;
	strptime(std::string(ss.str()).c_str(), "%s", &time);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &time);
	this->addHeader("Date", buffer);
}

void	Response::compressBody(const std::string &str)
{
	try
	{
		_body = str;
		if (_caninflate)
		{
			_body = compress_inflate(_body);
			this->addHeader("Content-Encoding", "deflate");
		}
		if (_cangzip)
		{
			_body = compress_gzip(_body);
			this->removeHeader("Content-Encoding");
			if (_caninflate)
				this->addHeader("Content-Encoding", "deflate, gzip");
			else
				this->addHeader("Content-Encoding", "gzip");
		}
	}
	catch(const std::runtime_error& e)
	{
		Logger::print(e.what(), NULL, WARNING, NORMAL);
		removeHeader("Content-Encoding");
		removeHeader("Transfer-Encoding");
		_body = "";
		this->addHeader("Content-Length", ft::toString(str.size()));
	}
}

/*
** ------------------------------- ACCESSORS --------------------------------
*/

Response& Response::setCode(int code)
{
	_code = code;
	return (*this);
}

int	Response::getCode() const { return _code; }
void Response::setMessage(const std::string& message) { _message = message; }
void Response::setBody(const std::string& body) { _body = body; }

void	Response::setCompression(const std::string& str)
{
	if (str == "gzip, deflate")
		_caninflate = _cangzip = true;
	else if (str.find("deflate") != std::string::npos)
		_caninflate = true;
	else if (str.find("gzip") != std::string::npos)
		_cangzip = true;
	else
		_caninflate = _cangzip = false;
}

/**
 * @brief Génère le texte de la réponse
 *
 * @param ConfigContext de la location qui correspond à l'url à laquelle on doit répondre
 * @return Le texte de la réponse
 */
std::string Response::getResponseText(const ConfigContext& config)
{
	if (_code >= 300)
	{
		if ((_caninflate || _cangzip) && true) //replace boolean with config value
		{
			this->addHeader("Transfer-Encoding", "chunked");

			std::string errorPage = config.getErrorPage(_code);
			if (errorPage.size() > MIN_COMPRESS_SIZE)
				this->compressBody(errorPage);
			else
				_body = errorPage;

			if (!_body.empty())
				_body = Chunk(_body);
		}
		else
			this->addHeader("Content-Length", ft::toString(config.getErrorPage(_code).size()));
		this->addHeader("Content-Type", "text/html");
		this->addHeader("Connection", "keep-alive");
		this->addDateHeader();
		this->addHeader("Server", "Webserv");
	}
	else
	{
		if (_body.size() > MIN_COMPRESS_SIZE && true) //replace boolean with config value
			compressBody(_body);
		this->removeHeader("Content-Length");
		this->addHeader("Content-Length", ft::toString(_body.size()));
	}
	std::stringstream ss;
	ss << _code;
	std::string code_str = ss.str();
	std::string str = "HTTP/1.1 " + code_str + " " + ft::getErrorMessage(_code) + "\r\n";

	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		str += it->first + ": " + it->second + "\r\n";
	}
	str += "\r\n";


	if (!_body.empty())
		str += _body;
	if (_body.empty() && _code >= 300)
		str += config.getErrorPage(_code);

	return str;
}
