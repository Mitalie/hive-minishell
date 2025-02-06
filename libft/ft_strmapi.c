/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:26:27 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/23 16:56:58 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*mapped;
	size_t	len;
	size_t	idx;

	len = ft_strlen(s);
	mapped = malloc(len + 1);
	if (!mapped)
		return (0);
	idx = 0;
	while (idx < len)
	{
		mapped[idx] = (*f)(idx, s[idx]);
		idx++;
	}
	mapped[len] = 0;
	return (mapped);
}
