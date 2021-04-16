/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Compress.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 18:14:13 by nforay            #+#    #+#             */
/*   Updated: 2021/04/16 17:16:44 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Compress.hpp"
#include "Utils.hpp"

std::string compress_gzip(const std::string &str, int compressionlevel)
{
	z_stream zs;
	ft::memset(&zs, 0, sizeof(zs));
	if (deflateInit2(&zs, compressionlevel, Z_DEFLATED, 31, 9, Z_DEFAULT_STRATEGY) != Z_OK)
		throw(std::runtime_error("deflateInit2() failed."));

	zs.next_in = (Bytef*)str.data();
	zs.avail_in = str.size();
	int ret;
	char outbuffer[32768];
	std::string outstring;

	do {
		zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
		zs.avail_out = sizeof(outbuffer);
		ret = deflate(&zs, Z_FINISH);
		if (outstring.size() < zs.total_out) {
			outstring.append(outbuffer,
			zs.total_out - outstring.size());
		}
	} while (ret == Z_OK);
	deflateEnd(&zs);

	if (ret != Z_STREAM_END)
		throw(std::runtime_error("Exception during zlib compression."));
	return (outstring);
}

std::string compress_deflate(const std::string &str, int compressionlevel)
{
	z_stream zs;
	ft::memset(&zs, 0, sizeof(zs));
	if (deflateInit(&zs, compressionlevel) != Z_OK)
		throw(std::runtime_error("deflateInit() failed."));

	zs.next_in = (Bytef*)str.data();
	zs.avail_in = str.size();
	int ret;
	char outbuffer[32768];
	std::string outstring;

	do {
		zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
		zs.avail_out = sizeof(outbuffer);
		ret = deflate(&zs, Z_FINISH);
		if (outstring.size() < zs.total_out) {
			outstring.append(outbuffer,
			zs.total_out - outstring.size());
		}
	} while (ret == Z_OK);
	deflateEnd(&zs);

	if (ret != Z_STREAM_END)
		throw(std::runtime_error("Exception during zlib compression."));
	return (outstring);
}
