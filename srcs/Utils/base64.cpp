#include <iostream>
#include <string>

//https://fr.wikipedia.org/wiki/Base64

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string	base64_encode(const char *str, unsigned int len)
{
	std::string		result;
	unsigned char	bytes[3];
	int 			i = 0;

	while (len--)
	{
		bytes[i++] = *(str++);
		if (i == 3)
		{
			result.push_back(base64_chars[((bytes[0] & 0b11111100) >> 2)]);
			result.push_back(base64_chars[((bytes[0] & 0b11) << 4) + ((bytes[1] & 0b11110000) >> 4)]);
			result.push_back(base64_chars[((bytes[1] & 0b1111) << 2) + ((bytes[2] & 0b11000000) >> 6)]);
			result.push_back(base64_chars[((bytes[2] & 0b111111))]);
			i = 0;
		}
	}
	/*switch (i)
	{
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	}*/
	return result;
}

int	main(void)
{
	std::string test("Hi!");
	std::cout << test << " = |" << base64_encode(test.c_str(), test.size()) << "|" << std::endl;
	return (0);
}