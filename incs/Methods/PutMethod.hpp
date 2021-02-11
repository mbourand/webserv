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
};

#endif
