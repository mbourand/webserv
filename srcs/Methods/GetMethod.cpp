#include "GetMethod.hpp"
#include "Logger.hpp"
#include "Request.hpp"
#include "Webserv.hpp"
#include "CGI.hpp"
#include <fstream>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "URL.hpp"
#include "Utils.hpp"
#include <dirent.h>
#include <algorithm>

GetMethod::GetMethod() {}
GetMethod::GetMethod(const GetMethod&) {}
GetMethod::~GetMethod() {}
GetMethod& GetMethod::operator=(const GetMethod&) { return *this; }

std::string GetMethod::getType() const { return "GET"; }

IMethod* GetMethod::clone() const { return new GetMethod(*this); }

bool GetMethod::allowAbsolutePath() const { return true; }
bool GetMethod::allowCompleteURL() const { return true; }
bool GetMethod::allowAuthorityURI() const { return false; }
bool GetMethod::allowAsteriskURI() const { return false; }

bool GetMethod::requestHasBody() const { return false; }
bool GetMethod::successfulResponseHasBody() const { return true; }
bool GetMethod::isSafe() const { return true; }
bool GetMethod::isIdempotent() const { return true; }
bool GetMethod::isCacheable() const { return true; }
bool GetMethod::isAllowedInHTMLForms() const { return true; }

std::list<std::string> GetMethod::list_directory(const std::string& realPath)
{
	std::list<std::string> list;
	DIR* dir;
	struct dirent *ent;

	if ((dir = opendir(realPath.c_str())) == NULL)
	{
		Logger::print("Error while trying to open directory", NULL, INFO, VERBOSE);
		throw std::invalid_argument("Error while trying to open directory");
	}
	while ((ent = readdir(dir)) != NULL)
		list.push_back(std::string(ent->d_name) + (ent->d_type == DT_DIR ? "/" : ""));
	closedir(dir);
	return list;
}

std::string GetMethod::get_last_modified_format(const std::string& realPath, const std::string& format)
{
	struct stat st;
	if (lstat(realPath.c_str(), &st) < 0)
		return "-";
	std::ostringstream convert;
	struct tm	time;
	strptime(std::string(convert.str()).c_str(), "%s", &time);
	convert << st.st_mtime;
	strptime(std::string(convert.str()).c_str(), "%s", &time);
	char		buffer[1024];
	strftime(buffer, sizeof(buffer), format.c_str(), &time);
	convert.str("");
	return buffer;
}

std::string GetMethod::get_file_size(const std::string& realPath)
{
	struct stat st;
	if (lstat(realPath.c_str(), &st) < 0)
		return "-";
	std::ostringstream convert;
	convert << st.st_size;
	return convert.str();
}

Response GetMethod::directory_listing(const Request& request, const ConfigContext&, const std::string &realPath)
{
	const URL& url = request._url;
	std::list<std::string> list;
	try
	{
		list = list_directory(realPath);
	}
	catch (std::exception& e)
	{
		if (errno == ENOENT)
			return Logger::print("Directory not found", Response(404, url._path), ERROR, VERBOSE);
		if (errno == EACCES)
			return Logger::print("Permission denied", Response(403, url._path), ERROR, VERBOSE);
		if (errno != 0)
			return Logger::print("Unknown error while trying to open directory", Response(500, url._path), ERROR, NORMAL);
	}


	std::string body =
"<html>\r\n\
	<head>\r\n\
		<title>Index</title>\r\n\
	</head>\r\n\
	<body>\r\n\
		<style>\r\n\
			table, td {\r\n\
				border: 1px solid black;\r\n\
			}\r\n\
\r\n\
			table {\r\n\
				width: 100%;\r\n\
				border-collapse: collapse;\r\n\
			}\r\n\
		</style>\r\n\
\r\n\
		<h1 align=\"center\">Index of " + url._path + "/</h1>\r\n\
		<hr>\r\n\
		<table>\r\n\
			<tbody>\r\n";


	for (std::list<std::string>::iterator it = ++list.begin(); it != list.end(); it++)
	{
		std::string path = realPath + "/" + *it;
		body +=
"				<tr>\r\n\
					<td><a href=\"" + url._path + "/" + *it + "\">" + *it + "</a></td>\r\n\
					<td>" + (path[path.size() - 1] == '/' ? "-" : get_last_modified_format(path, "%d-%b-%Y %H:%M")) + "</td>\r\n\
					<td>" + (path[path.size() - 1] == '/' ? "-" : get_file_size(path)) + "</td>\r\n\
				</tr>\r\n";
	}
	body +=
"			</tbody>\r\n\
		</table>\r\n\
	</body>\r\n\
</html>";


	Response response(200, url._path.substr(0, url._path.size() - 1));
	std::ostringstream convert;

	convert << body.size();
	response.addHeader("Content-Length", convert.str());
	response.setBody(body);
	convert.str("");
	response.addDateHeader();
	response.addHeader("Server", "Webserv");
	response.addHeader("Content-Type", "text/html");

	return response;
}

