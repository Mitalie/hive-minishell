/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 11:22:25 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/25 15:42:00 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include <stdlib.h>

#include "ast.h"
#include "parser.h"
#include "tokenizer.h"

static enum e_ast_redirect_op	redirect_token_to_op(enum e_token type)
{
	if (type == TOK_REDIR_IN)
		return (AST_REDIR_IN);
	else if (type == TOK_REDIR_OUT)
		return (AST_REDIR_OUT);
	else if (type == TOK_REDIR_APP)
		return (AST_REDIR_APP);
	else
		return (AST_HEREDOC);
}

/*
	Parses a redirect and adds it to the AST.
	Next token must be a redirect token.
*/
enum e_parser_status	parser_redirect(
	struct s_parser_state *state,
	struct s_ast_redirect **redirect)
{
	enum e_parser_status	status;
	struct s_ast_redirect	*new_redirect;

	new_redirect = malloc(sizeof(*new_redirect));
	if (!new_redirect)
		return (PARSER_ERR_MALLOC);
	new_redirect->next = NULL;
	new_redirect->fd = -1;
	new_redirect->op = redirect_token_to_op(state->curr_tok.type);
	parser_next_token(state);
	if (state->curr_tok.type != TOK_WORD)
	{
		free(new_redirect);
		parser_syntax_error("expected word after redirect");
		return (PARSER_ERR_SYNTAX);
	}
	new_redirect->word = state->curr_tok.word_content;
	parser_next_token(state);
	*redirect = new_redirect;
	status = PARSER_SUCCESS;
	if (new_redirect->op == AST_HEREDOC)
		status = add_heredoc_to_list(state, new_redirect);
	return (status);
}
