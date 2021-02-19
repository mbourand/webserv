#ifndef CONNECT_METHOD_HPP
#define CONNECT_METHOD_HPP

#include "IMethod.hpp"

class ConnectMethod : public IMethod
{
	public:
		ConnectMethod();
		ConnectMethod(const ConnectMethod& other);
		virtual ~ConnectMethod();
		ConnectMethod& operator=(const ConnectMethod& other);

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
