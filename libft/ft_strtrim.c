/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:44:52 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/17 15:02:49 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

#define LOOKUP_SIZE 256

char	*ft_strtrim(char const *s1, char const *set)
{
	char	lookup[LOOKUP_SIZE];
	size_t	i;
	char	*trimmed;

	i = 0;
	while (i < LOOKUP_SIZE)
		lookup[i++] = 0;
	while (*set)
		lookup[(unsigned char)*set++] = 1;
	while (lookup[(unsigned char)*s1])
		s1++;
	i = ft_strlen(s1);
	while (i && lookup[(unsigned char)s1[i - 1]])
		i--;
	trimmed = malloc(i + 1);
	if (trimmed)
	{
		ft_memcpy(trimmed, s1, i);
		trimmed[i] = 0;
	}
	return (trimmed);
}
