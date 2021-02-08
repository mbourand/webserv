#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>

#define RED "\033[31m"
#define GRN "\033[32m"
#define YLW "\033[33m"
#define RES "\033[37m"

enum LoggerMode
{
	VERBOSE,
	NORMAL,
	SILENT
};

enum MessageType
{
	INFO,
	SUCCESS,
	WARNING,
	ERROR
};

class Logger
{
	private:
		static Logger _inst;

		LoggerMode _mode;

		Logger();
		Logger(const Logger&);
		~Logger();
		Logger& operator=(const Logger&);

	public:
		static Logger& getInstance()
		{
			return _inst;
		}

		static const LoggerMode& getMode() { return getInstance().getModeImpl(); }
		static void setMode(const LoggerMode& mode) { getInstance().setModeImpl(mode); }
		static bool print(std::string message, bool rval, MessageType type = INFO, LoggerMode messagePriority = NORMAL) { return getInstance().printImpl(message, rval, type, messagePriority); }

	private:
		const LoggerMode& getModeImpl() const;
		void setModeImpl(const LoggerMode& mode);

		bool printImpl(std::string message, bool rval, MessageType type, LoggerMode messagePriority);
};

#endif
