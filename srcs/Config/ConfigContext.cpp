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

#include "ConfigContext.hpp"
#include "Logger.hpp"
#include "Utils.hpp"
#include <algorithm>

/*
** ---------------------------------- CONSTRUCTOR --------------------------------------
*/

ConfigContext::ConfigContext()
{
	_directive_names.push_back("server_name");
	_directive_names.push_back("listen");
	_directive_names.push_back("root");
	_directive_names.push_back("location");
}

ConfigContext::~ConfigContext()
{}

ConfigContext::ConfigContext(const ConfigContext& other)
{
	*this = other;
}

ConfigContext& ConfigContext::operator=(const ConfigContext& other)
{
	_params = other._params;
	_childs = other._childs;
	_error_pages = other._error_pages;
	_ports = other._ports;
	_names = other._names;
	_directive_names = other._directive_names;
	return *this;
}

ConfigContext::ConfigContext(const std::string& raw, const std::string& name)
{
	// si name a une valeur, c'est un context de location, on interdit donc certaines directives
	if (name != "")
		_names.push_back(name);
	else
	{
		_directive_names.push_back("server_name");
		_directive_names.push_back("listen");
	}
	_directive_names.push_back("root");
	_directive_names.push_back("location");
	_directive_names.push_back("index");


	for (size_t i = 0; i < raw.size();)
	{
		// On skip les whitepsaces en début de ligne
		i = raw.find_first_not_of(" \t", i);
		if (i == std::string::npos)
			break;

		// Si la ligne est vide ou que c'est un commentaire, on skip la ligne
		if (raw[i] == '\n' || raw[i] == '#')
		{
			i = raw.find('\n', i) + 1;
			continue;
		}

		// On récupère le nom de la directive, par exemple : "server_name" ou "root", etc.
		std::string directive_name = raw.substr(i, raw.find_first_of(" \t", i) - i);
		if (std::find(_directive_names.begin(), _directive_names.end(), directive_name) == _directive_names.end())
			throw std::invalid_argument("Unknown directive name in config");
		if (_params.find(directive_name) != _params.end())
			throw std::invalid_argument("Multiple declaration of the same directive in config");
		i += directive_name.size();
		i = raw.find_first_not_of(" \t", i);

		if (directive_name == "location")
		{
			// On récupère le chemin de la location (juste après le nom de la directive)
			std::string location_name = raw.substr(i, raw.find_first_of(" \t", i) - i);
			i += location_name.size();
			i = raw.find_first_not_of(" \t", i);

			// Si il n'y a pas d'accolate ouvrante, la config est invalide
			if (raw[i] != '{')
				throw std::invalid_argument("Bad curly braces in config");
			i = raw.find_first_not_of(" \t", i + 1);

			// Si il n'y a pas de retour à la ligne après les espaces, la config est invalide
			if (raw[i] != '\n')
				throw std::invalid_argument("Bad new line in config");

			// On récupère le contenu du fichier qui concerne la location
			std::string location_raw = raw.substr(i, find_closing_bracket(raw, i) - i - 2);
			_childs.push_back(ConfigContext(location_raw, location_name));
			i += location_raw.size() + 2;
			if (raw.find('\n', i) == std::string::npos)
				break;
			i = raw.find('\n', i) + 1;
			continue;
		}
		else if (directive_name == "server_name")
		{
			// On récupère la valeur de la directive : exemple "localhost www.localhost.com pouetpouet.fr"
			std::string directive_value = raw.substr(i, raw.find('\n', i) - i);

			for (size_t j = 0; j < directive_value.size();)
			{
				std::string name = directive_value.substr(j, directive_value.find_first_of(" \t\n", j) - j);
				j = directive_value.find_first_of(" \t\n", j);
				_names.push_back(name);

				// Si on est arrivé au dernier mot de la directive, on arrête
				if (j >= directive_value.size() || directive_value[j] == '\n')
					break;
				j = directive_value.find_first_not_of(" \t\n", j);
				if (j >= directive_value.size())
					break;
			}
		}
		else if (directive_name == "listen")
		{
			// On récupère la valeur de la directive : exemple "8080 80 443 5056 21 22"
			std::string directive_value = raw.substr(i, raw.find('\n', i) - i);


			for (size_t j = 0; j < directive_value.size();)
			{
				// On récupère la chaîne du port qu'on convertit ensuite en int
				std::string port_str = directive_value.substr(j, directive_value.find_first_of(" \t\n", j) - j);
				std::istringstream iss(port_str);
				int port;
				iss >> port;

				_ports.push_back(port);

				// Si on est arrivé au dernier mot de la directive, on arrête
				j = directive_value.find_first_of(" \t\n", j);
				if (j >= directive_value.size() || directive_value[j] == '\n')
					break;
				j = directive_value.find_first_not_of(" \t\n", j);
				if (j >= directive_value.size())
					break;
			}
		}
		else
		{
			std::string directive_value = raw.substr(i, raw.find('\n', i) - i);
			_params.insert(std::make_pair(directive_name, ft::split(directive_value, " \t")));
		}
		if (raw.find('\n', i) == std::string::npos)
			break;
		i = raw.find('\n', i) + 1;
	}
}

