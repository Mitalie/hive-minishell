/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 10:29:53 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/17 11:31:46 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	idx;

	idx = 0;
	while (dst[idx] && idx < dstsize)
		idx++;
	while (*src && idx + 1 < dstsize)
		dst[idx++] = *src++;
	if (idx < dstsize)
		dst[idx] = 0;
	while (*src++)
		idx++;
	return (idx);
}
