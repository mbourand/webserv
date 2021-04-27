#include "DeleteMethod.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Logger.hpp"
#include "Utils.hpp"
#include "URL.hpp"
#include "Request.hpp"
#include "Webserv.hpp"

DeleteMethod::DeleteMethod() {}
DeleteMethod::DeleteMethod(const DeleteMethod&) {}
DeleteMethod::~DeleteMethod() {}
DeleteMethod& DeleteMethod::operator=(const DeleteMethod&) { return *this; }

std::string DeleteMethod::getType() const { return "DELETE"; }

IMethod* DeleteMethod::clone() const { return new DeleteMethod(*this); }

bool DeleteMethod::allowAbsolutePath() const { return true; }
bool DeleteMethod::allowCompleteURL() const { return true; }
bool DeleteMethod::allowAuthorityURI() const { return false; }
bool DeleteMethod::allowAsteriskURI() const { return false; }

bool DeleteMethod::requestHasBody() const { return false; }
bool DeleteMethod::successfulResponseHasBody() const { return true; }
bool DeleteMethod::isSafe() const { return false; }
bool DeleteMethod::isIdempotent() const { return true; }
bool DeleteMethod::isCacheable() const { return false; }
bool DeleteMethod::isAllowedInHTMLForms() const { return false; }
#include <string.h>
#include <iostream>
#include <errno.h>
Response DeleteMethod::process(const Request& request, const ConfigContext& config, const ServerSocket&)
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
	if (realPath.rfind('/') == realPath.size() - 1)
		realPath = realPath.erase(realPath.size() - 1);
	std::string file_path = config.getParam("uploads").front() + realPath.substr(realPath.find('/'));
	struct stat	file_stats;
	std::cout << file_path << std::endl;
	if (lstat(file_path.c_str(), &file_stats) == 0 && S_ISREG(file_stats.st_mode))
	{
		if (unlink(file_path.c_str()) == 0)
			response.setCode(204);
		else
			response.setCode(202);
	}
	else
		response.setCode(404);
	return (response);
}
