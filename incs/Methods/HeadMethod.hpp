#ifndef HEAD_METHOD_HPP
#define HEAD_METHOD_HPP

#include "IMethod.hpp"
#include "GetMethod.hpp"

class Request;

class HeadMethod : public IMethod
{
	private:
		GetMethod _getMethod;

	public:
		HeadMethod();
		HeadMethod(const HeadMethod& other);
		virtual ~HeadMethod();
		HeadMethod& operator=(const HeadMethod& other);

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

		Response process(const Request& request, const ConfigContext& config, const ServerSocket& socket);
};

#endif
