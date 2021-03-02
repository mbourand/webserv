#include "GetMethod.hpp"
#include <fstream>
#include <errno.h>
#include <string.h>

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
	if (!file.is_open())
	{
		if (errno == ENOENT)
			return Logger::print("File not found", Response(404, "Not found"), ERROR, VERBOSE);
		if (errno == EACCES)
			return Logger::print("Premission denied", Response(403, "Forbidden"), ERROR, VERBOSE);
		return Logger::print("Unexpected error while trying to open file", Response(500, "Internal Server Error"));
	}
	std::string content;
	char buff[1024];
	bzero(buff, sizeof(buff));
	while (file.read(buff, sizeof(buff) - 1))
	{
		content += buff;
		bzero(buff, sizeof(buff));
	}
	content += buff;
	Response response(200, "OK");
	response.setBody(content);
	return response;
}
