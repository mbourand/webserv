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

bool Logger::printImpl(std::string message, bool rval, MessageType type, LoggerMode messagePriority)
{
	if (messagePriority < _mode)
		return rval;

	// Format timestamp
	std::stringstream ss;
	std::time_t time_now = std::time(0);
	tm *local = localtime(&time_now);
	char buffer[100];
	strftime(buffer, 100, "[%d/%h/%Y %T %z]", local);

	ss.str("");
	for (size_t i = 0; i < message.size(); i++)
		if (message[i] < 32 || message[i] > 127)
		{
			ss << static_cast<int>(message[i]);
			message.replace(i, 1, "<" + ss.str() + ">");
			ss.str("");
		}

	switch (type)
	{
		case INFO:
			std::cout << RES << "[webserv] " << buffer << " (INFO): " << message << RES << std::endl;
			break;
		case SUCCESS:
			std::cout << GRN << "[webserv] " << buffer << " (SUCCESS): " << message << RES << std::endl;
			break;
		case WARNING:
			std::cerr << YLW << "[webserv] " << buffer << " (WARNING): " << message << RES << std::endl;
			break;
		case ERROR:
			std::cerr << RED << "[webserv] " << buffer << " (ERROR): " << message << RES << std::endl;
			break;
	}
	return rval;
}

Logger Logger::_inst;
