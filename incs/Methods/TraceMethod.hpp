#ifndef TRACE_METHOD_HPP
#define TRACE_METHOD_HPP

#include "IMethod.hpp"

class Request;

class TraceMethod : public IMethod
{
	public:
		TraceMethod();
		TraceMethod(const TraceMethod& other);
		virtual ~TraceMethod();
		TraceMethod& operator=(const TraceMethod& other);

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

		Response process(const Request& request);
};

#endif
