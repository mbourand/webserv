#include "HeadMethod.hpp"

HeadMethod::HeadMethod() {}
HeadMethod::HeadMethod(const HeadMethod&) {}
HeadMethod::~HeadMethod() {}
HeadMethod& HeadMethod::operator=(const HeadMethod&) { return *this; }

std::string HeadMethod::getType() const { return "HEAD"; }

IMethod* HeadMethod::clone() const { return new HeadMethod(*this); }

bool HeadMethod::allowAbsolutePath() const { return true; }
bool HeadMethod::allowCompleteURL() const { return true; }
bool HeadMethod::allowAuthorityURI() const { return false; }
bool HeadMethod::allowAsteriskURI() const { return false; }

bool HeadMethod::requestHasBody() const { return false; }
bool HeadMethod::successfulResponseHasBody() const { return false; }
bool HeadMethod::isSafe() const { return true; }
bool HeadMethod::isIdempotent() const { return true; }
bool HeadMethod::isCacheable() const { return true; }
bool HeadMethod::isAllowedInHTMLForms() const { return false; }

Response HeadMethod::process(const Request& request, const ConfigContext& config, const ServerSocket& socket)
{
	Response response = _getMethod.process(request, config, socket);
	response.setBody("");
	return response;
}
