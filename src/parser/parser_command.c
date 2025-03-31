/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 11:22:25 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 03:36:02 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "ast.h"
#include <stdlib.h>

/*
	Gets the redirection operation based on token type.
	Returns the corresponding AST redirect operation.
*/
static enum e_ast_redirect_op	get_redirect_op(enum e_token type)
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
	Parses a redirect and adds it to the command.
	Returns 1 on success, 0 on failure.
*/
int	parse_redirect(struct s_token **tokens,
		struct s_ast_redirect **redirs_append)
{
	enum e_ast_redirect_op	op;
	struct s_ast_redirect	*redir;

	op = get_redirect_op((*tokens)->type);
	(*tokens)++;
	if (!(*tokens) || (*tokens)->type != TOK_WORD)
		return (0);
	redir = create_redirect(op, (*tokens)->word_content);
	if (!redir)
		return (0);
	*redirs_append = redir;
	return (1);
}

/*
	Parses a word and adds it to the command arguments.
	Returns 1 on success, 0 on failure.
*/
int	parse_word(struct s_token **tokens,
		struct s_ast_command_word **args_append)
{
	struct s_ast_command_word	*arg;

	arg = create_command_word((*tokens)->word_content);
	if (!arg)
		return (0);
	*args_append = arg;
	return (1);
}
