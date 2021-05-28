#include "Test.hpp"
#include "TestCategory.hpp"
#include <iostream>
#include "ClientSocket.hpp"

std::string send_request(const std::string& request, int port)
{
	std::string response;
	ClientSocket socket("localhost", port);
	socket << request;
	socket >> response;
	return response;
}



TestCategory test_request_head_line_parsing()
{
	TestCategory cat("Request head line parsing", 0);


	cat.addTest<StringStartsWithTest>("Method Not Allowed",
		[]() { return send_request("PUT / HTTP/1.1\r\nHost: localhost\r\n\r\nAAA\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 405 Method Not Allowed"); });


	cat.addTest<StringStartsWithTest>("Bad Method Name",
		[]() { return send_request("GZEGEZRGEZ / HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 400 Bad Request"); });


	cat.addTest<StringStartsWithTest>("Method case sensitive",
		[]() { return send_request("get / HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 400 Bad Request"); });


	cat.addTest<StringStartsWithTest>("Bad space 1",
		[]() { return send_request("get  / HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 400 Bad Request"); });


	cat.addTest<StringStartsWithTest>("Bad space 2",
		[]() { return send_request("GET /  HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 400 Bad Request"); });


	cat.addTest<StringStartsWithTest>("Bad protocol",
		[]() { return send_request("GET / HTTP/1.0\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 400 Bad Request"); });

	cat.addTest<StringStartsWithTest>("URI Without /",
		[]() { return send_request("GET sources/bonsoir/ HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 400 Bad Request"); });

	cat.addTest<StringStartsWithTest>("Invalid URI with .. /",
		[]() { return send_request("GET /sources/bonsoir/../../../../../../../../../../etc/nginx/nginx.conf HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 400 Bad Request"); });

	cat.addTest<StringStartsWithTest>("Valid URI with .. /",
		[]() { return send_request("GET /includes/Socket/../Socket/ClientSocket.hpp HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	return cat;
}



TestCategory test_request_headers_parsing()
{
	TestCategory cat("Request header line parsing", 0);

	cat.addTest<StringStartsWithTest>("2 Hosts",
		[]() { return send_request("GET / HTTP/1.1\r\nHost: localhost\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 400 Bad Request"); });

	cat.addTest<StringStartsWithTest>("No Host",
		[]() { return send_request("GET / HTTP/1.1\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 400 Bad Request"); });

	cat.addTest<StringStartsWithTest>("Bad space",
		[]() { return send_request("GET / HTTP/1.1\r\nHost : localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 400 Bad Request"); });

	cat.addTest<StringStartsWithTest>("Multiple spaces",
		[]() { return send_request("GET / HTTP/1.1\r\nHost:   localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Obsolete line folding",
		[]() { return send_request("GET / HTTP/1.1\r\nHost: local\r\n   host\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Bad Obsolete line folding",
		[]() { return send_request("GET / HTTP/1.1\r\nHost: local\r\nhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 400 Bad Request"); });

	cat.addTest<StringStartsWithTest>("Undefined header",
		[]() { return send_request("GET / HTTP/1.1\r\nHost: localhost\r\nziuegheuzyez: OUI\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Custom header",
		[]() { return send_request("GET / HTTP/1.1\r\nHost: localhost\r\nX-ziuegheuzyez: OUI\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Multiple non-host header",
		[]() { return send_request("GET / HTTP/1.1\r\nHost: localhost\r\nBonjour: OUI\r\nBonjour: Oui\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Case insensitive headers",
		[]() { return send_request("GET / HTTP/1.1\r\nhOsT: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Valid line folding 1",
		[]() { return send_request("GET / HTTP/1.1\r\nHost: l\r\n o\r\n c\r\n      alh\r\n	   o\r\n	   s\r\n	   t\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Valid line folding 1",
		[]() { return send_request("GET / HTTP/1.1\r\nHost: l\r\n 	o\r\n 			c\r\n      alh\r\n	 	  o\r\n		   s\r\n					  	 t\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Valid line folding 1",
		[]() { return send_request("GET / HTTP/1.1\r\nHost: loc\r\n \r\n  \r\n   \r\n	\r\n \r\n     alhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Invalid line folding",
		[]() { return send_request("GET / HTTP/1.1\r\nHost:\r\n \r\n \r\n loc\r\n \r\n  \r\n   \r\n	\r\n \r\n     alhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 400 Bad Request"); });

	cat.addTest<StringStartsWithTest>("Preference Headers",
		[]() { return send_request("GET / HTTP/1.1\r\nHost: localhost\r\nAccept-Language: fr, da, en-CA;q=0.9 *;q=0.8\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	return cat;
}


TestCategory test_request_body_parsing()
{
	TestCategory cat("Body Parsing", 0);


	cat.addTest<StringStartsWithTest>("Request Entity Too Large",
		[]() { return send_request("POST / HTTP/1.1\r\nHost: localhost\r\n\r\nzuyeghiuzehgiuzyehgiszeuhgeziughzeiughziuhfezziuhezhiuerzgiuhhiugzehiu\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 413 Request Entity Too Large"); });

	return cat;
}



TestCategory test_get_requests()
{
	TestCategory cat("GET Requests", 0);

	cat.addTest<StringStartsWithTest>("Path uri",
		[]() { return send_request("GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Full uri",
		[]() { return send_request("GET http://localhost/index.html HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Inexistant file",
		[]() { return send_request("GET /zdehjfgrdesuzygzeuyfgeyjg HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 404 Not Found"); });

	cat.addTest<StringStartsWithTest>("Directory",
		[]() { return send_request("GET /dir HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 301 Moved Permanently"); });

	cat.addTest<StringStartsWithTest>("Auto Index",
		[]() { return send_request("GET /sources/bonsoir HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Directory extra /",
		[]() { return send_request("GET /dir/ HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 403 Forbidden"); });

	cat.addTest<StringStartsWithTest>("Auto Index extra /",
		[]() { return send_request("GET /sources/bonsoir/ HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Index",
		[]() { return send_request("GET / HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	return cat;
}



TestCategory test_post_requests()
{
	TestCategory cat("POST Requests", 0);
	return cat;
}

TestCategory test_put_requests()
{
	TestCategory cat("PUT Requests", 0);
	return cat;
}

TestCategory test_options_requests()
{
	TestCategory cat("OPTIONS Requests", 0);

	cat.addTest<StringContainsTest>("URI *",
	[]() { return send_request("OPTIONS * HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
	[]() { return std::string("CONNECT, DELETE, GET, HEAD, OPTIONS, POST, PUT, TRACE"); });

	cat.addTest<StringContainsTest>("URI set",
	[]() { return send_request("OPTIONS / HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
	[]() { return std::string("CONNECT, DELETE, GET, HEAD, OPTIONS, POST, PUT, TRACE"); });

	cat.addTest<StringContainsTest>("Not every method allowed",
	[]() { return send_request("OPTIONS /noget/ HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
	[]() { return std::string("CONNECT, DELETE, HEAD, OPTIONS, POST, PUT, TRACE"); });

	return cat;
}

TestCategory test_connect_requests()
{
	TestCategory cat("CONNECT Requests", 0);
	return cat;
}

TestCategory test_head_requests()
{
	TestCategory cat("HEAD Requests", 0);

	cat.addTest<StringStartsWithTest>("Path uri",
		[]() { return send_request("HEAD /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Full uri",
		[]() { return send_request("HEAD http://localhost/index.html HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Inexistant file",
		[]() { return send_request("HEAD /zdehjfgrdesuzygzeuyfgeyjg HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 404 Not Found"); });

	cat.addTest<StringStartsWithTest>("Directory",
		[]() { return send_request("HEAD /dir HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 301 Moved Permanently"); });

	cat.addTest<StringStartsWithTest>("Auto Index",
		[]() { return send_request("HEAD /sources/bonsoir HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Directory extra /",
		[]() { return send_request("HEAD /dir/ HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 403 Forbidden"); });

	cat.addTest<StringStartsWithTest>("Auto Index extra /",
		[]() { return send_request("HEAD /sources/bonsoir/ HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	cat.addTest<StringStartsWithTest>("Index",
		[]() { return send_request("HEAD / HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 200 OK"); });

	return cat;
}

TestCategory test_delete_requests()
{
	TestCategory cat("DELETE Requests", 0);

	cat.addTest<StringStartsWithTest>("Valid delete",
		[]() { return send_request("DELETE /delete/test.txt HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 204 No Content"); });

	cat.addTest<StringStartsWithTest>("Inexistant file",
		[]() { return send_request("DELETE /delete/ikzehgfiuezhgiuze.txt HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 404 Not Found"); });

	cat.addTest<StringStartsWithTest>("Delete directory",
		[]() { return send_request("DELETE /delete/dir HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("HTTP/1.1 404 Not Found"); });


	return cat;
}

TestCategory test_trace_requests()
{
	TestCategory cat("TRACE Requests", 0);

	cat.addTest<StringContainsTest>("Basic no body",
		[]() { return send_request("TRACE /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n", 8080); },
		[]() { return std::string("TRACE /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n"); });

	cat.addTest<StringContainsTest>("Basic Body",
		[]() { return send_request("TRACE /index.html HTTP/1.1\r\nHost: localhost\r\n\r\noijuzerjgiouz jgoize jioezsjioez joiez eioz jezoi jezio ezioez ioezoeizeozi", 8080); },
		[]() { return std::string("TRACE /index.html HTTP/1.1\r\nHost: localhost\r\n\r\noijuzerjgiouz jgoize jioezsjioez joiez eioz jezoi jezio ezioez ioezoeizeozi"); });
	return cat;
}

int main()
{
	TestCategory requests("Request Parsing", 0);
	requests.addSubcategory(test_request_head_line_parsing());
	requests.addSubcategory(test_request_headers_parsing());
	requests.addSubcategory(test_request_body_parsing());

	requests.run();


	TestCategory category("Methods", 0);
	category.addSubcategory(test_get_requests());
	category.addSubcategory(test_post_requests());
	category.addSubcategory(test_put_requests());
	category.addSubcategory(test_delete_requests());
	category.addSubcategory(test_connect_requests());
	category.addSubcategory(test_trace_requests());
	category.addSubcategory(test_head_requests());

	category.run();
}

