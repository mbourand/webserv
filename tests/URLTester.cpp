#include "URL.hpp"
#include <iostream>
#include <vector>
#include <map>

struct URLResult
{
	std::string _scheme, _userinfo, _host, _port, _path, _query, _fragment;

	URLResult(const std::string& scheme, const std::string& userinfo, const std::string& host, const std::string& port, const std::string& path, const std::string& query, const std::string& fragment)
	{
		_scheme = scheme;
		_userinfo = userinfo;
		_host = host;
		_port = port;
		_path = path;
		_query = query;
		_fragment = fragment;
	}

	bool operator==(const URL& other)
	{
		return _scheme == other._scheme && _userinfo == other._userinfo && _host == other._host && _port == other._port && _path == other._path && _query == other._query && _fragment == other._fragment;
	}

	bool operator!=(const URL& other)
	{
		return !operator==(other);
	}
};

void test_url(std::pair<std::string, URLResult> test)
{
	try
	{
		URL url(test.first);
		if (test.second == url)
			std::cout << "OK !" << std::endl;
		else
		{
			std::cout << "Pas bon :" << std::endl;
			std::cout << "  Expected:" << std::endl;
			std::cout << test.second._scheme << std::endl << test.second._userinfo << std::endl << test.second._host << std::endl << test.second._port << std::endl <<
				test.second._path << std::endl << test.second._query << std::endl << test.second._fragment << std::endl;
			std::cout << "  Actual:" << std::endl;
			std::cout << url._scheme << std::endl << url._userinfo << std::endl << url._host << std::endl << url._port << std::endl <<
				url._path << std::endl << url._query << std::endl << url._fragment << std::endl;
			std::cout << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cout << "exception " << e.what() << " avec url : " << test.first << std::endl;
	}
}

int main()
{
	std::map<std::string, URLResult> valid_partial_tests;
	std::map<std::string, URLResult> valid_full_tests;
	std::map<std::string, URLResult> invalid_full_tests;

	valid_partial_tests.insert(std::make_pair("/bonjour/test/index.html", URLResult("", "", "", "80", "/bonjour/test/index.html", "", "")));
	valid_partial_tests.insert(std::make_pair("/bonjour/test/index.html?var1=coucou&var2=bonjour", URLResult("", "", "", "80", "/bonjour/test/index.html", "var1=coucou&var2=bonjour", "")));
	valid_partial_tests.insert(std::make_pair("/bonjour/test/index.html?var1=coucou&var2=bonjour#GrosChien", URLResult("", "", "", "80", "/bonjour/test/index.html", "var1=coucou&var2=bonjour", "GrosChien")));
	valid_partial_tests.insert(std::make_pair("/bonjour/tes:t/ind@@ex.html?var1=coucou&var2=bonjour;var3=eaze:gz//#GrosChien", URLResult("", "", "", "80", "/bonjour/tes:t/ind@@ex.html", "var1=coucou&var2=bonjour;var3=eaze:gz//", "GrosChien")));
	valid_partial_tests.insert(std::make_pair("/", URLResult("", "", "", "80", "/", "", "")));

	valid_full_tests.insert(std::make_pair("http://www.youtube.com/", URLResult("http", "", "www.youtube.com", "80", "/", "", "")));
	valid_full_tests.insert(std::make_pair("http://www.youtube.com:8186/", URLResult("http", "", "www.youtube.com", "8186", "/", "", "")));
	valid_full_tests.insert(std::make_pair("http://www.youtube.com/watch?v=6584658&t=135s", URLResult("http", "", "www.youtube.com", "80", "/watch", "v=6584658&t=135s", "")));
	valid_full_tests.insert(std::make_pair("http://user:password@www.youtube.com:6147878/watch?v=6584658&t=135s#frag", URLResult("http", "user:password", "www.youtube.com", "6147878", "/watch", "v=6584658&t=135s", "frag")));
	valid_full_tests.insert(std::make_pair("http://www.reddit.com", URLResult("http", "", "www.reddit.com", "80", "/", "", "")));
	valid_full_tests.insert(std::make_pair("http://watch?v=6584658&t=135s", URLResult("http", "", "watch?v=6584658&t=135s", "80", "/", "", "")));

	invalid_full_tests.insert(std::make_pair("http://@www.youtube.com:6147878/watch?v=6584658&t=135s#frag", URLResult("", "", "", "", "", "", "")));
	invalid_full_tests.insert(std::make_pair("http:www.youtube.com/watch?v=6584658&t=135s", URLResult("", "", "", "", "", "", "")));
	invalid_full_tests.insert(std::make_pair("http:/www.youtube.com/watch?v=6584658&t=135s", URLResult("", "", "", "", "", "", "")));
	invalid_full_tests.insert(std::make_pair("http//www.youtube.com/watch?v=6584658&t=135s", URLResult("", "", "", "", "", "", "")));
	invalid_full_tests.insert(std::make_pair("httpwww.youtube.com/watch?v=6584658&t=135s", URLResult("", "", "", "", "", "", "")));
	invalid_full_tests.insert(std::make_pair("", URLResult("", "", "", "", "", "", "")));
	invalid_full_tests.insert(std::make_pair("http://@:/?#", URLResult("", "", "", "", "", "", "")));
	invalid_full_tests.insert(std::make_pair("http://:/?#", URLResult("", "", "", "", "", "", "")));
	invalid_full_tests.insert(std::make_pair("http:///?#", URLResult("", "", "", "", "", "", "")));

	std::cout << "Partial tests : " << std::endl;
	for (auto& test : valid_partial_tests)
		test_url(test);
	std::cout << std::endl;
	std::cout << "Full tests :" << std::endl;
	for (auto& test : valid_full_tests)
		test_url(test);
	std::cout << std::endl;
	std::cout << "Full invalid tests :" << std::endl;
	for (auto& test : invalid_full_tests)
		test_url(test);
}
