#include "PostMethod.hpp"
#include "Logger.hpp"

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

Response PostMethod::process(const Request&, const ConfigContext&)
{
	return Response();
}
