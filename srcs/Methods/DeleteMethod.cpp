#include "DeleteMethod.hpp"

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
