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

		Logger() : _mode(NORMAL) {}

	public:
		static Logger& getInstance()
		{
			return _inst;
		}

		static const LoggerMode& getMode() { return getInstance().getModeImpl(); }
		static void setMode(const LoggerMode& mode) { getInstance().setModeImpl(mode); }
		static bool print(std::string message, bool rval, MessageType type = INFO, LoggerMode messagePriority = NORMAL) { return getInstance().printImpl(message, rval, type, messagePriority); }

	private:
		const LoggerMode& getModeImpl() const { return _mode; }
		void setModeImpl(const LoggerMode& mode) { _mode = mode; }

		bool printImpl(std::string message, bool rval, MessageType type, LoggerMode messagePriority)
		{
			if (messagePriority < _mode)
				return rval;

			// Format timestamp
			std::stringstream ss;
			std::time_t time_now = std::time(0);
			tm *local = localtime(&time_now);
			char buffer[100];
			strftime(buffer, 100, "[%d/%h/%Y %T %z]", local);

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
};

Logger Logger::_inst;
