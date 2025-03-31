/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_simple_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:53:36 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 20:47:53 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include <stdbool.h>
#include <stdlib.h>

#include "ast.h"
#include "parser.h"
#include "tokenizer.h"

static bool	is_redirect_token(struct s_token *token)
{
	if (token->type == TOK_REDIR_IN)
		return (true);
	if (token->type == TOK_REDIR_OUT)
		return (true);
	if (token->type == TOK_REDIR_APP)
		return (true);
	if (token->type == TOK_HEREDOC)
		return (true);
	return (false);
}

/*
	Process elements of a command (words and redirections).
*/
static enum e_parser_status	process_command_elements(
	struct s_token **tokens,
	struct s_ast_simple_command *new_command)
{
	enum e_parser_status		status;
	struct s_ast_command_word	**args_append;
	struct s_ast_redirect		**redirs_append;

	args_append = &new_command->args;
	redirs_append = &new_command->redirs;
	while (is_redirect_token(*tokens) || (*tokens)->type == TOK_WORD)
	{
		if (is_redirect_token(*tokens))
		{
			status = parser_redirect(tokens, redirs_append);
			if (status != PARSER_SUCCESS)
				return (status);
			redirs_append = &(*redirs_append)->next;
		}
		else
		{
			status = parser_word(tokens, args_append);
			if (status != PARSER_SUCCESS)
				return (status);
			args_append = &(*args_append)->next;
		}
	}
	return (PARSER_SUCCESS);
}

/*
	Parses a simple command from the token list.
*/
enum e_parser_status	parser_simple_command(
	struct s_token **tokens,
	struct s_ast_simple_command **simple_command)
{
	enum e_parser_status		status;
	struct s_ast_simple_command	*new_command;

	new_command = malloc(sizeof(*new_command));
	if (!new_command)
		return (PARSER_ERR_MALLOC);
	*simple_command = new_command;
	new_command->next = NULL;
	new_command->args = NULL;
	new_command->redirs = NULL;
	status = process_command_elements(tokens, new_command);
	if (status != PARSER_SUCCESS)
		return (status);
	if (new_command->args == NULL && new_command->redirs == NULL)
	{
		parser_syntax_error("expected word or redirect");
		return (PARSER_ERR_SYNTAX);
	}
	return (PARSER_SUCCESS);
}
