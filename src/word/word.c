/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:54:57 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/11 17:37:50 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word.h"
#include "word_internal.h"

#include <stdbool.h>
#include <stdlib.h>

/*
	Perform all word expansions on the given word and return the resulting field
	list.
*/
struct s_word_field	*word_expand(char *word)
{
	struct s_word_state	state;
	struct s_word_field	*first;

	first = NULL;
	state.word = word;
	state.out = NULL;
	state.out_append = &first;
	state.out_idx = 0;
	state.out_has_quotes = false;
	state.out_has_escape = false;
	state.out_has_wildcard = false;
	word_scan(&state);
	state.word = word;
	state.out = first;
	state.out_append = &first;
	word_scan(&state);
	return (first);
}

/*
	Add a character to current field. Recognizes unquoted wildcards to mark the
	field as a pattern, and escapes quoted wildcards and instances of the escape
	character itself.
*/
void	word_out_char(struct s_word_state *state, char c, bool quoted)
{
	if (c == '*' && !quoted)
		state->out_has_wildcard = true;
	else if (c == INTERNAL_ESCAPE || c == '*')
	{
		state->out_has_escape = true;
		if (state->out)
			state->out->value[state->out_idx] = INTERNAL_ESCAPE;
		state->out_idx++;
	}
	if (state->out)
		state->out->value[state->out_idx] = c;
	state->out_idx++;
}

/*
	Finalize a field after its value has been filled in, and move to next field.
	If current field is a pattern, add matched filenames to the fields list and
	free the pattern. Otherwise, or with no matches, unescape the field value
	and add it to the fields list.
*/
static void	word_out_finalize_field(struct s_word_state *state)
{
	struct s_word_field	*current;
	bool				had_matches;

	current = state->out;
	state->out = current->next;
	current->value[state->out_idx] = '\0';
	if (state->out_has_wildcard)
		word_filename(current->value, &state->out_append, &had_matches);
	if (state->out_has_wildcard && had_matches)
		free(current);
	else
	{
		if (state->out_has_escape)
			word_unescape(current->value);
		*state->out_append = current;
		state->out_append = &current->next;
	}
}

/*
	End the current field and prepare for the next one. During first pass,
	allocate memory for the current field and add it to preliminary linked list.
	During second pass, finalize current field and add it to final linked list.
	Empty fields are not stored unless input had quotes (quoted empty strings).
*/
void	word_out_split(struct s_word_state *state)
{
	struct s_word_field	*new_field;

	if (state->out_idx == 0 && !state->out_has_quotes)
		return ;
	if (!state->out)
	{
		new_field = malloc(sizeof(*new_field) + state->out_idx + 1);
		new_field->next = NULL;
		*state->out_append = new_field;
		state->out_append = &new_field->next;
	}
	else
		word_out_finalize_field(state);
	state->out_idx = 0;
	state->out_has_quotes = false;
	state->out_has_escape = false;
	state->out_has_wildcard = false;
}
