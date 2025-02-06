/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:27:42 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/23 16:15:01 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	char	*sub;

	s_len = ft_strlen(s);
	if (start > s_len)
		s_len = 0;
	else
	{
		s += start;
		s_len -= start;
	}
	if (s_len < len)
		len = s_len;
	sub = malloc(len + 1);
	if (sub)
	{
		ft_memcpy(sub, s, len);
		sub[len] = 0;
	}
	return (sub);
}
