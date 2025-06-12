/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:53:50 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/13 00:06:49 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

#include <stdbool.h>
#include <stdlib.h>

#include "libft.h"

#include "util.h"

// processed from end to start
// op that is prefix of another op must be listed first (processed last)
static const t_operator_def	g_tokenizer_ops[] = {
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

/*
	Consume current character, and if it began a quotation, consume until end
	of the quotation (including the closing quote).
*/
static t_status	tokenizer_consume_char_or_quoted(t_tokenizer_state *state)
{
	char	maybe_quote;

	maybe_quote = *state->line_pos++;
	if (maybe_quote == '"' || maybe_quote == '\'')
	{
		while (*state->line_pos != maybe_quote)
		{
			if (*state->line_pos == '\0')
				return (status_err(S_RESET_SYNTAX, "syntax error",
						"unbalanced quotes", 0));
			state->line_pos++;
		}
		state->line_pos++;
	}
	return (S_OK);
}

/*
	Check if current and following character match an operator. If op_type is
	not null, write the token type to the location it points to and consume
	the characters making up the operator.
*/
static bool	tokenizer_is_operator(t_tokenizer_state *state,
			enum e_token *op_type)
{
	size_t	i;
	size_t	op_len;

	i = sizeof g_tokenizer_ops / sizeof g_tokenizer_ops[0];
	while (i--)
	{
		op_len = ft_strlen(g_tokenizer_ops[i].str);
		if (ft_strncmp(state->line_pos, g_tokenizer_ops[i].str, op_len) == 0)
		{
			if (op_type)
			{
				*op_type = g_tokenizer_ops[i].type;
				state->line_pos += op_len;
			}
			return (true);
		}
	}
	return (false);
}

static t_status	tokenizer_build_word(t_tokenizer_state *state, char **word_out)
{
	t_status	status;
	char		*word_start;
	size_t		word_len;
	char		*word;

	word_start = state->line_pos;
	while (1)
	{
		status = tokenizer_consume_char_or_quoted(state);
		if (status != S_OK)
			return (status);
		if (*state->line_pos == '\0' || util_isblank(*state->line_pos))
			break ;
		if (tokenizer_is_operator(state, NULL))
			break ;
	}
	word_len = state->line_pos - word_start;
	word = malloc(word_len + 1);
	*word_out = word;
	if (!word)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	ft_memcpy(word, word_start, word_len);
	word[word_len] = '\0';
	return (S_OK);
}

t_status	tokenizer_get_next(t_tokenizer_state *state, t_token *token_out)
{
	while (util_isblank(*state->line_pos))
		state->line_pos++;
	token_out->word_content = NULL;
	token_out->type = TOK_END;
	if (*state->line_pos == '\0')
		return (S_OK);
	if (tokenizer_is_operator(state, &token_out->type))
		return (S_OK);
	token_out->type = TOK_WORD;
	return (tokenizer_build_word(state, &token_out->word_content));
}
