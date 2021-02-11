#include "HeadMethod.hpp"

HeadMethod::HeadMethod() {}
HeadMethod::HeadMethod(const HeadMethod&) {}
HeadMethod::~HeadMethod() {}
HeadMethod& HeadMethod::operator=(const HeadMethod&) { return *this; }

std::string HeadMethod::getType() const { return "HEAD"; }

IMethod* HeadMethod::clone() const { return new HeadMethod(*this); }
