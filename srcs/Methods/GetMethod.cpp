#include "GetMethod.hpp"
#include <fstream>
#include <sstream>
#include <errno.h>
#include <string.h>
#include "Logger.hpp"
#include "Request.hpp"

GetMethod::GetMethod() {}
GetMethod::GetMethod(const GetMethod&) {}
GetMethod::~GetMethod() {}
GetMethod& GetMethod::operator=(const GetMethod&) { return *this; }

std::string GetMethod::getType() const { return "GET"; }

IMethod* GetMethod::clone() const { return new GetMethod(*this); }

bool GetMethod::allowAbsolutePath() const { return true; }
bool GetMethod::allowCompleteURL() const { return true; }
bool GetMethod::allowAuthorityURI() const { return false; }
bool GetMethod::allowAsteriskURI() const { return false; }

bool GetMethod::requestHasBody() const { return false; }
bool GetMethod::successfulResponseHasBody() const { return true; }
bool GetMethod::isSafe() const { return true; }
bool GetMethod::isIdempotent() const { return true; }
bool GetMethod::isCacheable() const { return true; }
bool GetMethod::isAllowedInHTMLForms() const { return true; }

Response GetMethod::process(const Request& request)
{
	std::fstream file((std::string("www/") + request._path).c_str());
	if (!file.good() || !file.is_open())
	{
		if (errno == ENOENT)
			return Logger::print("File not found", Response(404, "Not found"), ERROR, VERBOSE);
		if (errno == EACCES)
			return Logger::print("Premission denied", Response(403, "Forbidden"), ERROR, VERBOSE);
		return Logger::print("Unexpected error while trying to open file", Response(500, "Internal Server Error"));
	}
	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	Response response(200, "OK");
	std::ostringstream size;
	size << content.size(); //BAAAAAAAD, use lstat to get the size.
	response.addHeader("Content-Length", size.str());
	response.setBody(content);
	return response;
}
