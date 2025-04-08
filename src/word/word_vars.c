/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_vars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:29:47 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/08 18:28:25 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

#include <stdbool.h>
#include <stdlib.h>

#include "util.h"

static void	word_vars_output_char(struct s_word_vars_state *state, char c)
{
	if (state->out)
		state->out[state->out_idx] = c;
	state->out_idx++;
}

static void	word_vars_output_var(struct s_word_vars_state *state, char *value)
{
	char	c;

	while (*value)
	{
		c = *value++;
		if (c == '\'' || c == '"' || c == INTERNAL_ESCAPE)
			word_vars_output_char(state, INTERNAL_ESCAPE);
		word_vars_output_char(state, c);
	}
}

/*
	TODO: correct value for $?
*/
static bool	word_vars_handle_var(struct s_word_vars_state *state)
{
	char	*varname;
	char	name_end_temp;
	char	*value;

	if (*state->word == '?')
		value = "0";
	else if (util_isname(*state->word))
	{
		varname = state->word;
		while (util_isname(*state->word))
			state->word++;
		name_end_temp = *state->word;
		*state->word = '\0';
		value = getenv(varname);
		*state->word = name_end_temp;
		if (!value)
			value = "";
	}
	else
		return (false);
	word_vars_output_var(state, value);
	return (true);
}

static void	word_vars_scan(struct s_word_vars_state *state)
{
	char	c;
	char	quote;

	quote = 0;
	while (*state->word)
	{
		c = *state->word++;
		if (c == INTERNAL_ESCAPE)
			word_vars_output_char(state, INTERNAL_ESCAPE);
		else if (c == quote)
			quote = 0;
		else if (!quote && c == '\'')
			quote = c;
		else if (!quote && c == '$')
			if (word_vars_handle_var(state))
				continue ;
		word_vars_output_char(state, c);
	}
}

/*
	Expand any variables in the given word. Also escapes any occurrences of
	internal escape character that might be present in the word.

	Works in two passes, first pass counting size to allocate and second pass
	filling the allocated space.
*/
char	*word_vars(char *word)
{
	struct s_word_vars_state	state;

	state.word = word;
	state.out = NULL;
	state.out_idx = 0;
	word_vars_scan(&state);
	state.word = word;
	state.out = malloc(state.out_idx + 1);
	state.out_idx = 0;
	word_vars_scan(&state);
	state.out[state.out_idx] = '\0';
	return (state.out);
}
