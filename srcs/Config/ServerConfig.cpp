/*
** server_name
** port
** Le 1er serveur pour host:port est celui par défaut
** Pages d'erreurs par défaut
** Taille max du body client
** location
**    liste de methods activées
**    root
**    directory_listing
**    fichier par défaut si la requête est un dossier
**    tous les trucs cgi a voir plus tard
**    choisir le dossier où mettre les fichiers uploadés
*/

#include "ServerConfig.hpp"
#include "Logger.hpp"

/*
** ---------------------------------- CONSTRUCTOR --------------------------------------
*/

ServerConfig::ServerConfig()
{
	_params["server_name"] = DEF_SERVER_NAME;
	_params["listen"] = DEF_SERVER_PORT;
	_params["client_max_body"] = DEF_CLIENT_MAX_BODY;
	_params["root"] = DEF_ROOT;
}

ServerConfig::~ServerConfig()
{}

ServerConfig::ServerConfig(const ServerConfig& other)
{
	*this = other;
}

ServerConfig& ServerConfig::operator=(const ServerConfig& other)
{
	_params = other._params;
	_locations = other._locations;
	_error_pages = other._error_pages;
	return *this;
}

ServerConfig::ServerConfig(const std::string& raw)
{
	_params["server_name"] = DEF_SERVER_NAME;
	_params["listen"] = DEF_SERVER_PORT;
	_params["client_max_body"] = DEF_CLIENT_MAX_BODY;
	_params["root"] = DEF_ROOT;

	for (size_t i = 0; i < raw.size();)
	{
		while (raw[i] == ' ' || raw[i] == '\t')
			i++;
		if (raw[i] == '\n')
		{
			i++;
			continue;
		}
		if (raw[i] == '#')
		{
			i = raw.find('\n', i) + 1;
			continue;
		}
		if (raw.find_first_of(" \t", i) > raw.find('\n', i))
			throw std::invalid_argument("ServerConfig: Constructor: Invalid config file");

		std::string directive_name = raw.substr(i, raw.find_first_of(" \t", i) - i);
		i += directive_name.size();
		while (raw[i] == ' ' || raw[i] == '\t')
			i++;

		if (directive_name == "location")
		{
			std::string location_name = raw.substr(i, raw.find_first_of(" \t", i) - i);
			i += location_name.size();
			while (raw[i] == ' ' || raw[i] == '\t')
				i++;
			if (raw[i] != '{')
				throw std::invalid_argument("ServerConfig: Constructor: Invalid config file");
			i += 2;
			std::string location_raw = raw.substr(i, raw.find("}", i) - i - 2);
			_locations.push_back(LocationConfig(location_name, location_raw));
			i += location_raw.size() + 2;
			i = raw.find('\n', i) + 1;
			continue;
		}

		std::string directive_value = raw.substr(i, raw.find('\n', i) - i);
		_params.insert(std::make_pair(directive_name, directive_value));
		i = raw.find('\n', i) + 1;
	}
}

/*
** ---------------------------------- ACCESSOR --------------------------------------
*/

std::string ServerConfig::getToken(const std::string& val, size_t token)
{
	size_t actual_token = 0;
	size_t i = 0;
	while (actual_token < token && i < val.size())
	{
		i = val.find_first_of(" \t", i);
		i = val.find_first_not_of(" \t", i);
		actual_token++;
	}
	if (i == val.size())
		throw std::out_of_range("ServerConfig: GetToken: token out of range");
	return val.substr(i, val.find_first_not_of(" \t\n", i) - i);
}

int ServerConfig::getInt(const std::string& param, size_t token)
{
	std::string token_str = getToken(_params[param], token);
	for (size_t i = 0; i < token_str.size() && token_str[i] != ' ' && token_str[i] != '\t'; i++)
		if (!std::isdigit(token_str[i]))
			throw std::invalid_argument("ServerConfig: GetInt: parameter is not an integer");
	int ret;
	std::istringstream(_params[param]) >> ret;

	return ret;
}

std::string ServerConfig::getString(const std::string& param, size_t token)
{
	return getToken(_params.at(param), token);
}

std::list<LocationConfig>& ServerConfig::getLocations()
{
	return _locations;
}

std::string ServerConfig::getErrorMessage(int code)
{
	switch (code)
	{
		case 100: return "Continue";
		case 101: return "Switching Protocols";
		case 102: return "Processing";
		case 103: return "Early Hints";
		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
		case 207: return "Multi-Status";
		case 208: return "Already Reported";
		case 210: return "Content Different";
		case 226: return "IM Used";
		case 300: return "Multiple choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 306: return "Switch Proxy";
		case 307: return "Temporary Redirect";
		case 308: return "Permanent Redirect";
		case 310: return "Too Many Redirects";
		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Time-out";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Request Entity Too Large";
		case 414: return "Request-URI Too Long";
		case 415: return "Unsupported Media Type";
		case 416: return "Requested range unsatisfiable";
		case 417: return "Expectation failed";
		case 418: return "I'm a teapot";
		case 421: return "Bad mapping / Misdirected Request";
		case 422: return "Unprocessable entity";
		case 423: return "Locked";
		case 424: return "Method failure";
		case 425: return "Too Early";
		case 426: return "Upgrade Required";
		case 428: return "Precondition Required";
		case 429: return "Too Many Requests";
		case 431: return "Request Header Fields Too Large";
		case 449: return "Retry With";
		case 450: return "Blocked by Windows Parental Controls";
		case 451: return "Unavailable For Legal Reasons";
		case 456: return "Unrecoverable Error";
		case 444: return "No Response";
		case 495: return "SSL Certificate Error";
		case 496: return "SSL Certificate Required";
		case 497: return "HTTP Request Sent to HTTPS Port";
		case 498: return "Token expired/invalid";
		case 499: return "Client Closed Request";
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 505: return "HTTP Version not supported";
		case 506: return "Variant Also Negociates";
		case 507: return "Insufficient Storage";
		case 508: return "Loop Detected";
		case 509: return "Bandwidth Limit Exceeded";
		case 510: return "Not extended";
		case 511: return "Network Authentication Required";
		case 520: return "Unknown Error";
		case 521: return "Web server is down";
		case 522: return "Connection Timed Out";
		case 523: return "Origin Is Unreachable";
		case 524: return "A Timeout Occured";
		case 525: return "SSL Handshake Failed";
		case 526: return "Invalid SSL Certificate";
		case 527: return "Railgun Error";
		default: return "Unknown Error";
	}
}

std::string ServerConfig::getErrorPage(int code)
{
	if (_error_pages.find(code) == _error_pages.end())
	{
		std::stringstream ss;
		ss << code;
		std::string code_str = ss.str();
		return
			"<html>\r \
			<head><title>" + code_str + " " + getErrorMessage(code) + "</title></head>\r \
			<body>\r \
			<center><h1>" + code_str + " " + getErrorMessage(code) + "</h1></center>\r \
			<hr><center>Webserv 1.0.0</center>\r \
			</body>\r \
			</html>\r";
	}
	return _error_pages[code];
}

std::map<std::string, std::string>& ServerConfig::getParams()
{
	return _params;
}

std::map<int, std::string>& ServerConfig::getErrorPages()
{
	return _error_pages;
}
