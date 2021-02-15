#ifndef HEAD_METHOD_HPP
#define HEAD_METHOD_HPP

#include "IMethod.hpp"

class HeadMethod : public IMethod
{
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
};

#endif
