#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include <ctime>
#include "ConfigContext.hpp"

class Response
{
	private:
		int _code;
		bool		_caninflate;
		bool		_cangzip;
		std::string _message;
		std::map<std::string, std::string> _headers;
		std::string _body;
		std::string _path;

	public:
		Response();
		Response(const Response& other);
		Response(int code, const std::string& path);
		virtual ~Response();
		Response& operator=(const Response& other);

		Response&	setCode(int code);
		void		setMessage(const std::string& message);

		void		addHeader(const std::string& header_name, const std::string& header_value);
		void		addDateHeader(void);
		void		removeHeader(const std::string& header_name);

		void		setBody(const std::string& body);
		void		setCompression(const std::string& str);
		void		compressBody(const std::string &str);
		std::string	Chunk(const std::string& str);

		std::string getResponseText(const ConfigContext& config);
		int			getCode(void) const;
};

#endif
