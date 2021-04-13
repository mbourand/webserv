#include "OptionsMethod.hpp"
#include "Logger.hpp"
#include "Request.hpp"
#include "URL.hpp"
#include "Utils.hpp"
#include "Webserv.hpp"

OptionsMethod::OptionsMethod() {}
OptionsMethod::OptionsMethod(const OptionsMethod&) {}
OptionsMethod::~OptionsMethod() {}
OptionsMethod& OptionsMethod::operator=(const OptionsMethod&) { return *this; }

std::string OptionsMethod::getType() const { return "OPTIONS"; }

IMethod* OptionsMethod::clone() const { return new OptionsMethod(*this); }

bool OptionsMethod::allowAbsolutePath() const { return true; }
bool OptionsMethod::allowCompleteURL() const { return true; }
bool OptionsMethod::allowAuthorityURI() const { return false; }
bool OptionsMethod::allowAsteriskURI() const { return true;}

bool OptionsMethod::requestHasBody() const { return false; }
bool OptionsMethod::successfulResponseHasBody() const { return true; }
bool OptionsMethod::isSafe() const { return true; }
bool OptionsMethod::isIdempotent() const { return true; }
bool OptionsMethod::isCacheable() const { return false; }
bool OptionsMethod::isAllowedInHTMLForms() const { return false; }

Response OptionsMethod::process(const Request& request, const ConfigContext& config, const ServerSocket& socket)
{
	std::string	allowed_header;
	if (request._url._path == "*")
	{
		const std::list<const IMethod*>& allowedMethods = g_webserv.vhosts.front().getConfig().getAllowedMethods();
		if (std::find(allowedMethods.begin(), allowedMethods.end(), request._method) == allowedMethods.end())
			return Response(405, request._url._path);
		for (std::list<const IMethod*>::const_iterator it = allowedMethods.begin(); it != allowedMethods.end(); it++)
		{
			if (it != allowedMethods.begin())
				allowed_header += ", ";
			allowed_header += (*it)->getType();
		}
	}
	else
	{
		const std::list<const IMethod*>& allowedMethods = config.getAllowedMethods();
		if (std::find(allowedMethods.begin(), allowedMethods.end(), request._method) == allowedMethods.end())
			return Response(405, request._url._path);
		for (std::list<const IMethod*>::const_iterator it = allowedMethods.begin(); it != allowedMethods.end(); it++)
		{
			if (it != allowedMethods.begin())
				allowed_header += ", ";
			allowed_header += (*it)->getType();
		}
	}
	Response response(200, request._url._path);
	response.addHeader("Allow", allowed_header);
	response.addDateHeader();
	response.addHeader("Server", "Webserv");
	response.addHeader("Content-Length", "0");
	return (response);
}
