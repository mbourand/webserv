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
#include <sstream>
#include <fstream>

/*
** ---------------------------------- CONSTRUCTOR --------------------------------------
*/

ConfigContext::ConfigContext()
	: _autoIndex(false)
{
	_directive_names.push_back("server_name");
	_directive_names.push_back("listen");
	_directive_names.push_back("root");
	_directive_names.push_back("location");
	_directive_names.push_back("autoindex");
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
	_error_pages = other._error_pages;
	_autoIndex = other._autoIndex;
	_directive_names = other._directive_names;
	_childs = other._childs;
	_names = other._names;
	return *this;
}

ConfigContext::ConfigContext(const std::string& raw, const std::string& name, const ConfigContext* parent)
	: _autoIndex(false)
{
	// si name a une valeur, c'est un context de location, on interdit donc certaines directives
	if (parent != NULL)
	{
		_params = parent->_params;
		_error_pages = parent->_error_pages;
		_autoIndex = parent->_autoIndex;
	}
	if (parent != NULL)
		_names.push_back(name);
	else
	{
		_directive_names.push_back("server_name");
		_directive_names.push_back("listen");
		_directive_names.push_back("location");
	}
	_directive_names.push_back("root");
	_directive_names.push_back("error_page");
	_directive_names.push_back("index");
	_directive_names.push_back("autoindex");


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
			_params.erase(_params.find(directive_name));
		i += directive_name.size();
		i = raw.find_first_not_of(" \t", i);

		if (directive_name == "location")
		{
			// On récupère le chemin de la location (juste après le nom de la directive)
			std::string location_name = raw.substr(i, raw.find_first_of(" \t\n", i) - i);
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
			_childs.push_back(ConfigContext(location_raw, location_name, this));
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
			if (directive_value.size() == 0)
				throw std::invalid_argument("Empty directive value in config");

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
		else if (directive_name == "error_page")
		{
			// On récupère la valeur de la directive : exemple "400 404 405 error.html"
			std::string directive_value = raw.substr(i, raw.find('\n', i) - i);
			if (directive_value.size() == 0)
				throw std::invalid_argument("Empty directive value in config");

			std::list<std::string> words = ft::split(directive_value, " \t\n");
			std::string page = words.back();

			for (std::list<std::string>::const_iterator it = words.begin(); it != --words.end(); it++)
				_error_pages.insert(std::make_pair(ft::toInt(*it), page));
		}
		else if (directive_name == "autoindex")
		{
			// On récupère la valeur de la directive : exemple "400 404 405 error.html"
			std::string directive_value = raw.substr(i, raw.find('\n', i) - i);
			if (directive_value.size() == 0)
				throw std::invalid_argument("Empty directive value in config");

			std::list<std::string> words = ft::split(directive_value, " \t\n");
			std::string val = words.front();

			if (val != "on" && val != "off")
				throw std::invalid_argument("Bad value for autoindex in config");
			_autoIndex = (val == "on" ? true : false);
		}
		else
		{
			std::string directive_value = raw.substr(i, raw.find('\n', i) - i);
			if (directive_value.size() == 0)
				throw std::invalid_argument("Empty directive value in config");
			_params.insert(std::make_pair(directive_name, ft::split(directive_value, " \t")));
		}
		if (raw.find('\n', i) == std::string::npos)
			break;
		i = raw.find('\n', i) + 1;
	}
	if ((std::find(_directive_names.begin(), _directive_names.end(), "listen") != _directive_names.end() && _params.find("listen") == _params.end()))
			throw std::invalid_argument("No listen in config");
	if (_params.find("root") == _params.end())
		_params["root"].push_back("./");
}

/*
** ---------------------------------- ACCESSOR --------------------------------------
*/

bool ConfigContext::hasAutoIndex() const
{
	return _autoIndex;
}

