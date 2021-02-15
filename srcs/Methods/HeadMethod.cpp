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
