/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:32:13 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/17 11:59:25 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	idx;

	if (!*needle)
		return ((char *)haystack);
	while (len && *haystack)
	{
		idx = 0;
		while (idx < len && needle[idx] && haystack[idx] == needle[idx])
			idx++;
		if (!needle[idx])
			return ((char *)haystack);
		haystack++;
		len--;
	}
	return (0);
}
