#include "URL.hpp"
#include <stdexcept>

/*
** ------------------------------- CONSTRUCTOR -------------------------------
*/

URL::URL()
{

}

URL::URL(const std::string& url)
{
	if (url.size() == 0)
		throw std::invalid_argument("Empty url");
	if (url[0] == '/')
		parse_partial(url);
	else
		parse_full(url);
	if (_port == "")
		_port = "80";
	if (_path == "")
		_path = "/";
}

URL::URL(const URL& other)
{
	*this = other;
}

URL::~URL() {}

/*
** ------------------------------- METHOD -------------------------------
*/

void URL::parse_userinfo(size_t& i, const std::string& url)
{
	std::string authority = url.substr(i, std::min(url.find("/", i), url.size()) - i);

	if (authority.find('@') == 0)
		throw std::invalid_argument("@ delimiter is present but no user info was given in uri");
	if (authority.find('@') == std::string::npos)
	{
		_userinfo = "";
		return;
	}
	_userinfo = authority.substr(0, authority.find('@'));
	i += _userinfo.size() + 1;
}

#include <iostream>
void URL::parse_full(const std::string& url)
{
	if (url.substr(0, 7) != "http://")
		throw std::invalid_argument("Full url doesnt start with 'http://'");
	_scheme = "http";
	size_t i = 7;

	parse_userinfo(i, url);
	_host = url.substr(i, url.find_first_of(":/", i) - i);
	if (_host.size() == 0)
		throw std::invalid_argument("Empty host in uri");
	i += _host.size();
	if (url[i] == ':')
	{
		i++;
		_port = url.substr(i, url.find_first_of("/", i) - i);
		if (_port.size() == 0)
			throw std::invalid_argument("Empty port in uri");
		i += _port.size();
	}
	if (url[i] == '/')
		parse_partial(url.substr(i));
}

void URL::parse_partial(const std::string& url)
{
	size_t i = 0;

	_path = url.substr(i, std::min(url.find_first_of("?#", i), url.size()) - i);
	i += _path.size();
	if (url[i] == '?')
	{
		i++;
		_query = url.substr(i, std::min(url.find_first_of("#", i), url.size()) - i);
		if (_query.size() == 0)
			throw std::invalid_argument("Empty query in url");
		i += _query.size();
	}
	if (url[i] == '#')
	{
		i++;
		_fragment = url.substr(i, url.size() - i);
		if (_fragment.size() == 0)
			throw std::invalid_argument("Empty fragment in url");
	}
}

/*
** ------------------------------- OPERATOR -------------------------------
*/

URL& URL::operator=(const URL& other)
{
	_scheme = other._scheme;
	_userinfo = other._userinfo;
	_host = other._host;
	_port = other._port;
	_query = other._query;
	_fragment = other._fragment;
	return *this;
}
