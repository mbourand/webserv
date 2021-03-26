#include "DeleteMethod.hpp"
#include "Logger.hpp"

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

bool DeleteMethod::requestHasBody() const { return true; }
bool DeleteMethod::successfulResponseHasBody() const { return true; }
bool DeleteMethod::isSafe() const { return false; }
bool DeleteMethod::isIdempotent() const { return true; }
bool DeleteMethod::isCacheable() const { return false; }
bool DeleteMethod::isAllowedInHTMLForms() const { return false; }

Response DeleteMethod::process(const Request&, const ConfigContext&, const ServerSocket&)
{
	return Response();
}
