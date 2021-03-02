#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>
#include "Logger.hpp"

#include "Methods.h"
#include "Headers.h"
#include "Factory.hpp"

#include <vector>

class Request
{
	public:
		typedef std::vector<Header*> HeadersVector;

	protected:
		Factory<IMethod*> _methodFactory;
		Factory<Header*> _headerFactory;
		std::string _raw;
		IMethod* _method;
		std::string _path;
		std::string _protocolVersion;
		HeadersVector _headers;
		std::string _body;
		bool _header_section_finished;
		bool _finished_parsing;
		size_t _parse_start;

	private:

		bool header_line_valid(const std::string& line) const;
		bool is_next_paragraph(size_t i) const;
		size_t count_concurrent_occurences(size_t index, char c) const;
		bool is_valid_URI(const std::string& uri) const;

		void parse_method();
		void parse_uri();
		void parse_protocol_version();
		bool parse_headers();
		void parse_body();

		void init_factories();

	public:
		Request();
		Request(const Request& other);
		virtual ~Request();
		Request& operator=(const Request& other);

		bool append(const std::string& raw);
		void parse();
		bool isfinished(void) const;

		bool operator==(const Request& other) const;
		bool operator!=(const Request& other) const;

		friend std::ostream& operator<<(std::ostream& out, const Request& request);
};

#endif
