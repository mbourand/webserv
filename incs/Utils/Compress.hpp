/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Compress.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 18:13:17 by nforay            #+#    #+#             */
/*   Updated: 2021/04/18 17:31:00 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPRESS_HPP
# define COMPRESS_HPP

# include <string>
# include <iostream>
# include <zlib.h>

std::string compress_gzip(const std::string& , unsigned int compressionlevel = Z_BEST_COMPRESSION);
std::string compress_deflate(const std::string& , unsigned int compressionlevel = Z_BEST_COMPRESSION);

#endif /* ****************************************************** COMPRESS_HPP */