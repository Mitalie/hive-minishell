/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_char.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:24:37 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/12 15:38:41 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <stdbool.h>

bool	util_isblank(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (true);
	return (false);
}

bool	util_isdigit(char c)
{
	if (c >= '0' && c <= '9')
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
