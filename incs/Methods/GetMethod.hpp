#ifndef GET_METHOD_HPP
#define GET_METHOD_HPP

#include "IMethod.hpp"

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
};

#endif
