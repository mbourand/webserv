#include "Utils.hpp"
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits>

int ft::toInt(const std::string& str)
{
	for (size_t i = 0; i < str.size(); ++i)
		if (!std::isdigit(str[i]))
			throw std::invalid_argument("String is not an integer");

	std::istringstream iss(str);
	int ret = 0;
	iss >> ret;
	return ret;
}

float	ft::toFloat(const std::string& str)
{
	if (str.empty())
		return 0;


	if (str.find_first_not_of("0123456789.-+") != std::string::npos)
		throw std::invalid_argument("String is not a float");

	if (std::count(str.begin(), str.end(), '-') == 1)
		if (str[0] != '-')
			throw std::invalid_argument("String is not a float");

	if (std::count(str.begin(), str.end(), '+') == 1)
		if (str[0] != '+')
			throw std::invalid_argument("String is not a float");

	if (std::count(str.begin(), str.end(), '.') > 1)
		throw std::invalid_argument("String is not a float");

	std::istringstream iss(str);
	float ret = 0;
	iss >> ret;
	return ret;
}

std::string ft::toString(int i)
{
	std::stringstream ss;
	ss << i;
	return ss.str();
}

std::string ft::toHex(int i)
{
	std::stringstream ss;
	ss << std::hex << i;
	return ss.str();
}

int			ft::hexToInt(const std::string& str)
{
	unsigned int out;
    std::stringstream ss;
    ss << std::hex << str;
    ss >> out;
	return (static_cast<int>(out));
}

std::list<std::string> ft::split(const std::string& str, const std::string& charset)
{
	std::list<std::string> ret;

	size_t i = str.find_first_not_of(charset, 0);
	while (i < str.size())
	{
		size_t word_start = i;
		i = str.find_first_of(charset, i);
		size_t word_end = i;
		i = str.find_first_not_of(charset, i);
		ret.push_back(str.substr(word_start, word_end - word_start));
	}
	return ret;
}

std::pair<std::string, int> ft::complete_ip(const std::string& incomplete_ip)
{
	std::pair<std::string, int> ret;

	std::string ip = "0.0.0.0";
	int port = 80;

	if (incomplete_ip.size() == 0 || incomplete_ip == ":")
		return std::make_pair(ip, port);
	if (incomplete_ip.find_first_not_of("0123456789:.") != std::string::npos || std::count(incomplete_ip.begin(), incomplete_ip.end(), ':') > 1)
		throw std::invalid_argument("Bad ip");

	if (ft::contains(incomplete_ip, ':'))
	{
		std::string part1 = incomplete_ip.substr(0, incomplete_ip.find(':'));
		std::string part2 = incomplete_ip.substr(incomplete_ip.find(':') + 1);
		if (part1.size() != 0)
		{
			if (std::count(part1.begin(), part1.end(), '.') != 3)
				throw std::invalid_argument("Bad ip");
			ip = part1;
		}
		if (part2.size() != 0)
		{
			if (part2.size() > 5)
				throw std::invalid_argument("Bad port");
			port = ft::toInt(part2);
		}
		return std::make_pair(ip, port);
	}
	if (std::count(incomplete_ip.begin(), incomplete_ip.end(), '.') == 3)
		ip = incomplete_ip;
	else if (std::count(incomplete_ip.begin(), incomplete_ip.end(), '.') == 0)
	{
		if (incomplete_ip.size() > 5)
			throw std::invalid_argument("Bad port");
		port = ft::toInt(incomplete_ip);
	}
	else
		throw std::invalid_argument("Bad ip");
	return std::make_pair(ip, port);
}

/**
 * @brief Retourne le message correspondant au code http
 *
 * @param code
 * @return Le message correspondant au code http
 */
std::string ft::getErrorMessage(int code)
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

bool ft::is_directory(std::string realPath)
{
	struct stat st;
	if (realPath[0] != '/')
		realPath = ft::get_cwd() + "/" + realPath;
	if (lstat(realPath.c_str(), &st) < 0)
		return false;
	return S_ISDIR(st.st_mode);
}

bool ft::is_regular_file(std::string realPath)
{
	struct stat st;
	if (realPath[0] != '/')
		realPath = ft::get_cwd() + "/" + realPath;
	if (lstat(realPath.c_str(), &st) < 0)
		return false;
	return S_ISREG(st.st_mode);
}

bool ft::is_executable(std::string realPath)
{
	struct stat st;
	if (realPath[0] != '/')
		realPath = ft::get_cwd() + "/" + realPath;
	if (lstat(realPath.c_str(), &st) < 0)
		return false;
	return st.st_mode & S_IXUSR;
}

/**
 * @brief Simplifie un chemin en enlevant les '/' en trop, les '..' inutiles, etc.
 *
 * @param input
 * @param safe Si safe est activé, le chemin ne peut pas accéder au parent du dossier (ex: "../" invalide, "./test/../test" valide, "./test/../.." invalide)
 * @param base_depth Le nombre de / que safe doit ignorer en début de chaîne (ne sert à rien si safe est false)
 * @return Le chemin simplifié
 */
std::string ft::simplify_path(const std::string& input, bool safe, int base_depth)
{
	if (input.size() == 0)
		return "";

	std::list<std::string> splitted = ft::split(input, "/");
	std::string ret;

	if (safe)
	{
		int depth = 0;
		for (std::list<std::string>::iterator it = splitted.begin(); it != splitted.end(); it++)
		{
			if (base_depth-- > 0)
				continue;
			if (*it == ".")
				continue;

			if (*it == "..")
				depth--;
			else
				depth++;
			if (depth == -1)
				throw std::invalid_argument("Path go out of current directory");
		}
	}

	if (input[0] == '/')
		ret += "/";

	for (std::list<std::string>::iterator it = splitted.begin(); it != splitted.end(); it++)
	{
		if (*it == ".")
			continue;
		else if (*it == ".." && ret.size() > 0)
		{
			it--;
			if (*it == "..")
				ret += "../";
			else
				ret = ret.substr(0, ret.size() - it->size() - 1);
			it++;
		}
		else
			ret += *it + "/";
	}
	return ret;
}

std::string ft::get_cwd()
{
	std::string cwd;
	char *buf = getcwd(NULL, 0);
	if (buf == NULL)
		return "";
	cwd += buf;
	free(buf);
	return cwd;
}

char	*ft::strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];
	for ( ; i < n; i++)
		dest[i] = '\0';
	return (dest);
}

void	*ft::memset(void *s, int c, size_t n)
{
	unsigned char	*start;

	start = (unsigned char*)s;
	while (n > 0)
	{
		*start++ = c;
		n--;
	}
	return (s);
}

void	ft::bzero(void *str, size_t len)
{
	ft::memset(str, 0, len);
}

std::string ft::get_extension(std::string path)
{
	if (path[path.size() - 1] == '/')
		path.erase(--path.end());

	if (!ft::contains(path, '.'))
		return "";

	if (!ft::contains(path, '/'))
	{
		if (path.rfind('.') == 0)
			return "";
		return path.substr(path.rfind('.'));
	}

	if (!ft::contains(path.substr(path.rfind('/') + 1), '.'))
		return "";

	if (path.substr(path.rfind('/') + 1).rfind('.') == 0)
		return "";

	return path.substr(path.rfind('/') + 1 + (path.rfind('.') - path.rfind('/') - 1));
}
