#include <fstream>
#include <sstream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "PostMethod.hpp"
#include "Logger.hpp"
#include "CGI.hpp"
#include "URL.hpp"
#include "Utils.hpp"
#include "Webserv.hpp"
#include <errno.h>

PostMethod::PostMethod() {}
PostMethod::PostMethod(const PostMethod&) {}
PostMethod::~PostMethod() {}
PostMethod& PostMethod::operator=(const PostMethod&) { return *this; }

std::string PostMethod::getType() const { return "POST"; }

IMethod* PostMethod::clone() const { return new PostMethod(*this); }

bool PostMethod::allowAbsolutePath() const { return true; }
bool PostMethod::allowCompleteURL() const { return true; }
bool PostMethod::allowAuthorityURI() const { return false; }
bool PostMethod::allowAsteriskURI() const { return false; }

bool PostMethod::requestHasBody() const { return true; }
bool PostMethod::successfulResponseHasBody() const { return true; }
bool PostMethod::isSafe() const { return false; }
bool PostMethod::isIdempotent() const { return false; }
bool PostMethod::isCacheable() const { return true; }
bool PostMethod::isAllowedInHTMLForms() const { return true; }

Response PostMethod::process(const Request& request, const ConfigContext& config, const ServerSocket& socket)
{
	const URL& url = request._url;
	int base_depth = 0;
	Response response(200, url._path); //change code on failure
	std::string realPath = config.rootPath(url._path, base_depth);
	try
	{
		realPath = ft::simplify_path(realPath, true, base_depth);
	}
	catch (std::exception& e)
	{
		return (response.setCode(404));
	}

	std::list<std::string> splitted = ft::split(realPath, "/");
	std::string extension = ft::get_extension(splitted.back());
	if ((!extension.empty() && config.getCGIExtensions().find(extension) != config.getCGIExtensions().end()) || (realPath.find(config.getParam("cgi_dir").front()) == 0))
	{
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
	}
	else
	{
		std::fstream file(realPath.c_str());
		if (!file.good() || !file.is_open())
		{
			if (errno == ENOENT || errno == ENOTDIR)
				return Logger::print("File not found", response.setCode(400), ERROR, VERBOSE);
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
	}
	return response;
}
