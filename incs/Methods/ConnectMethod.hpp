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
};

#endif
