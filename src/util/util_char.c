/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_char.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:24:37 by amakinen          #+#    #+#             */
/*   Updated: 2025/03/03 21:27:50 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

bool	util_isblank(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (true);
	return (false);
}
