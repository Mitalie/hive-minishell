/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 11:22:25 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 20:48:24 by amakinen         ###   ########.fr       */
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
	struct s_token **tokens,
	struct s_ast_redirect **redirect)
{
	struct s_ast_redirect	*new_redir;

	new_redir = malloc(sizeof(*new_redir));
	if (!new_redir)
		return (PARSER_ERR_MALLOC);
	*redirect = new_redir;
	new_redir->next = NULL;
	new_redir->word = NULL;
	new_redir->op = redirect_token_to_op((*tokens)->type);
	(*tokens)++;
	if ((*tokens)->type != TOK_WORD)
	{
		parser_syntax_error("expected word for redirect");
		return (PARSER_ERR_SYNTAX);
	}
	new_redir->word = (*tokens)->word_content;
	(*tokens)++;
	return (PARSER_SUCCESS);
}
