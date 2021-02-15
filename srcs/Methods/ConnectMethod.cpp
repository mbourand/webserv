#include "ConnectMethod.hpp"

ConnectMethod::ConnectMethod() {}
ConnectMethod::ConnectMethod(const ConnectMethod&) {}
ConnectMethod::~ConnectMethod() {}
ConnectMethod& ConnectMethod::operator=(const ConnectMethod&) { return *this; }

std::string ConnectMethod::getType() const { return "CONNECT"; }

IMethod* ConnectMethod::clone() const { return new ConnectMethod(*this); }

bool ConnectMethod::allowAbsolutePath() const { return false; }
bool ConnectMethod::allowCompleteURL() const { return false; }
bool ConnectMethod::allowAuthorityURI() const { return true; }
bool ConnectMethod::allowAsteriskURI() const { return false; }
