#ifndef PUT_METHOD_HPP
#define PUT_METHOD_HPP

#include "IMethod.hpp"

class PutMethod : public IMethod
{
	public:
		PutMethod();
		PutMethod(const PutMethod& other);
		virtual ~PutMethod();
		PutMethod& operator=(const PutMethod& other);

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