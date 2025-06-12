/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_simple_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:53:36 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/12 22:49:27 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include <stdbool.h>
#include <stdlib.h>

#include "ast.h"
#include "status.h"
#include "tokenizer.h"

static bool	parser_is_redirect_token(struct s_parser_state *state)
{
	if (state->curr_tok.type == TOK_REDIR_IN)
		return (true);
	if (state->curr_tok.type == TOK_REDIR_OUT)
		return (true);
	if (state->curr_tok.type == TOK_REDIR_APP)
		return (true);
	if (state->curr_tok.type == TOK_HEREDOC)
		return (true);
	return (false);
}

/*
	Process elements of a command (words and redirections).
*/
static t_status	parser_process_command_elements(
	struct s_parser_state *state,
	struct s_ast_simple_command *new_command)
{
	t_status					status;
	struct s_ast_command_word	**args_append;
	struct s_ast_redirect		**redirs_append;

	args_append = &new_command->args;
	redirs_append = &new_command->redirs;
	while (parser_is_redirect_token(state) || state->curr_tok.type == TOK_WORD)
	{
		if (parser_is_redirect_token(state))
		{
			status = parser_redirect(state, redirs_append);
			if (status != S_OK)
				return (status);
			redirs_append = &(*redirs_append)->next;
		}
		else
		{
			status = parser_word(state, args_append);
			if (status != S_OK)
				return (status);
			args_append = &(*args_append)->next;
		}
	}
	return (S_OK);
}

/*
	Parses a simple command from the token list.
*/
t_status	parser_simple_command(
	struct s_parser_state *state,
	struct s_ast_simple_command **simple_command)
{
	t_status					status;
	struct s_ast_simple_command	*new_command;

	new_command = malloc(sizeof(*new_command));
	if (!new_command)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	*simple_command = new_command;
	new_command->next = NULL;
	new_command->args = NULL;
	new_command->redirs = NULL;
	status = parser_process_command_elements(state, new_command);
	if (status != S_OK)
		return (status);
	if (new_command->args == NULL && new_command->redirs == NULL)
		return (parser_syntax_error("expected word or redirect"));
	return (S_OK);
}
