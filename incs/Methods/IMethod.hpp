#ifndef METHOD_HPP
#define METHOD_HPP

#include <string>

class IMethod
{
	public:
		virtual ~IMethod() {};
		virtual std::string getType() const = 0;
		virtual IMethod* clone() const = 0;
		virtual bool allowAbsolutePath() const = 0;
		virtual bool allowCompleteURL() const = 0;
		virtual bool allowAuthorityURI() const = 0;
		virtual bool allowAsteriskURI() const = 0;

		// From https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods
		virtual bool requestHasBody() const = 0;
		virtual bool successfulResponseHasBody() const = 0;
		virtual bool isSafe() const = 0;
		virtual bool isIdempotent() const = 0;
		virtual bool isCacheable() const = 0;
		virtual bool isAllowedInHTMLForms() const = 0;
};

#endif
