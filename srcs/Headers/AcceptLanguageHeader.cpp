#include "AcceptLanguageHeader.hpp"
#include <map>
#include <vector>
#include <functional>
#include "Utils.hpp"
#include <iostream>

AcceptLanguageHeader::AcceptLanguageHeader() {}
AcceptLanguageHeader::AcceptLanguageHeader(const AcceptLanguageHeader&) {}
AcceptLanguageHeader::~AcceptLanguageHeader() {}
AcceptLanguageHeader& AcceptLanguageHeader::operator=(const AcceptLanguageHeader&) { return *this; }

std::string AcceptLanguageHeader::getType() const { return "Accept-Language"; }

Header* AcceptLanguageHeader::clone() const { return new AcceptLanguageHeader(); }



std::multimap<float, std::string, std::greater<float> > AcceptLanguageHeader::getLanguages() const
{
	std::multimap<float, std::string, std::greater<float> > language_preferences;
	std::list<std::string> language_tokens = ft::split(_value, ",");

	if (language_tokens.empty())
		return std::multimap<float, std::string, std::greater<float> >();

	std::list<std::string> same_preference;
	for (std::list<std::string>::const_iterator it = language_tokens.begin(); it != language_tokens.end(); it++)
	{
		std::list<std::string> splitted = ft::split(*it, ";");
		if (splitted.size() == 0 || splitted.size() > 2)
			return std::multimap<float, std::string, std::greater<float> >();

		if (splitted.size() >= 1)
		{
			if (splitted.front().find_first_not_of(" ") == std::string::npos)
				return std::multimap<float, std::string, std::greater<float> >();

			same_preference.push_back(splitted.front().substr(splitted.front().find_first_not_of(" ")));
		}
		if (splitted.size() == 2)
		{
			if (splitted.front().find_first_not_of(" ") == std::string::npos || splitted.back().find_first_not_of(" ") == std::string::npos)
				return std::multimap<float, std::string, std::greater<float> >();
			if (splitted.back().substr(0, 2) != "q=")
				return std::multimap<float, std::string, std::greater<float> >();


			float preference = 0.0f;
			try { preference = ft::toFloat(splitted.back().substr(2)); }
			catch (std::exception& e) { return std::multimap<float, std::string, std::greater<float> >(); }

			for (std::list<std::string>::const_iterator lang_it = same_preference.begin(); lang_it != same_preference.end(); lang_it++)
				language_preferences.insert(std::make_pair(preference, *lang_it));

			same_preference.clear();
		}
	}

	return language_preferences;
}
