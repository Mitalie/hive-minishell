/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:53:50 by amakinen          #+#    #+#             */
/*   Updated: 2025/02/20 19:58:35 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "libft.h"

// processed from end to start
// op that is prefix of another op must be listed first (processed last)
const t_operator_def	g_ops[] = {
{"<", TOK_REDIR_IN},
{">", TOK_REDIR_OUT},
{">>", TOK_REDIR_APP},
{"<<", TOK_HEREDOC},
{"|", TOK_PIPE},
{"&&", TOK_AND},
{"||", TOK_OR},
{"(", TOK_GROUP_START},
{")", TOK_GROUP_END},
{"\n", TOK_END},
};

static bool	tok_isblank(char c)
{
	if (c == ' ' || c == '\t')
		return (true);
	return (false);
}

/*
	Consume current character, and if it began a quotation, consume until end
	of the quotation (including the closing quote).

	TODO: End of line in quotes? Unbalanced quotes don't need to be supported,
	but this runs off the end of the line and segfaults. Shoud either pass on
	the unbalanced quotes within the word, or report error somehow.
*/
static void	tok_consume_char_or_quoted(t_tokenizer_state *state)
{
	char	maybe_quote;

	maybe_quote = *state->line_pos++;
	if (maybe_quote == '"' || maybe_quote == '\'')
	{
		while (*state->line_pos != maybe_quote)
			state->line_pos++;
		state->line_pos++;
	}
}

t_token	tokenizer_get_next(t_tokenizer_state *state)
{
	char	*word_start;
	int		i;
	size_t	word_len;
	char	*word;

	// read new line if no line or current line consumed
	// TODO: free line before return if consumed
	if (!state->line || *state->line_pos == '\0')
	{
		free(state->line);
		state->line = readline("test prompt");
		state->line_pos = state->line;
	}
	// skip blanks
	while (tok_isblank(*state->line_pos))
		state->line_pos++;
	// check for operators
	i = sizeof g_ops / sizeof g_ops[0];
	while (i--)
		if (ft_strncmp(state->line_pos, g_ops[i].str, ft_strlen(g_ops[i].str)) == 0)
			return ((t_token){g_ops[i].type, NULL});
	// not op so it's word
	word_start = state->line_pos;
	while (1)
	{
		tok_consume_char_or_quoted(state);
		// end if next is eol or blank or op
		if (*state->line_pos == '\0' || tok_isblank(*state->line_pos))
			break ;
		i = sizeof g_ops / sizeof g_ops[0];
		while (i--)
			if (ft_strncmp(state->line_pos, g_ops[i].str, ft_strlen(g_ops[i].str)) == 0)
				break ;
		// else keep going with the next char
	}
	word_len = state->line_pos - word_start;
	word = malloc(word_len + 1);
	ft_memcpy(word, word_start, word_len);
	word[word_len] = '\0';
	return ((t_token){TOK_WORD, word});
}
