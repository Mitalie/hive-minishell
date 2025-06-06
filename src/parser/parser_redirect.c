/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 11:22:25 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/12 22:43:52 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include <stdlib.h>

#include "ast.h"
#include "status.h"
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
t_status	parser_redirect(
	struct s_parser_state *state,
	struct s_ast_redirect **redirect)
{
	t_status				status;
	struct s_ast_redirect	*new_redirect;

	new_redirect = malloc(sizeof(*new_redirect));
	if (!new_redirect)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	*redirect = new_redirect;
	new_redirect->next = NULL;
	new_redirect->heredoc_lines = NULL;
	new_redirect->op = redirect_token_to_op(state->curr_tok.type);
	parser_next_token(state);
	if (state->curr_tok.type != TOK_WORD)
		return (parser_syntax_error("expected word after redirect"));
	new_redirect->word = state->curr_tok.word_content;
	parser_next_token(state);
	status = S_OK;
	if (new_redirect->op == AST_HEREDOC)
		status = read_heredoc(new_redirect);
	return (status);
}
