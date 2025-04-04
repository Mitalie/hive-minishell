/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_expand2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:03:56 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/04 17:33:28 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word.h"
#include "word_internal2.h"

#include <stdlib.h>

#include "util.h"

char	word_expand_next_char(struct s_word_expand_state *state);
void	word_expand_output_char(struct s_word_expand_state *state, char c);
void	word_expand_output_split(struct s_word_expand_state *state);

struct s_word_field	*word_expand(char *word)
{
}

char	word_expand_next_char(struct s_word_expand_state *state)
{
	struct s_word_expand_scan	*scan;
	char						*varname;
	char						name_end_temp;

	scan = &state->count;
	if (state->is_write_scan)
		scan = &state->write;
	while (true)
	{
		if (scan->var && *scan->var)
			return (*scan->var++);
		scan->var = NULL;
		if (*scan->word != '$')
			return (*scan->word++);
		scan->word++;
		if (!util_isname(*scan->word))
			return ('$');
		varname = scan->word;
		while (util_isname(*scan->word))
			scan->word++;
		name_end_temp = *scan->word;
		*scan->word = '\0';
		scan->var = getenv(varname);
		*scan->word = name_end_temp;
	}
}

void	word_expand_output_char(struct s_word_expand_state *state, char c)
{
	if (state->is_write_scan)
		state->out_curr->value[state->out_idx] = c;
	state->out_idx++;
}
