/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_pipeline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 20:14:05 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/23 14:31:54 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

/*
	Creates a simple command node for the AST.
	Allocates memory for the node and initializes its fields to NULL.
	Returns the created node or NULL on failure.
*/
struct s_ast_simple_command	*create_simple_command(void)
{
	struct s_ast_simple_command	*cmd;

	cmd = malloc(sizeof(struct s_ast_simple_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}
