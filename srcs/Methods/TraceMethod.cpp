#include "TraceMethod.hpp"
#include "Request.hpp"
#include "Logger.hpp"
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include <ctime>

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

Response TraceMethod::process(const Request &request)
{
	Response response(200, "OK");
	char				buffer[1024];
	struct timeval 		tv;
	struct tm			time;
	std::ostringstream	convert;
	gettimeofday(&tv, NULL);
	tv.tv_sec -= 3600;
	convert << tv.tv_sec;
	strptime(std::string(convert.str()).c_str(), "%s", &time);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &time);
	response.addHeader("Content-Type", "message/http");
	response.addHeader("Server", "Webserv");
	response.addHeader("Date", buffer);
	response.setBody(request._raw);
	return response;
}
