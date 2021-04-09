#include "PostMethod.hpp"
#include "Logger.hpp"
#include "CGI.hpp"
#include "URL.hpp"
#include "Utils.hpp"

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
	URL url(request._path);
	const std::list<const IMethod*>& allowedMethods = config.getAllowedMethodsPath(url._path);
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

	Response response(200, url._path); //change code on failure
	std::list<std::string> splitted = ft::split(realPath, "/");
	if (splitted.back().rfind(".") != std::string::npos)
	{
		std::string extension = realPath.substr(realPath.rfind("."));
		if (config.getCGIExtensionsPath(url._path).find(extension) != config.getCGIExtensionsPath(url._path).end() || (realPath.find(config.getParamPath("cgi-dir", url._path).front()) == 0))	// Parse config, if file ext. associated with CGI or CGI bin found in path
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
	}
	return response;
}
