#include "PutMethod.hpp"

PutMethod::PutMethod() {}
PutMethod::PutMethod(const PutMethod&) {}
PutMethod::~PutMethod() {}
PutMethod& PutMethod::operator=(const PutMethod&) { return *this; }

std::string PutMethod::getType() const { return "PUT"; }

IMethod* PutMethod::clone() const { return new PutMethod(*this); }

bool PutMethod::allowAbsolutePath() const { return true; }
bool PutMethod::allowCompleteURL() const { return true; }
bool PutMethod::allowAuthorityURI() const { return false; }
bool PutMethod::allowAsteriskURI() const { return false; }

bool PutMethod::requestHasBody() const { return true; }
bool PutMethod::successfulResponseHasBody() const { return false; }
bool PutMethod::isSafe() const { return false; }
bool PutMethod::isIdempotent() const { return true; }
bool PutMethod::isCacheable() const { return false; }
bool PutMethod::isAllowedInHTMLForms() const { return false; }

Response PutMethod::process(const Request&)
{
	return Response();
}
