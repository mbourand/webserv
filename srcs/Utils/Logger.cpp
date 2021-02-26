#include "Logger.hpp"
#include <stdlib.h>

Logger::Logger() : _mode(NORMAL) {}
Logger::~Logger() {}
Logger::Logger(const Logger&) {}
Logger& Logger::operator=(const Logger&) { return Logger::getInstance(); }

const LoggerMode& Logger::getModeImpl() const
{
	return _mode;
}

void Logger::setModeImpl(const LoggerMode& mode)
{
	_mode = mode;
}

Logger Logger::_inst;
