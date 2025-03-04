/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:54:57 by amakinen          #+#    #+#             */
/*   Updated: 2025/03/04 18:18:25 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word.h"
#include "word_internal.h"

#include <stdbool.h>
#include <stdlib.h>

#include "util.h"

/*
Clang-tidy reports a potential null pointer reference when calling word_expand:
1. First pass finds a variable which doesn't exist, no extra fields are created.
2. End of first pass creates a field, setting its ->next to NULL.
3. Second pass finds a variable which exists, finds a blank, and ends the field.
4. End of that field sets state->field_curr from the ->next that has NULL.
5. Next character write in either var or word dereferences state->field_curr.
This can't actually happen, because the environment doesn't change by itself.
Both passes will see same values, producing same field count and field sizes.

Of course, this relies on us actually not changing the environment during
word_expand.
*/

static void	word_expand_scan(struct s_word_expand_state *state);
static void	word_expand_var(struct s_word_expand_state *state, bool unquoted);
static char	*word_expand_get_var(struct s_word_expand_state *state);
static void	word_expand_end_field(struct s_word_expand_state *state);

/*
	Expand a word into a linked list of fields.

	Performs two passes over the word, handling variable expansion and field
	splitting. First pass counts field lengths and allocates memory for them,
	second pass writes characters into the fields. Each completed field is
	further passed to pathname expansion and quote removal as necessary.
*/
struct s_word_field	*word_expand(char *word)
{
	struct s_word_expand_state	state;

	state.field_first = NULL;
	state.field_curr = NULL;
	state.field_pos = 0;
	state.field_has_unquoted_wildcard = false;
	state.word = word;
	state.write = false;
	word_expand_scan(&state);
	state.word = word;
	state.write = true;
	state.field_curr = state.field_first;
	word_expand_scan(&state);
	return (state.field_first);
}

/*
	Scan the word, counting or copying characters into the current output field,
	and detecting and processing relevant special characters.
*/
static void	word_expand_scan(struct s_word_expand_state *state)
{
	char	quote;
	char	c;

	quote = 0;
	while (true)
	{
		c = *state->word++;
		if (c == '\0')
			break ;
		if (state->write)
			state->field_curr->value[state->field_pos] = c;
		state->field_pos++;
		if (quote && c == quote)
			quote = 0;
		else if (!quote && (c == '\'' || c == '"'))
			quote = c;
		else if (c == '$' && (!quote || quote == '"'))
			word_expand_var(state, !quote);
		else if (c == '*' && !quote)
			state->field_has_unquoted_wildcard = true;
	}
	word_expand_end_field(state);
}

/*
	Identify a variable name in word and scan the value of the corresponding
	environment value, counting or copying characters into the current output
	field, and detecting and processing relevant special characters.

	TODO: escape quotes in value so that they survive quote removal.
*/
static void	word_expand_var(struct s_word_expand_state *state, bool unquoted)
{
	char	*value;
	char	c;

	value = word_expand_get_var(state);
	if (!value)
		return ;
	while (true)
	{
		c = *value++;
		if (unquoted && util_isblank(c))
			word_expand_end_field(state);
		while (unquoted && util_isblank(c))
			c = *value++;
		if (c == '\0')
			return ;
		if (state->write)
			state->field_curr->value[state->field_pos] = c;
		state->field_pos++;
		if (unquoted && c == '*')
			state->field_has_unquoted_wildcard = true;
	}
}

/*
	Read variable name from word and return a pointer to the corresponding
	environment value as returned by getenv. Decrements field_pos to remove
	the $ character from output, but only if it introduced a valid expansion.

	TODO: handle `$?` (exit status special parameter)
*/
static char	*word_expand_get_var(struct s_word_expand_state *state)
{
	char	*name_start;
	char	*value;
	char	next_char_temp;

	name_start = state->word;
	if (util_isname(*state->word))
		while (util_isname(*state->word))
			state->word++;
	else
		return (NULL);
	state->field_pos--;
	next_char_temp = *state->word;
	*state->word = '\0';
	value = getenv(name_start);
	*state->word = next_char_temp;
	return (value);
}

/*
	Process the end of a field and reset scan state for the next field.

	On scan pass, allocates memory for the ended field based on the counted
	length, and appends it to a linked list using field_curr as a tail pointer.
	
	On write pass, passes the completed field to filename generation and quote
	removal, and then updates field_curr to the next list node.

	TODO: filename generation
	TODO: quote removal
	TODO: handle malloc error
*/
static void	word_expand_end_field(struct s_word_expand_state *state)
{
	struct s_word_field	*field;

	if (state->write)
	{
		state->field_curr->value[state->field_pos] = '\0';
		state->field_curr = state->field_curr->next;
		state->field_pos = 0;
		state->field_has_unquoted_wildcard = false;
		return ;
	}
	field = malloc(sizeof(*field) + state->field_pos);
	if (state->field_curr)
		state->field_curr->next = field;
	else
		state->field_first = field;
	state->field_curr = field;
	field->next = NULL;
	state->field_pos = 0;
	state->field_has_unquoted_wildcard = false;
	return ;
}
