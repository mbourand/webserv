#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>
#include "Logger.hpp"

#define ABSOLUTE_PATH (1 << 0)
#define COMPLETE_URL (1 << 1)
#define URL_AUTHORITY (1 << 2)
#define ASTERISK_URI (1 << 3)

class Request
{
	public:
		typedef std::map<std::string, std::string> HeadersMap;

	protected:
		std::string _raw;
		std::string _method;
		std::string _path;
		std::string _protocolVersion;
		HeadersMap _headers;
		std::string _body;

	private:
		Request() {}

		bool isValidRequestFormat() const;
		bool header_line_valid(const std::string& line) const;
		bool is_next_paragraph(size_t i) const;
		size_t count_concurrent_occurences(size_t index, char c) const;
		std::pair<std::string, size_t> read_charset(size_t index, const std::string& charset) const;
		std::pair<std::string, size_t> read_until_charset(size_t index, const std::string& charset) const;
		bool is_valid_URI(const std::string& uri, int mask) const;

	public:
		Request(const Request& other);
		virtual ~Request();
		Request& operator=(const Request& other);
		Request(const std::string& raw);

		bool operator==(const Request& other) const;
		bool operator!=(const Request& other) const;

		friend std::ostream& operator<<(std::ostream& out, const Request& request);
};

#endif
