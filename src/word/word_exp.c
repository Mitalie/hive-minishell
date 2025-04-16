/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_exp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:12:48 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/16 15:08:06 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

#include <stdlib.h>

#include "util.h"

/*
	Identify and consume the expansion following a `$`, and return a pointer
	to its value. If no valid expansion is found, a null pointer is returned.

	TODO: implement correct value for `$?`.
*/
char	*word_exp_parse(struct s_word_state *state)
{
	char	*name_start;
	char	*value;
	char	name_end_tmp;

	if (*state->word == '?')
	{
		state->word++;
		value = "0";
	}
	else if (util_isname(*state->word))
	{
		name_start = state->word;
		while (util_isname(*state->word))
			state->word++;
		name_end_tmp = *state->word;
		*state->word = '\0';
		value = getenv(name_start);
		*state->word = name_end_tmp;
		if (!value)
			value = "";
	}
	else
		return (NULL);
	return (value);
}