Response GetMethod::process(const Request& request, const ConfigContext& config, const ServerSocket& socket)
{
	const URL& url = request._url;
	const std::list<const IMethod*>& allowedMethods = config.getAllowedMethods();
	Response response(200, url._path);
	response.setCompression(request.getHeaderValue("Accept-Encoding"));
	if (std::find(allowedMethods.begin(), allowedMethods.end(), request._method) == allowedMethods.end())
		return (response.setCode(405));
	int base_depth = 0;
	std::string realPath = config.rootPath(url._path, base_depth);
	try
	{
		realPath = ft::simplify_path(realPath, true, base_depth);
	}
	catch (std::exception& e)
	{
		return Logger::print("Path is not safe", response.setCode(404), ERROR, VERBOSE);
	}

	if (realPath[0] != '/')
		realPath = g_webserv.cwd + "/" + realPath;
	if (!realPath.empty() && realPath != "/")
		realPath.erase(--realPath.end());


	if (ft::is_directory(realPath))
	{
		try
		{
			std::string index = "/" + config.getParam("index").front();
			realPath += index;
			std::string extension = index.substr(index.rfind('.')); // Vérifier que ça va pas throw est inutile, vu qu'on est dans un try catch
			if (config.getCGIExtensions().find(extension) != config.getCGIExtensions().end() || (realPath.find(config.getParam("cgi_dir").front()) == 0))	// Parse config, if file ext. associated with CGI or CGI bin found in path
				return process_cgi(realPath, url, config, socket, request);
		}
		catch (std::exception& e)
		{
			if (config.hasAutoIndex())
				return directory_listing(request, config, realPath);
		}
	}
	else if (url._is_directory)
	{
		return Logger::print("File not found", response.setCode(404), ERROR, VERBOSE);
	}


	std::list<std::string> splitted = ft::split(realPath, "/");
	std::string current_path;
	current_path.reserve(realPath.size());
	if (realPath[0] == '/')
		current_path.push_back('/');
	for (std::list<std::string>::iterator it = splitted.begin();it != splitted.end(); it++, current_path.push_back('/'))
	{
		current_path += *it;
		if (!ft::is_regular_file(current_path))
			continue;
		std::string file_name;
		if (ft::contains(current_path, '/'))
			file_name = current_path.substr(current_path.rfind('/'));
		else
			file_name = current_path;
		if (file_name.rfind('.') != std::string::npos)
		{
			std::string extension = file_name.substr(file_name.rfind('.'));
			if (config.getCGIExtensions().find(extension) != config.getCGIExtensions().end() || (realPath.find(config.getParam("cgi_dir").front()) == 0))	// Parse config, if file ext. associated with CGI or CGI bin found in path
				return process_cgi(realPath, url, config, socket, request);
		}
	}

	std::fstream file(realPath.c_str());
	if (!file.good() || !file.is_open())
	{
		if (errno == ENOENT || errno == ENOTDIR)
			return Logger::print("File not found", response.setCode(404), ERROR, VERBOSE);
		if (errno == EACCES || errno == EISDIR)
			return Logger::print("Permission denied", response.setCode(403), ERROR, VERBOSE);
		return Logger::print("Unexpected error while trying to open file: " + std::string(strerror(errno)), response.setCode(500), ERROR, VERBOSE);
	}


	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	std::ostringstream convert;
	struct stat file_stats;

	lstat(realPath.c_str(), &file_stats);
	convert << file_stats.st_size;
	response.addHeader("Content-Length", convert.str());
	convert.str("");


	convert << file_stats.st_mtime;
	struct tm	time;
	strptime(std::string(convert.str()).c_str(), "%s", &time);
	if (time.tm_gmtoff > 0) // convert to GMT
		file_stats.st_mtim.tv_sec -= time.tm_gmtoff;
	if (time.tm_isdst) // substract Daylight saving time
		file_stats.st_mtim.tv_sec -= 3600;
	convert.str("");

	convert << file_stats.st_mtime;
	strptime(std::string(convert.str()).c_str(), "%s", &time);
	char		buffer[1024];
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &time);
	response.addHeader("Last-Modified", buffer);
	convert.str("");


	response.addDateHeader();
	response.addHeader("Server", "Webserv");
	t_hnode	*hnode = g_webserv.file_formatname->GetNode(realPath.substr(realPath.find_last_of('.') + 1));
	if (hnode != NULL)
		response.addHeader("Content-Type", hnode->value);


	response.setBody(content);

	return (response);
}

Response GetMethod::process_cgi(const std::string& realPath, const URL& url, const ConfigContext& config, const ServerSocket& socket, const Request& request)
{
	Response response(200, url._path);
	if (!request.getHeaderValue("Accept-Encoding").empty())
		response.setCompression(request.getHeaderValue("Accept-Encoding"));
	try
	{
		CGI	cgi(request, config, socket, realPath);
		cgi.process(response);
	}
	catch(const CGI::CGIException &e)
	{
		Logger::print(e.what(), NULL, ERROR, NORMAL);
		response.setCode(e.code());
	}
	return (response);
}
