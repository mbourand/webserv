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

Response GetMethod::process(const Request& request, const ConfigContext& config, const ServerSocket& socket)
{
	Response response(200); //change code on failure
	if ((request._path.find(".php") != std::string::npos) || (request._path.find("/cgi-bin") == 0))	// Parse config, if file ext. associated with CGI or CGI bin found in path
	{
		try
		{
			CGI	cgi(request, config, socket);
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
		std::fstream file((std::string("www/") + request._path).c_str());
		if (!file.good() || !file.is_open())
		{
			if (errno == ENOENT)
				return Logger::print("File not found", Response(404), ERROR, VERBOSE);
			if (errno == EACCES)
				return Logger::print("Premission denied", Response(403), ERROR, VERBOSE);
			return Logger::print("Unexpected error while trying to open file", Response(500));
		}
		std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
		std::ostringstream convert;
		struct stat file_stats;
		lstat((std::string("www/") + request._path).c_str(), &file_stats);
		convert << file_stats.st_size;
		response.addHeader("Content-Length", convert.str());
		convert.str("");
		convert << file_stats.st_mtime;
		struct tm	time;
		strptime(std::string(convert.str()).c_str(), "%s", &time);
		if (time.tm_gmtoff > 0)
			file_stats.st_mtim.tv_sec -= time.tm_gmtoff;
		convert.str("");
		convert << file_stats.st_mtime;
		strptime(std::string(convert.str()).c_str(), "%s", &time);
		char		buffer[1024];
		strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &time);
		response.addHeader("Last-Modified", buffer);
		response.addDateHeader();
		response.addHeader("Server", "Webserv");
		t_hnode	*hnode = g_webserv.file_formatname->GetNode(request._path.substr(request._path.find_last_of('.') + 1));
		if (hnode != NULL)
			response.addHeader("Content-Type", hnode->value);
		response.setBody(content);
	}
	return response;
}
