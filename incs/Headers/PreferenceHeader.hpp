#ifndef PREFERENCE_HEADER_HPP
#define PREFERENCE_HEADER_HPP

#include "Header.hpp"
#include <map>

class PreferenceHeader : public Header
{
	private:

	public:
		PreferenceHeader();
		PreferenceHeader(const PreferenceHeader& other);
		virtual ~PreferenceHeader();
		PreferenceHeader& operator=(const PreferenceHeader& other);

		virtual std::multimap<float, std::string, std::greater<float> > getPreferences() const;
};

#endif
