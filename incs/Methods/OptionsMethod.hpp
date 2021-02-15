#ifndef OPTIONS_METHOD_HPP
#define OPTIONS_METHOD_HPP

#include "IMethod.hpp"

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
};

#endif
