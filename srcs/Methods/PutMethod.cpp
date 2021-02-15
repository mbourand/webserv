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
