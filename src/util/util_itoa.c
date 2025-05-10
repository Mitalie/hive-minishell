/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_itoa.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 21:00:00 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/10 21:00:00 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "libft.h"

/*
	Convert an integer to a string in the given base and append it to dest
	Returns the number of characters written
*/
size_t	util_itoa_base(int n, const char *base, char *dest)
{
	size_t	base_len;
	size_t	len;
	long	num;

	base_len = ft_strlen(base);
	if (base_len < 2)
		return (0);
	len = 0;
	num = n;
	if (num < 0)
	{
		*dest++ = '-';
		num = -num;
		len++;
	}
	if (num >= (long)base_len)
		len += util_itoa_base(num / base_len, base, dest);
	dest[len] = base[num % base_len];
	dest[len + 1] = '\0';
	return (len + 1);
}
