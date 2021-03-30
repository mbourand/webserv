#include "PostMethod.hpp"
#include "Logger.hpp"
#include "CGI.hpp"

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
	return response;
}
