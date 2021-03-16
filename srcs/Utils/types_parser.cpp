#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "HashTable.hpp"

int	parse_MIME_file(void)
{
	HashTable file_formatname = HashTable(256);
	std::ifstream inputFileStream("/etc/mime.types");
	if (!inputFileStream.is_open())
		return (1);
	while (!inputFileStream.eof()) {
		std::string line;
		std::string formatname;
		std::getline(inputFileStream, line);
		std::stringstream currentline(line);
		currentline >> formatname;
		if (formatname[0] == '#') //skip commented lines
			continue;
		if (formatname.size() && (formatname.size() != line.size())) // ignore types without file extension
		{
			for (size_t i = 0; i < (std::count(line.begin(), line.end(), ' ') + 1); i++)
			{
				std::string occurrence;
				currentline >> occurrence;
				if (occurrence.size()) //ignore empty strings
					file_formatname.Insert(occurrence, formatname);
			}
		}
	}
	file_formatname.Print();
	inputFileStream.close();
	try
	{
		std::cout << "html=" << file_formatname.Lookup("html") << std::endl;
	}
	catch(const std::exception &e)
	{}
	return (0);
}
