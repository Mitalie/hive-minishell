/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:22:31 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 03:39:05 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "parser.h"
#include "ast.h"
#include <stdlib.h>

/*
	Processes a pipeline segment after the first command.
	Returns 0 on syntax error, 1 on success.
*/
static int	process_pipe_segment(struct s_token **tokens,
				struct s_ast_simple_command ***next_cmd_ptr)
{
	struct s_ast_simple_command	*current_cmd;

	(*tokens)++;
	if (!(*tokens) || (*tokens)->type == TOK_END)
	{
		print_syntax_error("unexpected end after pipe");
		return (0);
	}
	current_cmd = parse_simple_command(tokens);
	if (!current_cmd)
		return (0);
	**next_cmd_ptr = current_cmd;
	*next_cmd_ptr = &(current_cmd->next);
	return (1);
}

/*
	Parses a pipeline from the token list.
	Pipelines consist of simple commands separated by pipe tokens.
	Returns the first command in the pipeline, or NULL on failure.
*/
struct s_ast_simple_command	*parse_pipeline(struct s_token **tokens)
{
	struct s_ast_simple_command	*first_cmd;
	struct s_ast_simple_command	**next_cmd_ptr;
	int							status;

	if (!tokens || !(*tokens))
		return (NULL);
	first_cmd = parse_simple_command(tokens);
	if (!first_cmd)
		return (NULL);
	next_cmd_ptr = &(first_cmd->next);
	while (*tokens && (*tokens)->type == TOK_PIPE)
	{
		status = process_pipe_segment(tokens, &next_cmd_ptr);
		if (!status)
		{
			free_simple_command(first_cmd);
			return (NULL);
		}
	}
	return (first_cmd);
}
