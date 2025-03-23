/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 11:22:25 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/23 14:31:57 by josmanov         ###   ########.fr       */
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
static int	parse_redirect(struct s_token **tokens,
		struct s_ast_redirect ***redirs_append)
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
	**redirs_append = redir;
	*redirs_append = &redir->next;
	return (1);
}

/*
	Parses a word and adds it to the command arguments.
	Returns 1 on success, 0 on failure.
*/
static int	parse_word(struct s_token **tokens,
		struct s_ast_command_word ***args_append)
{
	struct s_ast_command_word	*arg;

	arg = create_command_word((*tokens)->word_content);
	if (!arg)
		return (0);
	**args_append = arg;
	*args_append = &arg->next;
	return (1);
}

/*
	Parses a simple command from the token list.
	Constructs an AST node for the command, including arguments and redirections.
	Returns the created command node or NULL on failure.
*/
struct s_ast_simple_command	*parse_simple_command(struct s_token **tokens)
{
	struct s_ast_simple_command		*cmd;
	struct s_ast_command_word		**args_append;
	struct s_ast_redirect			**redirs_append;

	if (!tokens || !(*tokens))
		return (NULL);
	cmd = malloc(sizeof(struct s_ast_simple_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	args_append = &cmd->args;
	redirs_append = &cmd->redirs;
	return (process_command_elements(tokens, cmd, args_append, redirs_append));
}

/*
	Process elements of a command (words and redirections).
	Returns the command or NULL on failure.
*/
struct s_ast_simple_command	*process_command_elements(struct s_token **tokens,
		struct s_ast_simple_command *cmd,
		struct s_ast_command_word **args_append,
		struct s_ast_redirect **redirs_append)
{
	while (*tokens && (*tokens)->type != TOK_PIPE
		&& (*tokens)->type != TOK_AND && (*tokens)->type != TOK_OR
		&& (*tokens)->type != TOK_END)
	{
		if ((*tokens)->type == TOK_WORD)
		{
			if (!parse_word(tokens, &args_append))
				return (NULL);
		}
		else if ((*tokens)->type >= TOK_REDIR_IN
			&& (*tokens)->type <= TOK_HEREDOC)
		{
			if (!parse_redirect(tokens, &redirs_append))
				return (NULL);
		}
		else
			break ;
		(*tokens)++;
	}
	return (cmd);
}
