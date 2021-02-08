#include <map>
#include <iostream>
#include "Logger.hpp"

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

		bool isValidRequestFormat()
		{
			static const size_t mth_size = 8;
			static const std::string methods[mth_size] = { "GET", "POST", "TRACE", "HEAD", "OPTIONS", "CONNECT", "PUT", "DELETE" };
			static const size_t ver_size = 1;
			static const std::string versions[ver_size] = { "HTTP/1.1" };

			// Method
			std::pair<std::string, size_t> token = read_until_space(0);
			for (int i = 0; i < mth_size; i++)
			{
				if (methods[i] == token.first)
					break ;
				if (i == mth_size - 1)
					return Logger::print("Request method could not be recognized.", false, ERROR, VERBOSE);
			}

			// Space
			if (count_concurrent_occurences(token.second, ' ') != 1)
				return Logger::print("Bad whitespace in request.", false, ERROR, VERBOSE);

			// Path
			token = read_until_space(skip_spaces(token.second));

			// Space
			if (count_concurrent_occurences(token.second, ' ') != 1)
				return Logger::print("Bad whitespace in request.", false, ERROR, VERBOSE);

			// Protocol Version
			token = read_until_next_line(skip_spaces(token.second));
			for (int i = 0; i < ver_size; i++)
			{
				if (versions[i] == token.first)
					break ;
				if (i == ver_size - 1)
					return Logger::print("Protocol version could not be recognized.", false, ERROR, VERBOSE);
			}

			return Logger::print("Request was parsed successfully.", true, SUCCESS, VERBOSE);
		}

		std::pair<std::string, size_t> read_until_space(size_t index)
		{
			std::string res;
			while (_raw[index] != ' ' && _raw[index] != '\n')
				res += _raw[index++];
			return std::make_pair(res, index);
		}

		std::pair<std::string, size_t> read_until_next_line(size_t index)
		{
			std::string res;
			while (_raw[index] && _raw[index] != '\n')
				res += _raw[index++];
			return std::make_pair(res, index);
		}

		size_t skip_spaces(size_t index)
		{
			while (_raw[index] == ' ' || _raw[index] == '\n')
				index++;
			return index;
		}

		size_t skip_lines(size_t index)
		{
			while (_raw[index] == '\n')
				index++;
			return index;
		}

		bool is_next_paragraph(size_t i)
		{
			return _raw[i - 1] == '\n' && _raw[i - 2] == '\n';
		}

		size_t count_concurrent_occurences(size_t index, char c = ' ')
		{
			size_t cnt = 0;
			while (_raw[index + cnt] == c)
				cnt++;
			return cnt;
		}

	public:
		Request(const Request& other) : _raw(other._raw), _method(other._method), _path(other._path), _protocolVersion(other._protocolVersion), _body(other._body) {}

		virtual ~Request() {}

		Request& operator=(const Request& other)
		{
			_raw = other._raw;
			_method = other._method;
			_path = other._path;
			_protocolVersion = other._protocolVersion;
			_body = other._body;
			return *this;
		}

		Request(const std::string& raw)
		{
			size_t i = 0;
			_raw = raw;
			if (!isValidRequestFormat())
				throw std::invalid_argument("Request is not in a valid format.");

			// Method
			std::pair<std::string, size_t> read_ret = read_until_space(i);
			_method = read_ret.first;
			i = skip_spaces(read_ret.second);

			// Path
			read_ret = read_until_space(i);
			_path = read_ret.first;
			i = skip_spaces(read_ret.second);

			// Protocol
			read_ret = read_until_space(i);
			_protocolVersion = read_ret.first;
			i = skip_spaces(read_ret.second);

			// Headers
			read_ret = read_until_next_line(i);
			i = skip_lines(read_ret.second);
			if (read_ret.first[0])
			{
				while (!is_next_paragraph(i))
				{
					std::string header_name = read_ret.first.substr(0, read_ret.first.find(": "));
					std::string header_val = read_ret.first.substr(read_ret.first.find(": ") + 2, read_ret.first.size());
					_headers[header_name] = header_val;
					read_ret = read_until_next_line(i);
					i = skip_lines(read_ret.second);
				}
				std::string header_name = read_ret.first.substr(0, read_ret.first.find(": "));
				std::string header_val = read_ret.first.substr(read_ret.first.find(": ") + 2, read_ret.first.size());
				_headers[header_name] = header_val;
			}
			_body = _raw.substr(i, _raw.size());
		}

		bool operator==(const Request& other)
		{
			return _raw == other._raw;
		}

		bool operator!=(const Request& other)
		{
			return !(*this == other);
		}

		friend std::ostream& operator<<(std::ostream& out, const Request& request);
};

std::ostream& operator<<(std::ostream& out, const Request& request)
{
	out <<	"Method: " << request._method << '\n' <<
			"Path: " << request._path << '\n' <<
			"Protocol Version: " << request._protocolVersion << '\n' <<
			"Headers: " << std::endl;
	for (Request::HeadersMap::const_iterator it = request._headers.begin(); it != request._headers.end(); it++)
		out << "  \"" << it->first << "\" -> \"" << it->second << '"' << std::endl;
	out << "Body:\n" << request._body << std::endl;
	return out;
}

int main()
{
	std::string raw, line;
	Logger::setMode(VERBOSE);
	while (std::getline(std::cin, line))
		raw += line + '\n';
	try
	{
		Request req(raw);
		std::cout << req;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
