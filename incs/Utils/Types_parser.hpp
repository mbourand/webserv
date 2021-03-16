/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types_parser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 16:23:29 by nforay            #+#    #+#             */
/*   Updated: 2021/03/16 16:25:42 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_PARSER_HPP
# define TYPES_PARSER_HPP

# include "HashTable.hpp"

bool	parse_types_file(HashTable *file_formatname, const char *src);

#endif /* ************************************************** TYPES_PARSER_HPP */