/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base64.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 19:32:10 by nforay            #+#    #+#             */
/*   Updated: 2021/04/02 16:58:49 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base64.hpp"

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string	base64_encode(const char *str, size_t len)
{
	std::string		result;
	unsigned char	bytes[3];
	size_t			new_len;
	int 			i = 0;

	if ((new_len = (4 * ((len + 2) / 3))) < len)
		return (result);
	result.reserve(new_len);
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
	switch (i)
	{
	case 1:
		result.push_back(base64_chars[((bytes[0] & 0b11111100) >> 2)]);
		result.push_back(base64_chars[((bytes[0] & 0b11) << 4)]);
		result += "==";
		break;
	case 2:
		result.push_back(base64_chars[((bytes[0] & 0b11111100) >> 2)]);
		result.push_back(base64_chars[((bytes[0] & 0b11) << 4) + ((bytes[1] & 0b11110000) >> 4)]);
		result.push_back(base64_chars[((bytes[1] & 0b1111) << 2)]);
		result += "=";
		break;
	default:
		break;
	}
	return result;
}