#include "Webserv.hpp"
#include "Utils.hpp"
#include "Types_parser.hpp"
#include <fstream>

#ifndef DEBUG
# define DEBUG 0
#endif

void s_webserv::init_factories()
{
	methods.add(new ConnectMethod());
	methods.add(new DeleteMethod());
	methods.add(new GetMethod());
	methods.add(new HeadMethod());
	methods.add(new OptionsMethod());
	methods.add(new PostMethod());
	methods.add(new PutMethod());
	methods.add(new TraceMethod());

	headers.add(new AcceptCharsetsHeader());
	headers.add(new AcceptEncodingHeader());
	headers.add(new AcceptLanguageHeader());
	headers.add(new AllowHeader());
	headers.add(new AuthorizationHeader());
	headers.add(new ContentLanguageHeader());
	headers.add(new ContentLengthHeader());
	headers.add(new ContentLocationHeader());
	headers.add(new ContentTypeHeader());
	headers.add(new DateHeader());
	headers.add(new HostHeader());
	headers.add(new LastModifiedHeader());
	headers.add(new LocationHeader());
	headers.add(new RefererHeader());
	headers.add(new RetryAfterHeader());
	headers.add(new ServerHeader());
	headers.add(new TransferEncodingHeader());
	headers.add(new UserAgentHeader());
	headers.add(new WWWAuthenticateHeader());
}

s_webserv::s_webserv()
	: run(true), debug(DEBUG), file_formatname(new HashTable(256)), cwd(ft::get_cwd()), workers_amount(0), max_connections(100), compression_deflate(true), compression_gzip(true), compression_level(6)
{
	init_factories();
	parse_types_file(file_formatname, "/etc/mime.types");
}

/**
 * @brief Parse la Config globale, et initialise les ConfigContext server{} dans des VirtualHosts, stockés dans s_webserv::vhosts
 *
 * @param filename
 */
void s_webserv::init_config(const std::string& filename)
{
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file.good() || !file.is_open())
		throw std::invalid_argument("Couldn't open configuration file");

	std::string raw((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));


	if (std::count(raw.begin(), raw.end(), '{') != std::count(raw.begin(), raw.end(), '}'))
		throw std::invalid_argument("Bad braces in config.");


	for (size_t i = 0; i < raw.size();)
	{
		while (raw[i] == ' ' || raw[i] == '\t')
			i++;
		if (raw[i] == '\n' || raw[i] == '#')
		{
			if (raw.find('\n', i) == std::string::npos)
				break;
			i = raw.find('\n', i) + 1;
			continue;
		}
		std::string directive_name = raw.substr(i, raw.find_first_of(" \t", i) - i);
		if (directive_name == "workers")
		{
			i += directive_name.size();
			while (raw[i] == ' ' || raw[i] == '\t')
				i++;

			std::string directive_value = raw.substr(i, raw.find('\n', i) - i);
			if (directive_value.empty())
				throw std::invalid_argument("Empty directive value in config");

			std::list<std::string> words = ft::split(directive_value, " \t\n");
			std::string amount_str = words.front();
			if (!ft::is_integer<int>(amount_str))
				throw std::invalid_argument("Bad integer value in config");

			workers_amount = ft::toInt(amount_str);
			i += directive_value.size();
		}
		else if (directive_name == "max_connections")
		{
			i += directive_name.size();
			while (raw[i] == ' ' || raw[i] == '\t')
				i++;

			std::string directive_value = raw.substr(i, raw.find('\n', i) - i);
			if (directive_value.empty())
				throw std::invalid_argument("Empty directive value in config");

			std::list<std::string> words = ft::split(directive_value, " \t\n");
			std::string amount_str = words.front();
			if (!ft::is_integer<int>(amount_str))
				throw std::invalid_argument("Bad integer value in config");

			max_connections = ft::toInt(amount_str);
			i += directive_value.size();
		}
		else if (directive_name == "compression_level")
		{
			i += directive_name.size();
			while (raw[i] == ' ' || raw[i] == '\t')
				i++;

			std::string directive_value = raw.substr(i, raw.find('\n', i) - i);
			if (directive_value.empty())
				throw std::invalid_argument("Empty directive value in config");

			std::list<std::string> words = ft::split(directive_value, " \t\n");
			std::string amount_str = words.front();
			if (!ft::is_integer<int>(amount_str))
				throw std::invalid_argument("Bad integer value in config");

			compression_level = ft::toInt(amount_str);
			if (!compression_level || compression_level > 9)
				throw std::invalid_argument("Bad integer value in config");
			i += directive_value.size();
		}
		else if (directive_name == "server")
		{
			i += 6;
			while (raw[i] == ' ' || raw[i] == '\t')
				i++;
			if (raw[i] == '{' && raw[i + 1] == '\n')
				i += 2;
			else
				throw std::invalid_argument("Expected newline after { in config");

			size_t nesting = 1;
			size_t start_i = i;
			while (i < raw.size())
			{
				if (raw[i] == '{')
					nesting++;
				else if (raw[i] == '}')
					nesting--;
				if (nesting == 0)
					break;
				i++;
			}
			if (nesting != 0)
				throw std::invalid_argument("Bad braces in config file");
			if (raw[i + 1] != '\n')
				throw std::invalid_argument("Expected newline after } in config");
			vhosts.push_back(VirtualHost(ConfigContext(raw.substr(start_i, i - start_i))));
		}
		else
			throw std::invalid_argument("Bad directive name in config");
		if (raw.find('\n', i) == std::string::npos)
			break;
		i = raw.find('\n', i) + 1;
	}
	Logger::print("Config was parsed successfully", NULL, SUCCESS, SILENT);
}
