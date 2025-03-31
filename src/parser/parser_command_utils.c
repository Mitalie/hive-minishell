/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_elements.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:53:36 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 01:53:36 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "ast.h"
#include <stdlib.h>

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
		&& (*tokens)->type != TOK_END && (*tokens)->type != TOK_GROUP_END)
	{
		if ((*tokens)->type == TOK_WORD)
		{
			if (!parse_word(tokens, args_append))
				return (NULL);
			args_append = &(*args_append)->next;
		}
		else if ((*tokens)->type >= TOK_REDIR_IN
			&& (*tokens)->type <= TOK_HEREDOC)
		{
			if (!parse_redirect(tokens, redirs_append))
				return (NULL);
			redirs_append = &(*redirs_append)->next;
		}
		else
			break ;
		(*tokens)++;
	}
	return (cmd);
}

/*
	Parses a simple command from the token list.
	Returns the created command node or NULL on failure.
*/
struct s_ast_simple_command	*parse_simple_command(struct s_token **tokens)
{
	struct s_ast_simple_command	*cmd;
	struct s_ast_command_word	**args_append;
	struct s_ast_redirect		**redirs_append;

	if (!tokens || !(*tokens))
		return (NULL);
	cmd = create_simple_command();
	if (!cmd)
		return (NULL);
	args_append = &cmd->args;
	redirs_append = &cmd->redirs;
	cmd = process_command_elements(tokens, cmd, args_append, redirs_append);
	if (!cmd)
		return (NULL);
	return (cmd);
}
