#include "Test.hpp"
#include "TestCategory.hpp"
#include <iostream>
#include "ClientSocket.hpp"

int main()
{

	TestCategory cat_bad_req("Bad Requests", 0);
	cat_bad_req.addTest<StringStartsWithTest>("Request Entity Too Large", []()
	{
		std::string response;
		ClientSocket socket("localhost", 8080);
		socket << "POST / HTTP/1.1\r\nHost: localhost\r\n\r\nzuyeghiuzehgiuzyehgiszeuhgeziughzeiughziuhfezziuhezhiuerzgiuhhiugzehiu\r\n\r\n";
		socket >> response;
		return response;
	},
	[]() { return std::string("HTTP/1.1 413 Request Entity Too Large"); });


	cat_bad_req.addTest<StringStartsWithTest>("Method Not Allowed", []()
	{
		std::string response;
		ClientSocket socket("localhost", 8080);
		socket << "PUT / HTTP/1.1\r\nHost: localhost\r\n\r\nAAA\r\n\r\n";
		socket >> response;
		return response;
	},
	[]() { return std::string("HTTP/1.1 405 Method Not Allowed"); });



	cat_bad_req.addTest<StringStartsWithTest>("Bad Request", []()
	{
		std::string response;
		ClientSocket socket("localhost", 8080);
		socket << "GZEGEZRGEZ / HTTP/1.1\r\nHost: localhost\r\n\r\n";
		socket >> response;
		return response;
	},
	[]() { return std::string("HTTP/1.1 400 Bad Request"); });



	cat_bad_req.run();




	TestCategory category("Basic tests", 0);
	category.addTest<NoThrowTest>("Basic get request", []()
	{
		std::string response;
		ClientSocket socket("localhost", 8080);
		socket << "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
		socket >> response;
	});

	category.addTest<NoThrowTest>("Accept-Encoding request", []()
	{
		std::string response;
		ClientSocket socket("localhost", 8080);
		socket << "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
		socket >> response;
	});

	category.run();
}
