#ifndef TRACE_METHOD_HPP
#define TRACE_METHOD_HPP

#include "IMethod.hpp"

class TraceMethod : public IMethod
{
	public:
		TraceMethod();
		TraceMethod(const TraceMethod& other);
		virtual ~TraceMethod();
		TraceMethod& operator=(const TraceMethod& other);

		std::string getType() const;
		IMethod* clone() const;
};

#endif
