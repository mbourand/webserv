#ifndef POST_METHOD_HPP
#define POST_METHOD_HPP

#include "IMethod.hpp"

class Request;

class PostMethod : public IMethod
{
	public:
		PostMethod();
		PostMethod(const PostMethod& other);
		virtual ~PostMethod();
		PostMethod& operator=(const PostMethod& other);

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
};

#endif
