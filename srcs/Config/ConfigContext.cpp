#include "ConfigContext.hpp"
#include "Logger.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <list>
#include "Webserv.hpp"
#include "Methods.h"



/*
** ---------------------------------- CONSTRUCTOR --------------------------------------
*/



ConfigContext::ConfigContext()
	: _autoIndex(false)
{}



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
	_acceptedMethods = other._acceptedMethods;
	_cgi_exts = other._cgi_exts;
	_uploads_exts = other._uploads_exts;
	return *this;
}



ConfigContext::ConfigContext(const std::string& raw, const std::string& name, const ConfigContext* parent)
	: _autoIndex(false)
{
	handle_parent_and_directives(parent, name);

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
		if (!ft::contains(_directive_names, directive_name))
			throw std::invalid_argument("Unknown directive name in config");

		// Pour empêcher une directive d'apparaître deux fois dans le même context
		if (_params.find(directive_name) != _params.end())
			_params.erase(_params.find(directive_name));

		i += directive_name.size();
		i = raw.find_first_not_of(" \t", i);

		std::string directive_value = raw.substr(i, raw.find('\n', i) - i);
		if (directive_value.empty())
			throw std::invalid_argument("Empty directive value in config");

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
			parse_server_name(directive_value);
		else if (directive_name == "error_page")
			parse_error_page(directive_value);
		else if (directive_name == "autoindex")
			parse_autoindex(directive_value);
		else if (directive_name == "disable_methods")
			parse_methods(directive_value);
		else if (directive_name == "cgi_ext")
			parse_cgi_ext(directive_value);
		else if (directive_name == "uploads_exts")
			parse_uploads_exts(directive_value);
		else if (directive_name == "auth_basic")
			parse_auth_basic(directive_value);
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
	if (ft::contains(_directive_names, "listen") && _params.find("listen") == _params.end())
		throw std::invalid_argument("No listen in config");
	set_uploads_default();
	set_root_default();
	set_max_body_size_default();
	set_cgi_dir_default();
	set_auth_basic_default();
	if (_params["auth_basic"].front() != "")
		set_auth_basic_user_file_default();
}



void ConfigContext::parse_auth_basic(const std::string& directive_value)
{
	std::list<std::string> words = ft::split(directive_value, " \t\n");
	if (words.size() == 1 && words.front() == "off")
	{
		_params["auth_basic"].assign(1, "");
		return;
	}
	_params["auth_basic"].assign(1, directive_value);
}



void ConfigContext::add_server_directives()
{
	_directive_names.push_back("server_name");
	_directive_names.push_back("listen");
	_directive_names.push_back("location");
}



void ConfigContext::add_location_directives()
{
	_directive_names.push_back("root");
	_directive_names.push_back("error_page");
	_directive_names.push_back("index");
	_directive_names.push_back("autoindex");
	_directive_names.push_back("max_client_body_size");
	_directive_names.push_back("disable_methods");
	_directive_names.push_back("cgi_dir");
	_directive_names.push_back("cgi_ext");
	_directive_names.push_back("uploads");
	_directive_names.push_back("uploads_exts");
	_directive_names.push_back("auth_basic");
	_directive_names.push_back("auth_basic_user_file");
}



void ConfigContext::handle_parent_and_directives(const ConfigContext* parent, const std::string& name)
{
	if (parent != NULL)
	{
		_params = parent->_params;
		_error_pages = parent->_error_pages;
		_autoIndex = parent->_autoIndex;
		_acceptedMethods = parent->_acceptedMethods;
		_cgi_exts = parent->_cgi_exts;
		_uploads_exts = parent->_uploads_exts;
		_names.push_back(name);
	}
	else
	{
		// Directives réservées aux contexts server{}
		add_server_directives();
		for (std::list<IMethod*>::const_iterator it = g_webserv.methods.getRegistered().begin(); it != g_webserv.methods.getRegistered().end(); it++)
			_acceptedMethods.push_back(*it);
	}
	add_location_directives();
}



