#include "Webserv.hpp"
#include "Utils.hpp"
#include "Types_parser.hpp"
#include <fstream>

#ifndef DEBUG
# define DEBUG 0
#endif

s_webserv::s_webserv()
	: run(true), debug(DEBUG), file_formatname(new HashTable(256)), languages(new HashTable(256)), cwd(ft::get_cwd()), workers_amount(0), max_connections(100), compression_deflate(false), compression_gzip(false), compression_level(6)
{
	init_factories();
	parse_types_file(file_formatname, (cwd+"/config/mime.types").c_str());
	parse_types_file(file_formatname, "/etc/mime.types");
	init_languages();
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
		else if (directive_name == "gzip")
		{
			i += directive_name.size();
			while (raw[i] == ' ' || raw[i] == '\t')
				i++;
			std::string directive_value = raw.substr(i, raw.find('\n', i) - i);
			if (directive_value.empty())
				throw std::invalid_argument("Empty directive value in config");
			else if (directive_value == "on")
				compression_gzip = true;
			else if (directive_value == "off")
				compression_gzip = false;
			else
				throw std::invalid_argument("Bad string value in config");
			i += directive_value.size();
		}
		else if (directive_name == "deflate")
		{
			i += directive_name.size();
			while (raw[i] == ' ' || raw[i] == '\t')
				i++;
			std::string directive_value = raw.substr(i, raw.find('\n', i) - i);
			if (directive_value.empty())
				throw std::invalid_argument("Empty directive value in config");
			else if (directive_value == "on")
				compression_deflate = true;
			else if (directive_value == "off")
				compression_deflate = false;
			else
				throw std::invalid_argument("Bad string value in config");
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



void s_webserv::init_languages()
{
	languages->Insert("af", "Afrikaans");
	languages->Insert("af-ZA", "Afrikaans (South Africa)");
	languages->Insert("ar", "Arabic");
	languages->Insert("ar-AE", "Arabic (U.A.E.)");
	languages->Insert("ar-BH", "Arabic (Bahrain)");
	languages->Insert("ar-DZ", "Arabic (Algeria)");
	languages->Insert("ar-EG", "Arabic (Egypt)");
	languages->Insert("ar-IQ", "Arabic (Iraq)");
	languages->Insert("ar-JO", "Arabic (Jordan)");
	languages->Insert("ar-KW", "Arabic (Kuwait)");
	languages->Insert("ar-LB", "Arabic (Lebanon)");
	languages->Insert("ar-LY", "Arabic (Libya)");
	languages->Insert("ar-MA", "Arabic (Morocco)");
	languages->Insert("ar-OM", "Arabic (Oman)");
	languages->Insert("ar-QA", "Arabic (Qatar)");
	languages->Insert("ar-SA", "Arabic (Saudi Arabia)");
	languages->Insert("ar-SY", "Arabic (Syria)");
	languages->Insert("ar-TN", "Arabic (Tunisia)");
	languages->Insert("ar-YE", "Arabic (Yemen)");
	languages->Insert("az", "Azeri (Latin)");
	languages->Insert("az-AZ", "Azeri (Latin) (Azerbaijan)");
	languages->Insert("az-Cyrl-AZ", "Azeri (Cyrillic) (Azerbaijan)");
	languages->Insert("be", "Belarusian");
	languages->Insert("be-BY", "Belarusian (Belarus)");
	languages->Insert("bg", "Bulgarian");
	languages->Insert("bg-BG", "Bulgarian (Bulgaria)");
	languages->Insert("bs-BA", "Bosnian (Bosnia and Herzegovina)");
	languages->Insert("ca", "Catalan");
	languages->Insert("ca-ES", "Catalan (Spain)");
	languages->Insert("cs", "Czech");
	languages->Insert("cs-CZ", "Czech (Czech Republic)");
	languages->Insert("cy", "Welsh");
	languages->Insert("cy-GB", "Welsh (United Kingdom)");
	languages->Insert("da", "Danish");
	languages->Insert("da-DK", "Danish (Denmark)");
	languages->Insert("de", "German");
	languages->Insert("de-AT", "German (Austria)");
	languages->Insert("de-CH", "German (Switzerland)");
	languages->Insert("de-DE", "German (Germany)");
	languages->Insert("de-LI", "German (Liechtenstein)");
	languages->Insert("de-LU", "German (Luxembourg)");
	languages->Insert("dv", "Divehi");
	languages->Insert("dv-MV", "Divehi (Maldives)");
	languages->Insert("el", "Greek");
	languages->Insert("el-GR", "Greek (Greece)");
	languages->Insert("en", "English");
	languages->Insert("en-AU", "English (Australia)");
	languages->Insert("en-BZ", "English (Belize)");
	languages->Insert("en-CA", "English (Canada)");
	languages->Insert("en-CB", "English (Caribbean)");
	languages->Insert("en-GB", "English (United Kingdom)");
	languages->Insert("en-IE", "English (Ireland)");
	languages->Insert("en-JM", "English (Jamaica)");
	languages->Insert("en-NZ", "English (New Zealand)");
	languages->Insert("en-PH", "English (Republic of the Philippines)");
	languages->Insert("en-TT", "English (Trinidad and Tobago)");
	languages->Insert("en-US", "English (United States)");
	languages->Insert("en-ZA", "English (South Africa)");
	languages->Insert("en-ZW", "English (Zimbabwe)");
	languages->Insert("eo", "Esperanto");
	languages->Insert("es", "Spanish");
	languages->Insert("es-AR", "Spanish (Argentina)");
	languages->Insert("es-BO", "Spanish (Bolivia)");
	languages->Insert("es-CL", "Spanish (Chile)");
	languages->Insert("es-CO", "Spanish (Colombia)");
	languages->Insert("es-CR", "Spanish (Costa Rica)");
	languages->Insert("es-DO", "Spanish (Dominican Republic)");
	languages->Insert("es-EC", "Spanish (Ecuador)");
	languages->Insert("es-ES", "Spanish (Spain)");
	languages->Insert("es-GT", "Spanish (Guatemala)");
	languages->Insert("es-HN", "Spanish (Honduras)");
	languages->Insert("es-MX", "Spanish (Mexico)");
	languages->Insert("es-NI", "Spanish (Nicaragua)");
	languages->Insert("es-PA", "Spanish (Panama)");
	languages->Insert("es-PE", "Spanish (Peru)");
	languages->Insert("es-PR", "Spanish (Puerto Rico)");
	languages->Insert("es-PY", "Spanish (Paraguay)");
	languages->Insert("es-SV", "Spanish (El Salvador)");
	languages->Insert("es-UY", "Spanish (Uruguay)");
	languages->Insert("es-VE", "Spanish (Venezuela)");
	languages->Insert("et", "Estonian");
	languages->Insert("et-EE", "Estonian (Estonia)");
	languages->Insert("eu", "Basque");
	languages->Insert("eu-ES", "Basque (Spain)");
	languages->Insert("fa", "Farsi");
	languages->Insert("fa-IR", "Farsi (Iran)");
	languages->Insert("fi", "Finnish");
	languages->Insert("fi-FI", "Finnish (Finland)");
	languages->Insert("fo", "Faroese");
	languages->Insert("fo-FO", "Faroese (Faroe Islands)");
	languages->Insert("fr", "French");
	languages->Insert("fr-BE", "French (Belgium)");
	languages->Insert("fr-CA", "French (Canada)");
	languages->Insert("fr-CH", "French (Switzerland)");
	languages->Insert("fr-FR", "French (France)");
	languages->Insert("fr-LU", "French (Luxembourg)");
	languages->Insert("fr-MC", "French (Principality of Monaco)");
	languages->Insert("gl", "Galician");
	languages->Insert("gl-ES", "Galician (Spain)");
	languages->Insert("gu", "Gujarati");
	languages->Insert("gu-IN", "Gujarati (India)");
	languages->Insert("he", "Hebrew");
	languages->Insert("he-IL", "Hebrew (Israel)");
	languages->Insert("hi", "Hindi");
	languages->Insert("hi-IN", "Hindi (India)");
	languages->Insert("hr", "Croatian");
	languages->Insert("hr-BA", "Croatian (Bosnia and Herzegovina)");
	languages->Insert("hr-HR", "Croatian (Croatia)");
	languages->Insert("hu", "Hungarian");
	languages->Insert("hu-HU", "Hungarian (Hungary)");
	languages->Insert("hy", "Armenian");
	languages->Insert("hy-AM", "Armenian (Armenia)");
	languages->Insert("id", "Indonesian");
	languages->Insert("id-ID", "Indonesian (Indonesia)");
	languages->Insert("is", "Icelandic");
	languages->Insert("is-IS", "Icelandic (Iceland)");
	languages->Insert("it", "Italian");
	languages->Insert("it-CH", "Italian (Switzerland)");
	languages->Insert("it-IT", "Italian (Italy)");
	languages->Insert("ja", "Japanese");
	languages->Insert("ja-JP", "Japanese (Japan)");
	languages->Insert("ka", "Georgian");
	languages->Insert("ka-GE", "Georgian (Georgia)");
	languages->Insert("kk", "Kazakh");
	languages->Insert("kk-KZ", "Kazakh (Kazakhstan)");
	languages->Insert("kn", "Kannada");
	languages->Insert("kn-IN", "Kannada (India)");
	languages->Insert("ko", "Korean");
	languages->Insert("ko-KR", "Korean (Korea)");
	languages->Insert("kok", "Konkani");
	languages->Insert("kok-IN", "Konkani (India)");
	languages->Insert("ky", "Kyrgyz");
	languages->Insert("ky-KG", "Kyrgyz (Kyrgyzstan)");
	languages->Insert("lt", "Lithuanian");
	languages->Insert("lt-LT", "Lithuanian (Lithuania)");
	languages->Insert("lv", "Latvian");
	languages->Insert("lv-LV", "Latvian (Latvia)");
	languages->Insert("mi", "Maori");
	languages->Insert("mi-NZ", "Maori (New Zealand)");
	languages->Insert("mk", "FYRO Macedonian");
	languages->Insert("mk-MK", "FYRO Macedonian (Former Yugoslav Republic of Macedonia)");
	languages->Insert("mn", "Mongolian");
	languages->Insert("mn-MN", "Mongolian (Mongolia)");
	languages->Insert("mr", "Marathi");
	languages->Insert("mr-IN", "Marathi (India)");
	languages->Insert("ms", "Malay");
	languages->Insert("ms-BN", "Malay (Brunei Darussalam)");
	languages->Insert("ms-MY", "Malay (Malaysia)");
	languages->Insert("mt", "Maltese");
	languages->Insert("mt-MT", "Maltese (Malta)");
	languages->Insert("nb", "Norwegian (Bokm?l)");
	languages->Insert("nb-NO", "Norwegian (Bokm?l) (Norway)");
	languages->Insert("nl", "Dutch");
	languages->Insert("nl-BE", "Dutch (Belgium)");
	languages->Insert("nl-NL", "Dutch (Netherlands)");
	languages->Insert("nn-NO", "Norwegian (Nynorsk) (Norway)");
	languages->Insert("ns", "Northern Sotho");
	languages->Insert("ns-ZA", "Northern Sotho (South Africa)");
	languages->Insert("pa", "Punjabi");
	languages->Insert("pa-IN", "Punjabi (India)");
	languages->Insert("pl", "Polish");
	languages->Insert("pl-PL", "Polish (Poland)");
	languages->Insert("ps", "Pashto");
	languages->Insert("ps-AR", "Pashto (Afghanistan)");
	languages->Insert("pt", "Portuguese");
	languages->Insert("pt-BR", "Portuguese (Brazil)");
	languages->Insert("pt-PT", "Portuguese (Portugal)");
	languages->Insert("qu", "Quechua");
	languages->Insert("qu-BO", "Quechua (Bolivia)");
	languages->Insert("qu-EC", "Quechua (Ecuador)");
	languages->Insert("qu-PE", "Quechua (Peru)");
	languages->Insert("ro", "Romanian");
	languages->Insert("ro-RO", "Romanian (Romania)");
	languages->Insert("ru", "Russian");
	languages->Insert("ru-RU", "Russian (Russia)");
	languages->Insert("sa", "Sanskrit");
	languages->Insert("sa-IN", "Sanskrit (India)");
	languages->Insert("se", "Sami");
	languages->Insert("se-FI", "Sami (Finland)");
	languages->Insert("se-NO", "Sami (Norway)");
	languages->Insert("se-SE", "Sami (Sweden)");
	languages->Insert("sk", "Slovak");
	languages->Insert("sk-SK", "Slovak (Slovakia)");
	languages->Insert("sl", "Slovenian");
	languages->Insert("sl-SI", "Slovenian (Slovenia)");
	languages->Insert("sq", "Albanian");
	languages->Insert("sq-AL", "Albanian (Albania)");
	languages->Insert("sr-BA", "Serbian (Latin) (Bosnia and Herzegovina)");
	languages->Insert("sr-Cyrl-BA", "Serbian (Cyrillic) (Bosnia and Herzegovina)");
	languages->Insert("sr-SP", "Serbian (Latin) (Serbia and Montenegro)");
	languages->Insert("sr-Cyrl-SP", "Serbian (Cyrillic) (Serbia and Montenegro)");
	languages->Insert("sv", "Swedish");
	languages->Insert("sv-FI", "Swedish (Finland)");
	languages->Insert("sv-SE", "Swedish (Sweden)");
	languages->Insert("sw", "Swahili");
	languages->Insert("sw-KE", "Swahili (Kenya)");
	languages->Insert("syr", "Syriac");
	languages->Insert("syr-SY", "Syriac (Syria)");
	languages->Insert("ta", "Tamil");
	languages->Insert("ta-IN", "Tamil (India)");
	languages->Insert("te", "Telugu");
	languages->Insert("te-IN", "Telugu (India)");
	languages->Insert("th", "Thai");
	languages->Insert("th-TH", "Thai (Thailand)");
	languages->Insert("tl", "Tagalog");
	languages->Insert("tl-PH", "Tagalog (Philippines)");
	languages->Insert("tn", "Tswana");
	languages->Insert("tn-ZA", "Tswana (South Africa)");
	languages->Insert("tr", "Turkish");
	languages->Insert("tr-TR", "Turkish (Turkey)");
	languages->Insert("tt", "Tatar");
	languages->Insert("tt-RU", "Tatar (Russia)");
	languages->Insert("ts", "Tsonga");
	languages->Insert("uk", "Ukrainian");
	languages->Insert("uk-UA", "Ukrainian (Ukraine)");
	languages->Insert("ur", "Urdu");
	languages->Insert("ur-PK", "Urdu (Islamic Republic of Pakistan)");
	languages->Insert("uz", "Uzbek (Latin)");
	languages->Insert("uz-UZ", "Uzbek (Latin) (Uzbekistan)");
	languages->Insert("uz-Cyrl-UZ", "Uzbek (Cyrillic) (Uzbekistan)");
	languages->Insert("vi", "Vietnamese");
	languages->Insert("vi-VN", "Vietnamese (Viet Nam)");
	languages->Insert("xh", "Xhosa");
	languages->Insert("xh-ZA", "Xhosa (South Africa)");
	languages->Insert("zh", "Chinese");
	languages->Insert("zh-CN", "Chinese (S)");
	languages->Insert("zh-HK", "Chinese (Hong Kong)");
	languages->Insert("zh-MO", "Chinese (Macau)");
	languages->Insert("zh-SG", "Chinese (Singapore)");
	languages->Insert("zh-TW", "Chinese (T)");
	languages->Insert("zu", "Zulu");
	languages->Insert("zu-ZA", "Zulu (South Africa");
}
