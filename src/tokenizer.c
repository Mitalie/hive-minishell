/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:53:50 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/03 19:12:16 by amakinen         ###   ########.fr       */
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

/*
	Check if current and following character match an operator. If op_type is
	not null, write the token type to the location it points to and consume
	the characters making up the operator.
*/
static bool	tok_is_operator(t_tokenizer_state *state, enum e_token *op_type)
{
	size_t	i;
	size_t	op_len;

	i = sizeof g_ops / sizeof g_ops[0];
	while (i--)
	{
		op_len = ft_strlen(g_ops[i].str);
		if (ft_strncmp(state->line_pos, g_ops[i].str, op_len) == 0)
		{
			if (op_type)
			{
				*op_type = g_ops[i].type;
				state->line_pos += op_len;
			}
			return (true);
		}
	}
	return (false);
}

static char	*tok_build_word(t_tokenizer_state *state)
{
	char			*word_start;
	size_t			word_len;
	char			*word;

	word_start = state->line_pos;
	while (1)
	{
		tok_consume_char_or_quoted(state);
		if (*state->line_pos == '\0' || tok_isblank(*state->line_pos))
			break ;
		if (tok_is_operator(state, NULL))
			break ;
	}
	word_len = state->line_pos - word_start;
	word = malloc(word_len + 1);
	ft_memcpy(word, word_start, word_len);
	word[word_len] = '\0';
	return (word);
}

t_token	tokenizer_get_next(t_tokenizer_state *state)
{
	enum e_token	op_type;

	if (!state->line)
	{
		state->line = readline("test prompt");
		state->line_pos = state->line;
	}
	if (!state->line)
	{
		state->eof_reached = true;
		return ((t_token){TOK_END, NULL});
	}
	while (tok_isblank(*state->line_pos))
		state->line_pos++;
	if (*state->line_pos == '\0')
		return ((t_token){TOK_END, NULL});
	else if (tok_is_operator(state, &op_type))
		return ((t_token){op_type, NULL});
	else
		return ((t_token){TOK_WORD, tok_build_word(state)});
}
