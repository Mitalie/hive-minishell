/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_unquote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:11:34 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/08 18:51:35 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

/*
	Remove quotes from a string. Characters are moved over the empty space left
	by the removed characters. Quote characters that are quoted (e.g. double
	quote character inside single quotes) are not removed.
*/
void	word_unquote(char *str)
{
	char	*unquoted;
	char	quote;
	char	c;

	unquoted = str;
	quote = 0;
	while (*str)
	{
		c = *str++;
		if (c == INTERNAL_ESCAPE)
			*unquoted++ = *str++;
		else if (c == quote)
			quote = 0;
		else if (!quote && (c == '\'' || c == '"'))
			quote = c;
		else
			*unquoted++ = c;
	}
	*unquoted = '\0';
}
