#ifndef POST_METHOD_HPP
#define POST_METHOD_HPP

#include "IMethod.hpp"

class PostMethod : public IMethod
{
	public:
		PostMethod();
		PostMethod(const PostMethod& other);
		virtual ~PostMethod();
		PostMethod& operator=(const PostMethod& other);

		std::string getType() const;
		IMethod* clone() const;

		bool allowAbsolutePath() const;
		bool allowCompleteURL() const;
		bool allowAuthorityURI() const;
		bool allowAsteriskURI() const;
};

#endif