void ConfigContext::parse_uploads_exts(const std::string& directive_value)
{
	std::list<std::string> words = ft::split(directive_value, " \t\n");
	for (std::list<std::string>::iterator it = words.begin(); it != words.end(); it++)
	{
		if ((*it)[0] != '.')
		{
			if (*it == "all")
			{
				_uploads_exts.clear();
				_uploads_exts.push_back("all");
			}
			else if (*it == "none")
				_uploads_exts.clear();
			else
				throw std::invalid_argument("Extension doesn't start with '.' for uploads_exts in config");
			return;
		}
		_uploads_exts.push_back(*it);
	}
}



void ConfigContext::parse_cgi_ext(const std::string& directive_value)
{
	std::list<std::string> words = ft::split(directive_value, " \t\n");
	if (words.size() & 1)
		throw std::invalid_argument("No cgi path was provided for cgi_ext in config");
	bool extension_arg = true;
	std::string extension;
	for (std::list<std::string>::iterator it = words.begin(); it != words.end(); it++)
	{
		if (extension_arg)
		{
			if ((*it)[0] != '.')
				throw std::invalid_argument("Extension doesn't start with a dot for cgi_ext in config");
			extension = *it;
		}
		else
			_cgi_exts.insert(std::make_pair(extension, *it));
		extension_arg = !extension_arg;
	}
	for (std::map<std::string, std::string>::iterator it = _cgi_exts.begin(); it != _cgi_exts.end(); it++)
	{
		it->second = ft::simplify_path(it->second);
		if (!ft::is_executable(it->second.c_str()))
			throw std::invalid_argument("cgi_ext parameter is not an executable in config");
	}
}



void ConfigContext::set_auth_basic_user_file_default()
{
	if (_params.find("auth_basic_user_file") == _params.end())
		return;

	_params["auth_basic_user_file"].front() = ft::simplify_path(_params["auth_basic_user_file"].front());
	if (!ft::is_regular_file(_params["auth_basic_user_file"].front().c_str()))
		throw std::invalid_argument("Auth Basic User File parameter is not a regular file");
}



void ConfigContext::set_root_default()
{
	if (_params.find("root") == _params.end())
		_params["root"].push_back("./");
	else
	{
		_params["root"].front() = ft::simplify_path(_params["root"].front());
		if (!ft::is_directory(_params["root"].front().c_str()))
			throw std::invalid_argument("Root parameter is not a folder in config");
	}
}



void ConfigContext::set_cgi_dir_default()
{
	if (_params.find("cgi_dir") == _params.end())
		_params["cgi_dir"].push_back("/cgi-bin/");
	else
	{
		_params["cgi_dir"].front() = ft::simplify_path(_params["cgi_dir"].front());
		if (!ft::is_directory(_params["cgi_dir"].front().c_str()))
			throw std::invalid_argument("cgi_dir parameter is not a folder in config");
	}
}



void ConfigContext::parse_methods(const std::string& directive_value)
{
	std::list<std::string> words = ft::split(directive_value, " \t\n");
	_acceptedMethods.assign(g_webserv.methods.getRegistered().begin(), g_webserv.methods.getRegistered().end());
	if (words.front() == "none")
		return;
	for (std::list<std::string>::const_iterator it = words.begin(); it != words.end(); it++)
	{
		if (g_webserv.methods.getByType(*it) == NULL)
			throw std::invalid_argument("Bad method name in method field in config.");
		_acceptedMethods.remove(g_webserv.methods.getByType(*it));
	}
}



void ConfigContext::parse_autoindex(const std::string& directive_value)
{
	std::list<std::string> words = ft::split(directive_value, " \t\n");
	std::string val = words.front();

	if (val != "on" && val != "off")
		throw std::invalid_argument("Bad value for autoindex in config");
	_autoIndex = (val == "on" ? true : false);
}



void ConfigContext::parse_error_page(const std::string& directive_value)
{
	std::list<std::string> words = ft::split(directive_value, " \t\n");
	std::string page = words.back();

	for (std::list<std::string>::const_iterator it = words.begin(); it != --words.end(); it++)
	{
		if (!ft::is_integer<int>(*it))
			throw std::invalid_argument("Bad integer for error_page in config");
		int nb = ft::toInt(*it);

		if (page == "none")
		{
			if (_error_pages.find(nb) != _error_pages.end())
				_error_pages.erase(_error_pages.find(nb));
		}
		else
			_error_pages.insert(std::make_pair(ft::toInt(*it), page));
	}
}



