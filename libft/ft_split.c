/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:10:43 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/24 10:41:00 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	count_pieces(char const *s, char sep)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		if (*s != sep && !(s[1] && s[1] != sep))
			count++;
		s++;
	}
	return (count);
}

static	char	*dup_piece(char const *s, char sep, char **dup)
{
	char	*end;

	end = ft_strchr(s, sep);
	if (end)
	{
		*dup = ft_substr(s, 0, end - s);
		return (end + 1);
	}
	else
	{
		*dup = ft_strdup(s);
		return (0);
	}
}

char	**ft_split(char const *s, char c)
{
	size_t		arrsize;
	size_t		arridx;
	char		**arr;

	arrsize = count_pieces(s, c);
	arr = malloc((arrsize + 1) * sizeof(*arr));
	if (!arr)
		return (0);
	arr[arrsize] = 0;
	arridx = 0;
	while (arridx < arrsize)
	{
		while (*s == c)
			s++;
		s = dup_piece(s, c, &arr[arridx]);
		if (!arr[arridx++])
		{
			while (arridx)
				free(arr[--arridx]);
			free(arr);
			return (0);
		}
	}
	return (arr);
}
