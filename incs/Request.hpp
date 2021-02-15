#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>
#include "Logger.hpp"

#include "Methods.h"
#include "Headers.h"
#include "Factory.hpp"

class Request
{
	public:
		typedef std::map<std::string, std::string> HeadersMap;

	protected:
		Factory<IMethod*> _methodFactory;
		Factory<IHeader*> _headerFactory;
		std::string _raw;
		IMethod* _method;
		std::string _path;
		std::string _protocolVersion;
		HeadersMap _headers;
		std::string _body;

	private:
		Request() {}

		bool header_line_valid(const std::string& line) const;
		bool is_next_paragraph(size_t i) const;
		size_t count_concurrent_occurences(size_t index, char c) const;
		std::pair<std::string, size_t> read_charset(size_t index, const std::string& charset) const;
		std::pair<std::string, size_t> read_until_charset(size_t index, const std::string& charset) const;
		bool is_valid_URI(const std::string& uri) const;

		void parse_method();
		void parse_uri();
		void parse_protocol_version();
		void parse_headers();
		void parse_body();

		void init_factories();

		size_t parse_start;

	public:
		Request(const Request& other);
		virtual ~Request();
		Request& operator=(const Request& other);
		Request(std::string raw = "");

		bool append(const std::string& raw);
		void parse();

		bool operator==(const Request& other) const;
		bool operator!=(const Request& other) const;

		friend std::ostream& operator<<(std::ostream& out, const Request& request);
};

#endif
