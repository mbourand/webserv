#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <sys/time.h>
#include <time.h>
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
		template<class T>
		static T print(std::string message, T rval, MessageType type = INFO, LoggerMode messagePriority = NORMAL) { return getInstance().printImpl(message, rval, type, messagePriority); }

	private:
		const LoggerMode& getModeImpl() const;
		void setModeImpl(const LoggerMode& mode);

		template<class T>
		T printImpl(std::string message, T rval, MessageType type, LoggerMode messagePriority)
		{
			if (messagePriority < _mode)
				return rval;

			// Format timestamp
			std::stringstream ss;
			struct timeval 	tv;
			tm time;
			gettimeofday(&tv, NULL);
			ss << tv.tv_sec;
			strptime(std::string(ss.str()).c_str(), "%s", &time);
			char buffer[100];
			strftime(buffer, 100, "[%d/%h/%Y %T %z]", &time);

			// Replace non-printable characters by their code
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
};

#endif
