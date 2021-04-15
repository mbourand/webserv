/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Compress.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 18:13:17 by nforay            #+#    #+#             */
/*   Updated: 2021/04/15 18:41:52 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPRESS_HPP
# define COMPRESS_HPP

# include <string>
# include <iostream>
# include <zlib.h>

std::string compress_gzip(const std::string& , int compressionlevel = Z_BEST_COMPRESSION);
std::string compress_inflate(const std::string& , int compressionlevel = Z_BEST_COMPRESSION);

#endif /* ****************************************************** COMPRESS_HPP */