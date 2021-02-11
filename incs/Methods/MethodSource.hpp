#ifndef METHOD_SOURCE_HPP
#define METHOD_SOURCE_HPP

#include <set>
#include "IMethod.hpp"
#include "ConnectMethod.hpp"
#include "TraceMethod.hpp"
#include "PutMethod.hpp"
#include "GetMethod.hpp"
#include "PostMethod.hpp"
#include "HeadMethod.hpp"
#include "OptionsMethod.hpp"
#include "DeleteMethod.hpp"

class MethodSource
{
	private:
		std::set<IMethod*> _methods;

	public:
		MethodSource();
		MethodSource(const MethodSource& other);
		virtual ~MethodSource();
		MethodSource& operator=(const MethodSource& other);

		IMethod* createByType(std::string type) const;
		IMethod* getByType(std::string type) const;
		bool contains(std::string type) const;
};

#endif
