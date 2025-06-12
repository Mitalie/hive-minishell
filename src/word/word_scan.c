/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_scan.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:18:16 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/13 01:11:13 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

#include <stdbool.h>

#include "status.h"
#include "util.h"

static t_status	word_scan_expansion(struct s_word_state *state, bool quoted);

/*
	Scan the word for any special characters that need to be handled.
	Recognizes quotes and maintains quote state, and recognizes expansions
	introduced by `$`.
*/
t_status	word_scan(struct s_word_state *state)
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
	return (word_out_split(state));
}

/*
	Process the expansion following a `$` by scanning the replacement value
	for special characters that need to be handled. Recognizes word splitting
	characters when unquoted.
*/
t_status	word_scan_expansion(struct s_word_state *state, bool quoted)
{
	char		*value;
	char		c;
	t_status	status;

	value = word_exp_parse(&state->word, state->env);
	if (!value)
	{
		word_out_char(state, '$', quoted);
		return (S_OK);
	}
	while (*value)
	{
		c = *value++;
		if (!quoted && util_isblank(c))
		{
			status = word_out_split(state);
			if (status != S_OK)
				return (status);
		}
		else
			word_out_char(state, c, quoted);
	}
	return (S_OK);
}
