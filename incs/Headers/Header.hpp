#ifndef IHEADER_HPP
#define IHEADER_HPP

#include <string>

class Header
{
	protected:
		std::string _value;

	public:
		virtual ~Header() {};

		virtual std::string getType() const = 0;
		virtual Header* clone() const = 0;
		virtual size_t parse(std::string value);

		void setValue(const std::string &value);
		const std::string& getValue() const;
};

#endif
