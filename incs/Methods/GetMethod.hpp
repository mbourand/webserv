#ifndef GET_METHOD_HPP
#define GET_METHOD_HPP

#include "IMethod.hpp"
#include "ConfigContext.hpp"

class Request;

class GetMethod : public IMethod
{
	public:
		GetMethod();
		GetMethod(const GetMethod& other);
		virtual ~GetMethod();
		GetMethod& operator=(const GetMethod& other);

		std::string getType() const;
		IMethod* clone() const;

		bool allowAbsolutePath() const;
		bool allowCompleteURL() const;
		bool allowAuthorityURI() const;
		bool allowAsteriskURI() const;

		bool requestHasBody() const;
		bool successfulResponseHasBody() const;
		bool isSafe() const;
		bool isIdempotent() const;
		bool isCacheable() const;
		bool isAllowedInHTMLForms() const;

		Response process(const Request& request, const ConfigContext& config);

	private:
		Response directory_listing(const Request& request, const ConfigContext& config);
		std::string get_file_size(const std::string& realPath);
		std::string get_last_modified_format(const std::string& realPath, const std::string& format);
		std::list<std::string> list_directory(const std::string& realPath);
};

#endif
