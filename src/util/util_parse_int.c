/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_parse_int.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:37:48 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/12 16:01:34 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

bool	util_parse_int(const char *str, size_t *parsed_len, int *value_out)
{
	size_t	idx;
	int		value;
	int		sign;
	int		digit;

	idx = 0;
	value = 0;
	sign = 1;
	if (str[idx] == '-')
		sign = -1;
	if (str[idx] == '-' || str[idx] == '+')
		idx++;
	if (!util_isdigit(str[idx]))
		return (false);
	while (util_isdigit(str[idx]))
	{
		digit = str[idx++] - '0';
		if ((sign > 0 && value > (INT_MAX - digit) / 10)
			|| (sign < 0 && value < (INT_MIN + digit) / 10))
			return (false);
		value = value * 10 + sign * digit;
	}
	*value_out = value;
	*parsed_len = idx;
	return (true);
}
