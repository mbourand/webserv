#include "TraceMethod.hpp"
#include "Request.hpp"
#include "Logger.hpp"
#include <sstream>
#include <ctime>
#include <sys/time.h>
#include "URL.hpp"

TraceMethod::TraceMethod() {}
TraceMethod::TraceMethod(const TraceMethod&) {}
TraceMethod::~TraceMethod() {}
TraceMethod& TraceMethod::operator=(const TraceMethod&) { return *this; }

std::string TraceMethod::getType() const { return "TRACE"; }

IMethod* TraceMethod::clone() const { return new TraceMethod(*this); }

bool TraceMethod::allowAbsolutePath() const { return true; }
bool TraceMethod::allowCompleteURL() const { return true; }
bool TraceMethod::allowAuthorityURI() const { return false; }
bool TraceMethod::allowAsteriskURI() const { return false; }

bool TraceMethod::requestHasBody() const { return false; }
bool TraceMethod::successfulResponseHasBody() const { return false; }
bool TraceMethod::isSafe() const { return true; }
bool TraceMethod::isIdempotent() const { return true; }
bool TraceMethod::isCacheable() const { return false; }
bool TraceMethod::isAllowedInHTMLForms() const { return false; }

Response TraceMethod::process(const Request& request, const ConfigContext& config, const ServerSocket&)
{
	const URL& url = request._url;
	Response response(200, url._path);
	response.addHeader("Content-Type", "message/http");
	response.addHeader("Server", "Webserv");
	response.addDateHeader();
	response.setBody(request._raw);
	return response;
}