void ConfigContext::parse_server_name(const std::string& directive_value)
{
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



void ConfigContext::set_uploads_default()
{
	if (_params.find("uploads") == _params.end())
		_params["uploads"].push_back("./");
	else
	{
		_params["uploads"].front() = ft::simplify_path(_params["uploads"].front());
		if (!ft::is_directory(_params["uploads"].front().c_str()))
			throw std::invalid_argument("Uploads parameter is not a folder in config");
	}
}



void ConfigContext::set_auth_basic_default()
{
	if (_params.find("auth_basic") == _params.end())
		_params["auth_basic"].push_back("");
}



void ConfigContext::set_max_body_size_default()
{
	if (_params.find("max_client_body_size") == _params.end())
		_params["max_client_body_size"].push_back("8000");
	else
	{
		if (!ft::is_integer<int>(_params["max_client_body_size"].front()))
			throw std::invalid_argument("max_client_body_size is not an integer in config.");
		if (_params["max_client_body_size"].front()[0] == '-')
			throw std::invalid_argument("Negative max_client_body_size in config.");
	}
}



/*
** ---------------------------------- ACCESSOR --------------------------------------
*/



const std::map<std::string, std::string>& ConfigContext::getCGIExtensions() const { return _cgi_exts; }
const std::map<std::string, std::list<std::string> >& ConfigContext::getParams() const { return _params; }
const std::map<int, std::string>& ConfigContext::getErrorPages() const { return _error_pages; }
const std::list<std::string>& ConfigContext::getNames() const { return _names; }
const std::list<ConfigContext>& ConfigContext::getChilds() const { return _childs; }
const std::list<const IMethod*>& ConfigContext::getAllowedMethods() const { return _acceptedMethods; }
bool ConfigContext::hasAutoIndex() const { return _autoIndex; }



const std::list<std::string>& ConfigContext::getParam(const std::string& name) const
{
	if (_params.find(name) == _params.end())
		throw std::invalid_argument("Parameter not found in config");
	return _params.find(name)->second;
}



/**
 * @brief Récupère soit la page d'erreur configurée dans la config pour une erreur donnée, soit la page d'erreur par défaut si il n'y en a pas
 *
 * @param error_code
 * @return Le body de la page d'erreur
 */
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
		catch (std::exception& e) {}
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
</html>\r\n";
}



/*
** --------------------------------------- METHODS ---------------------------------------
*/



/**
 * @brief Transforme un chemin url en chemin physique
 *
 * @param url_path
 * @param base_depth
 * @return Le chemin url transformé en chemin physique, selon le root de la config
 */
std::string ConfigContext::rootPath(const std::string& path, int& base_depth) const
{
	std::string res;

	res = (getParam("root").front()[0] == '/' ? "/" : "");

	std::list<std::string> splitted = ft::split(getParam("root").front(), "/");
	base_depth = splitted.size();
	std::list<std::string> splitted_path = ft::split(path.substr(getNames().front().size()), "/");

	for (std::list<std::string>::iterator it = splitted.begin(); it != splitted.end(); it++)
		res += *it + "/";
	for (std::list<std::string>::iterator it2 = splitted_path.begin(); it2 != splitted_path.end(); it2++)
		res += *it2 + "/";
	if (res[res.size() - 1] == '/' && res != "/")
		res.erase(--res.end()); // Enlève le / à la fin
	return res;
}



/**
 * @brief Récupère la config correspondant à un chemin url
 *
 * @param path
 * @return Le ConfigContext de la 1re location qui match avec path, sinon retourne *this
 */
const ConfigContext& ConfigContext::getConfigPath(const std::string& path) const
{
	for (std::list<ConfigContext>::const_iterator it = _childs.begin(); it != _childs.end(); it++)
	{
		if (it->getNames().front() == path.substr(0, std::min(path.size(), it->getNames().front().size())))
			return *it;
	}
	return *this;
}



bool	ConfigContext::can_be_uploaded(const std::string& extension) const
{
	if (_uploads_exts.empty())
		return false;
	if (_uploads_exts.front() == "all")
		return true;
	if (ft::contains(_uploads_exts, extension))
		return true;
	return false;
}



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
