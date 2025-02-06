/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 19:25:54 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/16 19:32:40 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	idx;
	int		cmp;

	idx = 0;
	cmp = 0;
	while (idx < n && cmp == 0)
	{
		cmp = ((unsigned char *)s1)[idx] - ((unsigned char *)s2)[idx];
		idx++;
	}
	return (cmp);
}
