#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "PutMethod.hpp"
#include "Request.hpp"
#include "Logger.hpp"
#include "Utils.hpp"
#include "URL.hpp"
#include "Webserv.hpp"

PutMethod::PutMethod() {}
PutMethod::PutMethod(const PutMethod&) {}
PutMethod::~PutMethod() {}
PutMethod& PutMethod::operator=(const PutMethod&) { return *this; }

std::string PutMethod::getType() const { return "PUT"; }

IMethod* PutMethod::clone() const { return new PutMethod(*this); }

bool PutMethod::allowAbsolutePath() const { return true; }
bool PutMethod::allowCompleteURL() const { return true; }
bool PutMethod::allowAuthorityURI() const { return false; }
bool PutMethod::allowAsteriskURI() const { return false; }

bool PutMethod::requestHasBody() const { return true; }
bool PutMethod::successfulResponseHasBody() const { return false; }
bool PutMethod::isSafe() const { return false; }
bool PutMethod::isIdempotent() const { return true; }
bool PutMethod::isCacheable() const { return false; }
bool PutMethod::isAllowedInHTMLForms() const { return false; }

Response PutMethod::process(const Request& request, const ConfigContext& config, const ServerSocket&)
{
	URL url(request._url._path);
	const std::list<const IMethod*>& allowedMethods = config.getAllowedMethods();
	if (std::find(allowedMethods.begin(), allowedMethods.end(), request._method) == allowedMethods.end())
		return Response(405, url._path);


	int base_depth = 0;
	std::string realPath = config.rootPath(url._path, base_depth);
	try
	{
		realPath = ft::simplify_path(realPath, true, base_depth);
	}
	catch (std::exception& e)
	{
		return Response(404, url._path);
	}


	Response response(200, url._path);
	if (realPath.rfind("/") == realPath.size() - 1)
		realPath = realPath.erase(realPath.size() - 1);
	if (realPath.rfind('.') >= (realPath.size() - 1) || realPath.rfind("/") != config.getParam("root").front().size()) // if no ext or target is subfoler inside uploads
		return Response(415, url._path);


	std::string extension = realPath.substr(realPath.rfind('.') + 1); //TODO: config "uploads_ext html" only allow certains files types to be uploaded
	if (extension != "html" || !g_webserv.file_formatname->GetNode(extension)
	|| request.getHeaderValue("Content-Type") != g_webserv.file_formatname->Lookup(extension))// si l'extension n'est pas autoritée ou que le content-type déclaré ne correspond pas au content-type connu
		return Response(415, url._path); //TODO: lookup extension in uploads_ext withelist


	std::string file_path = config.getParam("uploads").front() + realPath.substr(realPath.find("/"));
	std::fstream file;
	struct stat	file_stats;
	if (lstat(file_path.c_str(), &file_stats) < 0 || static_cast<unsigned int>(file_stats.st_size) != request._body.size())
	{
		file.open(file_path.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
		if (!S_ISREG(file_stats.st_mode))
			response.setCode(201);
	}
	else if (!S_ISREG(file_stats.st_mode))
		return Response(500, url._path);
	else
		file.open(file_path.c_str());
	if (!file.good() || !file.is_open())
		return Response(500, url._path);

	response.addHeader("Content-Location", realPath.substr(realPath.find("/")));


	std::stringstream ss;
	ss << file.rdbuf();
	if (request._body.size() == ss.str().size() && ss.str() == request._body)
	{
		response.setCode(204);
	}
	else
	{
		file.seekg(std::fstream::beg);
		file << request._body;
	}


	file.close();
	return (response);
}
