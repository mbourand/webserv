#include "OptionsMethod.hpp"

OptionsMethod::OptionsMethod() {}
OptionsMethod::OptionsMethod(const OptionsMethod&) {}
OptionsMethod::~OptionsMethod() {}
OptionsMethod& OptionsMethod::operator=(const OptionsMethod&) { return *this; }

std::string OptionsMethod::getType() const { return "OPTIONS"; }

IMethod* OptionsMethod::clone() const { return new OptionsMethod(*this); }

bool OptionsMethod::allowAbsolutePath() const { return true; }
bool OptionsMethod::allowCompleteURL() const { return true; }
bool OptionsMethod::allowAuthorityURI() const { return false; }
bool OptionsMethod::allowAsteriskURI() const { return false;}

bool OptionsMethod::requestHasBody() const { return false; }
bool OptionsMethod::successfulResponseHasBody() const { return true; }
bool OptionsMethod::isSafe() const { return true; }
bool OptionsMethod::isIdempotent() const { return true; }
bool OptionsMethod::isCacheable() const { return false; }
bool OptionsMethod::isAllowedInHTMLForms() const { return false; }
