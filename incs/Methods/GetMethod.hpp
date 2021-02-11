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
};

#endif
