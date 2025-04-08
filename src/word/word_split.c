/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:28:55 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/08 19:17:03 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"
#include "word.h"

#include <stdlib.h>

#include "util.h"

static void	word_split_output_char(struct s_word_split_state *state, char c)
{
	if (state->out)
		state->out->value[state->out_idx] = c;
	state->out_idx++;
}

static void	word_split_end_field(struct s_word_split_state *state)
{
	struct s_word_field	*field;

	if (state->out_idx == 0)
		return ;
	if (!state->out)
	{
		field = malloc(sizeof(*field) + state->out_idx + 1);
		field->next = NULL;
		*state->out_append = field;
		state->out_append = &field->next;
		state->out_idx = 0;
	}
	else
	{
		state->out->value[state->out_idx] = '\0';
		state->out = state->out->next;
		state->out_idx = 0;
	}
}

static void	word_split_scan(struct s_word_split_state *state)
{
	char	c;
	char	quote;

	quote = 0;
	while (*state->word)
	{
		c = *state->word++;
		if (c == INTERNAL_ESCAPE)
			c = *state->word++;
		else if (c == quote)
			quote = 0;
		else if (!quote && (c == '\'' || c == '"'))
			quote = c;
		else if (!quote && util_isblank(c))
		{
			word_split_end_field(state);
			continue ;
		}
		word_split_output_char(state, c);
	}
	word_split_end_field(state);
}

/*
	Split the word into fields on unquoted blank characters.

	TODO: use IFS instead of blanks
*/
struct s_word_field	*word_split(char *word)
{
	struct s_word_split_state	state;
	struct s_word_field			*out_first;

	out_first = NULL;
	state.word = word;
	state.out_append = &out_first;
	state.out = NULL;
	state.out_idx = 0;
	word_split_scan(&state);
	state.word = word;
	state.out = out_first;
	word_split_scan(&state);
	return (out_first);
}
