#include "GetMethod.hpp"
#include "Logger.hpp"
#include "Request.hpp"
#include "Webserv.hpp"
#include <fstream>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "URL.hpp"
#include "Utils.hpp"
#include <dirent.h>

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

Response GetMethod::directory_listing(const Request& request, const ConfigContext& config)
{
	URL url(request._path);
	std::string realPath = config.rootPath(url._path);

	std::list<std::string> list;
	std::string body = "<html>\n\t<head>\n\t\t<title>Index</title>\n\t</head>\n\t<body>\n";

	DIR* dir;
	struct dirent *ent;

	if ((dir = opendir(realPath.c_str())) == NULL)
	{
		if (errno == ENOENT)
			return Logger::print("Directory not found", Response(404, url._path), ERROR, VERBOSE);
		if (errno == EACCES)
			return Logger::print("Permission denied", Response(403, url._path), ERROR, VERBOSE);
		return Logger::print("Unexpected error while trying to open directory", Response(500, url._path));
	}
	while ((ent = readdir(dir)) != NULL)
		list.push_back(std::string(ent->d_name) + (ent->d_type == DT_DIR ? "/" : ""));
	closedir(dir);

	body += "\t\t<style>\n\t\t\ttable, td {\n\t\t\t\tborder: 1px solid black;\n\t\t\t}\n\n\t\t\ttable {\n\t\t\t\twidth: 100%;\n\t\t\t\tborder-collapse: collapse;\n\t\t\t}\n\t\t</style>\n";
	body += "\t\t<h1 align=\"center\">Index of " + url._path + "/</h1>\n\t\t<hr>\n\t\t<table>\n\t\t\t<tbody>\n";
	for (std::list<std::string>::iterator it = ++list.begin(); it != list.end(); it++)
		body += "\t\t\t\t<tr><td><a href=\"" + *it + "\">" + *it + "</a></td></tr>\n";

	body += "\t\t\t</tbody>\n\t\t</table>\n\t</body>\n</html>\n";

	Response response(200, url._path.substr(0, url._path.size() - 1));
	std::ostringstream convert;

	convert << body.size();
	response.addHeader("Content-Length", convert.str());
	response.setBody(body);
	convert.str("");

	struct timeval 	tv;
	struct tm time;
	char buffer[1024];
	gettimeofday(&tv, NULL);
	tv.tv_sec -= 3600; // assume we're GMT+1
	convert << tv.tv_sec;
	strptime(std::string(convert.str()).c_str(), "%s", &time);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &time);
	response.addHeader("Date", buffer);

	response.addHeader("Server", "Webserv");
	response.addHeader("Content-Type", "text/html");

	return response;
}

Response GetMethod::process(const Request& request, const ConfigContext& config)
{
	URL url(request._path);
	std::string realPath = config.rootPath(url._path);

	if (url._is_directory && config.hasAutoIndexPath(url._path))
		return directory_listing(request, config);

	std::fstream file(realPath.c_str());

	if (!file.good() || !file.is_open())
	{
		if (errno == ENOENT)
			return Logger::print("File not found", Response(404, url._path), ERROR, VERBOSE);
		if (errno == EACCES || errno == EISDIR)
			return Logger::print("Permission denied", Response(403, url._path), ERROR, VERBOSE);
		return Logger::print("Unexpected error while trying to open file", Response(500, url._path));
	}

	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	Response response(200, url._path);
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

	struct timeval 	tv;
	gettimeofday(&tv, NULL);
	tv.tv_sec -= 3600; // assume we're GMT+1
	convert << tv.tv_sec;
	strptime(std::string(convert.str()).c_str(), "%s", &time);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &time);
	response.addHeader("Date", buffer);

	response.addHeader("Server", "Webserv");

	t_hnode	*hnode = g_webserv.file_formatname->GetNode(realPath.substr(realPath.find_last_of('.') + 1));
	if (hnode != NULL)
		response.addHeader("Content-Type", hnode->value);

	response.setBody(content);

	return response;
}
