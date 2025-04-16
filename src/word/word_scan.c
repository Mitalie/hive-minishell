/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_scan.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:18:16 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/16 15:08:06 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

#include <stdbool.h>

#include "util.h"

static void	word_scan_expansion(struct s_word_state *state, bool quoted);

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

	value = word_exp_parse(state);
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