bool ConfigContext::hasAutoIndexPath(const std::string& path) const
{
	for (std::list<ConfigContext>::const_iterator it = _childs.begin(); it != _childs.end(); it++)
		if (it->getNames().front() == path.substr(0, std::min(path.size(), it->getNames().front().size())))
			return it->hasAutoIndex();
	return hasAutoIndex();
}

const std::list<std::string>& ConfigContext::getParam(const std::string& name) const
{
	if (_params.find(name) == _params.end())
		throw std::invalid_argument("Parameter not found in config");
	return _params.find(name)->second;
}

const std::list<std::string>& ConfigContext::getParamPath(const std::string& name, const std::string& path) const
{
	for (std::list<ConfigContext>::const_iterator it = _childs.begin(); it != _childs.end(); it++)
	{
		if (it->getNames().front() == path.substr(0, std::min(path.size(), it->getNames().front().size())))
			return it->getParam(name);
	}
	return getParam(name);
}

std::string ConfigContext::rootPath(const std::string& path) const
{
	std::string res = "./";

	for (std::list<ConfigContext>::const_iterator it = _childs.begin(); it != _childs.end(); it++)
	{
		if (it->getNames().front() == path.substr(0, std::min(path.size(), it->getNames().front().size())))
		{
			std::list<std::string> splitted = ft::split(it->getParam("root").front(), "/");
			std::list<std::string> splitted_path = ft::split(path.substr(it->getNames().front().size()), "/");

			for (std::list<std::string>::iterator it = splitted.begin(); it != splitted.end(); it++)
				res += *it + "/";
			for (std::list<std::string>::iterator it2 = splitted_path.begin(); it2 != splitted_path.end(); it2++)
				res += *it2 + "/";
			res.erase(--res.end()); // Enlève le / à la fin
			return res;
		}
	}
	std::list<std::string> splitted = ft::split(getParam("root").front(), "/");
	std::list<std::string> splitted_path = ft::split(path, "/");

	for (std::list<std::string>::iterator it = splitted.begin(); it != splitted.end(); it++)
		res += *it + "/";
	for (std::list<std::string>::iterator it2 = splitted_path.begin(); it2 != splitted_path.end(); it2++)
		res += *it2 + "/";
	res.erase(--res.end()); // Enlève le / à la fin
	return res;
}

std::list<ConfigContext>& ConfigContext::getChilds()
{
	return _childs;
}

std::string ConfigContext::getErrorPage(int code) const
{
	if (_error_pages.find(code) != _error_pages.end())
	{
		try
		{
			std::ifstream file(_error_pages.find(code)->second.c_str(), std::ifstream::in);
			std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
			return content;
		}
		catch (std::exception& e)
		{}
	}
	std::stringstream ss;
	ss << code;
	std::string code_str = ss.str();
	return
"<html>\r\n\
	<head><title>" + code_str + " " + ft::getErrorMessage(code) + "</title></head>\r\n\
	<body>\r\n\
		<center><h1>" + code_str + " " + ft::getErrorMessage(code) + "</h1></center>\r\n\
		<hr><center>Webserv 1.0.0</center>\r\n\
	</body>\r\n\
</html>";
}

std::string ConfigContext::getErrorPagePath(int code, const std::string& path) const
{
	for (std::list<ConfigContext>::const_iterator it = _childs.begin(); it != _childs.end(); it++)
	{
		if (it->getNames().front() == path.substr(0, std::min(path.size(), it->getNames().front().size())))
		{
			if (it->getErrorPages().find(code) != it->getErrorPages().end())
				return it->getErrorPage(code);
		}
	}
	return getErrorPage(code);
}

std::string ConfigContext::findFileWithRoot(const std::string& name) const
{
	const std::list<std::string>& roots = getParam("root");
	for (std::list<std::string>::const_iterator it = roots.begin(); it != roots.end(); it++)
	{
		std::ifstream file((*it + "/" + name).c_str());
		if (file.good())
			return *it + "/" + name.c_str();
	}
	throw std::invalid_argument("File doesn't exist.");
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
