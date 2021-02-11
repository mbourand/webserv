#include "DeleteMethod.hpp"

DeleteMethod::DeleteMethod() {}
DeleteMethod::DeleteMethod(const DeleteMethod&) {}
DeleteMethod::~DeleteMethod() {}
DeleteMethod& DeleteMethod::operator=(const DeleteMethod&) { return *this; }

std::string DeleteMethod::getType() const { return "DELETE"; }

IMethod* DeleteMethod::clone() const { return new DeleteMethod(*this); }
