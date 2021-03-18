#ifndef OPTIONS_METHOD_HPP
#define OPTIONS_METHOD_HPP

#include "IMethod.hpp"

class Request;

class OptionsMethod : public IMethod
{
	public:
		OptionsMethod();
		OptionsMethod(const OptionsMethod& other);
		virtual ~OptionsMethod();
		OptionsMethod& operator=(const OptionsMethod& other);

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
