#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "Types_parser.hpp"

bool	parse_types_file(HashTable *file_formatname, const char *src)
{
	std::ifstream inputFileStream(src);
	if (!inputFileStream.is_open())
		return (false);
	while (!inputFileStream.eof()) {
		std::string line;
		std::string formatname;
		std::getline(inputFileStream, line);
		std::stringstream currentline(line);
		currentline >> formatname;
		if (formatname[0] == '#')
			continue;
		if (formatname.size() && (formatname.size() != line.size()))
		{
			for (size_t i = 0; i < (std::count(line.begin(), line.end(), ' ') + 1); i++)
			{
				std::string occurrence;
				currentline >> occurrence;
				if (occurrence.size())
					file_formatname->Insert(occurrence, formatname);
			}
		}
	}
	inputFileStream.close();
	return (true);
}
