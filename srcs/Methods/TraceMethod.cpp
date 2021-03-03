#include "TraceMethod.hpp"
#include "Logger.hpp"

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

Response TraceMethod::process(const Request&)
{
	return Response();
}
