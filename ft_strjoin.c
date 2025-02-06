/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:34:42 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/18 13:24:33 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdint.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*joined;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (len2 + 1 > SIZE_MAX - len1)
		return (0);
	joined = malloc(len1 + len2 + 1);
	if (joined)
	{
		ft_memcpy(joined, s1, len1);
		ft_memcpy(joined + len1, s2, len2);
		joined[len1 + len2] = 0;
	}
	return (joined);
}
