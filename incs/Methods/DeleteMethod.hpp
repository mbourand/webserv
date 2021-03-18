#ifndef DELETE_METHOD_HPP
#define DELETE_METHOD_HPP

#include "IMethod.hpp"

class Request;

class DeleteMethod : public IMethod
{
	public:
		DeleteMethod();
		DeleteMethod(const DeleteMethod& other);
		virtual ~DeleteMethod();
		DeleteMethod& operator=(const DeleteMethod& other);

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
