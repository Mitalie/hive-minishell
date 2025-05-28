/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:54:57 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/12 19:10:49 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word.h"
#include "word_internal.h"

#include <stdbool.h>
#include <stdlib.h>

#include "status.h"

/*
	Perform all word expansions on the given word and return the resulting field
	list. In case of an error, fields may be allocated and must be freed, but
	their content is undefined.
*/
t_status	word_expand(char *word, struct s_word_field **fields_out)
{
	struct s_word_state	state;
	t_status			status;

	*fields_out = NULL;
	state.word = word;
	state.out = NULL;
	state.out_append = fields_out;
	state.out_idx = 0;
	state.out_has_quotes = false;
	state.out_has_escape = false;
	state.out_has_wildcard = false;
	status = word_scan(&state);
	if (status != S_OK)
		return (status);
	state.word = word;
	state.out = *fields_out;
	state.out_append = fields_out;
	status = word_scan(&state);
	if (status != S_OK)
		return (status);
	return (S_OK);
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
static t_status	word_out_finalize_field(struct s_word_state *state)
{
	struct s_word_field	*current;
	bool				had_matches;
	t_status			status;

	current = state->out;
	state->out = current->next;
	current->value[state->out_idx] = '\0';
	if (state->out_has_wildcard)
	{
		status = word_filename(current->value, &state->out_append,
				&had_matches);
		if (had_matches)
			free(current);
		if (had_matches || status != S_OK)
			return (status);
	}
	if (state->out_has_escape)
		word_unescape(current->value);
	*state->out_append = current;
	state->out_append = &current->next;
	return (S_OK);
}

/*
	End the current field and prepare for the next one. During first pass,
	allocate memory for the current field and add it to preliminary linked list.
	During second pass, finalize current field and add it to final linked list.
	Empty fields are not stored unless input had quotes (quoted empty strings).
*/
t_status	word_out_split(struct s_word_state *state)
{
	struct s_word_field	*new_field;
	t_status			status;

	if (state->out_idx == 0 && !state->out_has_quotes)
		return (S_OK);
	if (!state->out)
	{
		new_field = malloc(sizeof(*new_field) + state->out_idx + 1);
		if (!new_field)
			return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
		new_field->next = NULL;
		*state->out_append = new_field;
		state->out_append = &new_field->next;
	}
	else
	{
		status = word_out_finalize_field(state);
		if (status != S_OK)
			return (status);
	}
	state->out_idx = 0;
	state->out_has_quotes = false;
	state->out_has_escape = false;
	state->out_has_wildcard = false;
	return (S_OK);
}
