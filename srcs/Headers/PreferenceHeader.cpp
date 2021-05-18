#include "PreferenceHeader.hpp"
#include <list>
#include <string>
#include "Utils.hpp"

PreferenceHeader::PreferenceHeader() {}
PreferenceHeader::~PreferenceHeader() {}
PreferenceHeader::PreferenceHeader(const PreferenceHeader&) {}
PreferenceHeader& PreferenceHeader::operator=(const PreferenceHeader&) { return *this; }

std::multimap<float, std::string, std::greater<float> > PreferenceHeader::getPreferences() const
{
	std::multimap<float, std::string, std::greater<float> > language_preferences;
	std::list<std::string> language_tokens = ft::split(_value, ',');

	if (language_tokens.empty())
		return std::multimap<float, std::string, std::greater<float> >();

	for (std::list<std::string>::const_iterator it = language_tokens.begin(); it != language_tokens.end(); it++)
	{
		std::list<std::string> splitted = ft::split(*it, ';');
		if (splitted.size() == 0 || splitted.size() > 2)
			return std::multimap<float, std::string, std::greater<float> >();

		if (splitted.size() == 1)
		{
			if (splitted.front().find_first_not_of(' ') == std::string::npos)
				return std::multimap<float, std::string, std::greater<float> >();

			language_preferences.insert(std::make_pair(1.f, splitted.front().substr(splitted.front().find_first_not_of(' '))));
		}
		else if (splitted.size() == 2)
		{
			if (splitted.front().find_first_not_of(' ') == std::string::npos || splitted.back().find_first_not_of(' ') == std::string::npos)
				return std::multimap<float, std::string, std::greater<float> >();
			if (splitted.back().substr(0, 2) != "q=")
				return std::multimap<float, std::string, std::greater<float> >();


			float preference = 0.0f;
			try { preference = ft::toFloat(splitted.back().substr(2)); }
			catch (std::exception& e) { return std::multimap<float, std::string, std::greater<float> >(); }

			language_preferences.insert(std::make_pair(preference, splitted.front().substr(splitted.front().find_first_not_of(' '))));
		}
	}

	return language_preferences;
}
