/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_expand2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:03:56 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/07 21:24:52 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word.h"
#include "word_internal2.h"

#include <stdlib.h>

#include "util.h"

void	word_expand_process_field(struct s_word_field *field, struct s_word_field ***processed_append);
bool	word_expand_process_char(struct s_word_expand_state *state, char c);
bool	word_expand_envvar(struct s_word_expand_scan *scan);
char	word_expand_next_char(struct s_word_expand_state *state);
void	word_expand_output_char(struct s_word_expand_state *state, char c);
void	word_expand_output_split(struct s_word_expand_state *state);

struct s_word_field	*word_expand(char *word)
{
	struct s_word_expand_state	state;
	struct s_word_expand_scan	*scan;
	struct s_word_field			*out_first;
	char						c;

	out_first = NULL;
	state.count.word = word;
	state.count.var = NULL;
	state.count.quote = '\0';
	state.write.word = word;
	state.write.var = NULL;
	state.write.quote = '\0';
	state.is_write_scan = false;
	state.has_filename_wildcard = false;
	state.out_idx = 0;
	state.out_append = &out_first;
	while (1)
	{
		c = word_expand_next_char(&state);
		word_expand_process_char(&state, c);
	}
	word_expand_output_split(&state);
	return (out_first);
}

/*
	Attempt to read a variable name from `scan->word` and store its value in
	`scan->var`. Returns false if no name could be read, true otherwise, even
	if variable is unset or empty.
*/
bool	word_expand_envvar(struct s_word_expand_scan *scan)
{
	char	*varname;
	char	name_end_temp;

	if (!util_isname(*scan->word))
		return (false);
	varname = scan->word;
	while (util_isname(*scan->word))
		scan->word++;
	name_end_temp = *scan->word;
	*scan->word = '\0';
	scan->var = getenv(varname);
	*scan->word = name_end_temp;
	return (true);
}

/*
	Get next character from the word or current expansion. Detects start of a
	new expansion and loops as necessary to skip empty expansions.
*/
char	word_expand_next_char(struct s_word_expand_state *state)
{
	struct s_word_expand_scan	*scan;
	char						word_char;

	scan = &state->count;
	if (state->is_write_scan)
		scan = &state->write;
	while (true)
	{
		if (scan->var && *scan->var)
			return (*scan->var++);
		scan->var = NULL;
		word_char = *scan->word++;
		if (word_char == '$' && word_expand_envvar(scan))
			continue ;
		return (word_char);
	}
}

void	word_expand_output_char(struct s_word_expand_state *state, char c)
{
	if (state->is_write_scan)
		state->out_curr->value[state->out_idx] = c;
	state->out_idx++;
}

void	word_expand_output_split(struct s_word_expand_state *state)
{
	struct s_word_field	*new_field;
	struct s_word_field	**processed_append;

	if (!state->is_write_scan)
	{
		new_field = malloc(sizeof(*new_field) + state->out_idx + 1);
		new_field->next = NULL;
		state->out_curr = new_field;
		state->is_write_scan = true;
	}
	else
	{
		state->out_curr->value[state->out_idx] = '\0';
		word_expand_process_field(state->out_curr, &processed_append);
		*state->out_append = state->out_curr;
		state->out_append = processed_append;
		state->is_write_scan = false;
	}
	state->out_idx = 0;
}
