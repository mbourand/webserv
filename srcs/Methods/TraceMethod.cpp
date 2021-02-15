#include "TraceMethod.hpp"

TraceMethod::TraceMethod() {}
TraceMethod::TraceMethod(const TraceMethod&) {}
TraceMethod::~TraceMethod() {}
TraceMethod& TraceMethod::operator=(const TraceMethod&) { return *this; }

std::string TraceMethod::getType() const { return "TRACE"; }

IMethod* TraceMethod::clone() const { return new TraceMethod(*this); }

bool TraceMethod::allowAbsolutePath() const { return true; }
bool TraceMethod::allowCompleteURL() const { return true; }
bool TraceMethod::allowAuthorityURI() const { return false; }
bool TraceMethod::allowAsteriskURI() const { return false; }
