#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

int	parse_MIME_file(void)
{
	std::ifstream inputFileStream("/etc/mime.types");
	if (!inputFileStream.is_open())
		return (1);
	while (!inputFileStream.eof()) {
		std::string line;
		std::string formatname;
		std::getline(inputFileStream, line);
		std::stringstream currentline(line);
		currentline >> formatname;
		if (line[0] == '#') //skip commented lines
			continue;
		if (formatname.size() && (formatname.size() != line.size())) // ignore types without file extension
		{
			for (size_t i = 0; i < (std::count(line.begin(), line.end(), ' ') + 1); i++)
			{
				std::string occurrence;
				currentline >> occurrence;
				if (occurrence.size()) //ignore empty strings
					std::cout << "\"" << occurrence << "\" = \"" << formatname << "\"" << std::endl; // hash occurence and store in hashtable to get formatname
			}
		}
	}
	inputFileStream.close();
	return (0);
}
