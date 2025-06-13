/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 19:03:02 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/30 13:21:09 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

/*
	The dst == source early return can hide errors if two NULL pointers are
	provided - segfaulting here would reveal the caller's mistake.

	Unfortunately our libc memcpy and memmove do something similar, and some
	libft testers check for this and expect no segfault even when non-zero
	length is provided. Passing NULL pointers to memcpy and memmove is undefined
	behaviour, so it could be argued that this doesn't need to be matched, but
	the check is trivial and can be justified as a performance optimization.
*/

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	idx;

	if (dst == src)
		return (dst);
	if (dst > src)
	{
		idx = len;
		while (idx--)
			((unsigned char *)dst)[idx] = ((unsigned char *)src)[idx];
	}
	else
	{
		idx = 0;
		while (idx < len)
		{
			((unsigned char *)dst)[idx] = ((unsigned char *)src)[idx];
			idx++;
		}
	}
	return (dst);
}
