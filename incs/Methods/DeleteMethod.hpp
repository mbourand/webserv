#ifndef DELETE_METHOD_HPP
#define DELETE_METHOD_HPP

#include "IMethod.hpp"

class DeleteMethod : public IMethod
{
	public:
		DeleteMethod();
		DeleteMethod(const DeleteMethod& other);
		virtual ~DeleteMethod();
		DeleteMethod& operator=(const DeleteMethod& other);

		std::string getType() const;
		IMethod* clone() const;
};

#endif
