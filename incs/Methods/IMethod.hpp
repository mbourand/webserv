#ifndef METHOD_HPP
#define METHOD_HPP

#include <string>

class IMethod
{
	public:
		virtual ~IMethod() {};
		virtual std::string getType() const = 0;
		virtual IMethod* clone() const = 0;
};

#endif
