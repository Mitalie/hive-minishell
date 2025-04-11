/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_unescape.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:41:42 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/11 16:14:10 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

/*
	Remove escapes that may have been inserted into the word during processing.
*/
void	word_unescape(char *escaped)
{
	char	*unescaped;

	unescaped = escaped;
	while (*escaped)
	{
		if (*escaped == INTERNAL_ESCAPE)
			escaped++;
		*unescaped++ = *escaped++;
	}
	*unescaped = '\0';
}
