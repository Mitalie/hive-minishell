/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_scan.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:18:16 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/16 14:33:02 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

#include <stdbool.h>
#include <stdlib.h>

#include "util.h"

static void	word_scan_expansion(struct s_word_state *state, bool quoted);
static char	*word_scan_get_exp(struct s_word_state *state);

/*
	Scan the word for any special characters that need to be handled.
	Recognizes quotes and maintains quote state, and recognizes expansions
	introduced by `$`.
*/
void	word_scan(struct s_word_state *state)
{
	char	quote;
	char	c;

	quote = 0;
	while (*state->word)
	{
		c = *state->word++;
		if (c == quote)
			quote = 0;
		else if ((c == '\'' || c == '"') && !quote)
			quote = c;
		else if (c == '$' && (!quote || quote == '"'))
			word_scan_expansion(state, quote);
		else
			word_out_char(state, c, quote);
		if (quote)
			state->out_has_quotes = true;
	}
	word_out_split(state);
}

/*
	Process the expansion following a `$` by scanning the replacement value
	for special characters that need to be handled. Recognizes word splitting
	characters when unquoted.

	TODO: split using IFS instead of isblank.
*/
void	word_scan_expansion(struct s_word_state *state, bool quoted)
{
	char	*value;
	char	c;

	value = word_scan_get_exp(state);
	if (!value)
	{
		word_out_char(state, '$', quoted);
		return ;
	}
	while (*value)
	{
		c = *value++;
		if (!quoted && util_isblank(c))
			word_out_split(state);
		else
			word_out_char(state, c, quoted);
	}
}

/*
	Identify and consume the expansion following a `$`, and return a pointer
	to its value. If no valid expansion is found, a null pointer is returned.

	TODO: implement correct value for `$?`.
*/
char	*word_scan_get_exp(struct s_word_state *state)
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
