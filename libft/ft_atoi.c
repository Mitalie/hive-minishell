/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:59:27 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/30 13:20:31 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static int	ft_isspace(int c)
{
	if (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ')
		return (1);
	return (0);
}

/*
	If the string describes a number which cannot be represented in an int, atoi
	invokes undefined behaviour. Our libc atoi simply returns whatever strtol
	returns, converted to int. This includes clamping the values to LONG_MIN and
	LONG_MAX, which become 0 and -1 respectively when converted from 64-bit long
	to 32-bit int.

	Requiring specific garbage values far beyond the well-defined range is not
	very useful, but strict interpretation of the libft assignment requires
	exactly matching libc functions, even undefined behaviours. The tests
	included in francinette don't seem to check this, but implement it anyway in
	case an evaluator decides to be strict about it. It also avoids invoking
	undefined behaviour with signed integer overflow, although the same check
	could be done directly with int while satisfying atoi requirements.
*/

int	ft_atoi(const char *str)
{
	char	sign;
	int		digit_value;
	long	value;

	while (ft_isspace(*str))
		str++;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	value = 0;
	while (ft_isdigit(*str))
	{
		digit_value = *str - '0';
		if (sign > 0 && (LONG_MAX - digit_value) / 10 < value)
			return ((int)LONG_MAX);
		if (sign < 0 && (LONG_MIN + digit_value) / 10 > value)
			return ((int)LONG_MIN);
		value = 10 * value + sign * digit_value;
		str++;
	}
	return (value);
}
