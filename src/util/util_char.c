/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_char.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:24:37 by amakinen          #+#    #+#             */
/*   Updated: 2025/03/03 21:31:06 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

bool	util_isblank(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (true);
	return (false);
}

bool	util_isname(char c)
{
	if (c == '_'
		|| (c >= '0' && c <= '9')
		|| (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
	)
		return (true);
	return (false);
}
