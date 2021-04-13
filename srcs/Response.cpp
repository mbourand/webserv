#include "Response.hpp"
#include <sstream>
#include <sys/time.h>
#include "Utils.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response()
	: _code(0)
{}

Response::~Response()
{}

Response::Response(const Response& other)
{
	*this = other;
}

Response::Response(int code, const std::string& path)
	: _code(code), _path(path)
{}

Response& Response::operator=(const Response& other)
{
	_code = other._code;
	_message = other._message;
	_headers = other._headers;
	_body = other._body;
	_path = other._path;
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



/*
** ------------------------------- ACCESSORS --------------------------------
*/



int	Response::getCode() const { return _code; }
void Response::setCode(int code) { _code = code; }
void Response::setMessage(const std::string& message) { _message = message; }
void Response::setBody(const std::string& body) { _body = body; }

/**
 * @brief Génère le texte de la réponse
 *
 * @param ConfigContext de la location qui correspond à l'url à laquelle on doit répondre
 * @return Le texte de la réponse
 */
std::string Response::getResponseText(const ConfigContext& config)
{
	std::stringstream ss;
	ss << _code;

	std::string code_str = ss.str();

	std::string str = "HTTP/1.1 " + code_str + " " + _message + "\r\n";

	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		str += it->first + ": " + it->second + "\r\n";
	}
	str += "\r\n";
	if (_body != "")
		str += _body;
	if (_body == "" && _code >= 300)
	{
		str += config.getErrorPage(_code);
	}
	return str;
}
