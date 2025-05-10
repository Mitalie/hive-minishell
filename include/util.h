/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:23:26 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/10 22:23:57 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include <stdbool.h>
# include <stddef.h>

bool	util_isblank(char c);
bool	util_isname(char c);

bool	util_write_all(int fd, const void *buf, size_t len);

size_t	util_itoa_base(int n, const char *base, char *dest);

#endif
