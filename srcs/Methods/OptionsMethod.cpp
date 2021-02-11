#include "OptionsMethod.hpp"

OptionsMethod::OptionsMethod() {}
OptionsMethod::OptionsMethod(const OptionsMethod&) {}
OptionsMethod::~OptionsMethod() {}
OptionsMethod& OptionsMethod::operator=(const OptionsMethod&) { return *this; }

std::string OptionsMethod::getType() const { return "OPTIONS"; }

IMethod* OptionsMethod::clone() const { return new OptionsMethod(*this); }