/*
** ---------------------------------- ACCESSOR --------------------------------------
*/

std::string ConfigContext::getToken(const std::string& val, size_t token)
{
	size_t actual_token = 0;
	size_t start_i = 0;
	size_t i = 0;
	while (actual_token < token && i < val.size())
	{
		i = val.find_first_of(" \t", i);
		i = val.find_first_not_of(" \t", i);
		if (i == std::string::npos && actual_token == token - 1)
			return val.substr(start_i);
		else if (i == std::string::npos)
			throw std::out_of_range("ConfigContext: GetToken: token out of range");
		start_i = i;
		actual_token++;
	}
	return val.substr(start_i, val.find_first_not_of(" \t\n", start_i) - start_i);
}

const std::list<std::string>& ConfigContext::getParam(const std::string& name)
{
	for (std::list<ConfigContext>::iterator it = _childs.begin(); it != _childs.end(); it++)
	{
		// .front() car si il y a des enfants, ce sont forcément des locations qui ne portent qu'un seul nom
		if (it->getNames().front() == name)
		{
			const std::list<std::string>& ret = it->getParam(name);
			if (!ret.empty())
				return ret;
		}
	}
	return _params[name];
}

std::list<ConfigContext>& ConfigContext::getChilds()
{
	return _childs;
}

std::string ConfigContext::getErrorMessage(int code)
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

std::string ConfigContext::getErrorPage(int code)
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

const std::map<std::string, std::list<std::string> >& ConfigContext::getParams() const
{
	return _params;
}

const std::map<int, std::string>& ConfigContext::getErrorPages() const
{
	return _error_pages;
}

const std::list<std::string>& ConfigContext::getNames() const
{
	return _names;
}

const std::list<int>& ConfigContext::getPorts() const
{
	return _ports;
}

/*
** --------------------------------------- METHODS ---------------------------------------
*/

size_t ConfigContext::find_closing_bracket(const std::string& str, size_t start) const
{
	size_t nesting = 1;

	for (size_t i = start; i < str.size(); i++)
	{
		if (str[i] == '{')
			nesting++;
		if (str[i] == '}')
			nesting--;
		if (nesting == 0)
			return i;
	}
	return size_t(-1);
}

std::string ConfigContext::toString() const
{
	std::string str;

	str += "Names:\n";
	for (std::list<std::string>::const_iterator it = _names.begin(); it != _names.end(); it++)
		str += "  " + *it + "\n";

	if (!_ports.empty())
	{
		str += "Ports:\n";
		std::stringstream ss;
		for (std::list<int>::const_iterator it = _ports.begin(); it != _ports.end(); it++)
		{
			ss << *it;
			str += "  " + ss.str() + "\n";
			ss.str("");
		}
	}

	if (!_params.empty())
	{
		str += "Params:\n";
		for (std::map<std::string, std::list<std::string> >::const_iterator it = _params.begin(); it != _params.end(); it++)
		{
			str += "  " + it->first + "\n";
			for (std::list<std::string>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
				str += "    " + *it2 + "\n";
		}
	}

	if (!_childs.empty())
	{
		str += "Childs:\n";
		for (std::list<ConfigContext>::const_iterator it = _childs.begin(); it != _childs.end(); it++)
			str += it->toString();
	}
	return str;
}
