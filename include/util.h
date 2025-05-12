/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:23:26 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/12 15:49:28 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include <stdbool.h>
# include <stddef.h>

bool	util_isblank(char c);
bool	util_isdigit(char c);
bool	util_isname(char c);

bool	util_parse_int(const char *str, size_t *parsed_len, int *value_out);

bool	util_write_all(int fd, const void *buf, size_t len);

size_t	util_itoa_base(int n, const char *base, char *dest);

#endif
