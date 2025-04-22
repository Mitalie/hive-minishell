/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:35:32 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/21 18:49:16 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

#include <stdbool.h>

#include "ast.h"
/*
    Execute a single list entry (pipeline or group)
    Returns the exit status of the executed entry
*/
static int	execute_list_entry(struct s_ast_list_entry *entry, t_env *env)
{
	int	status;

	if (entry->type == AST_LIST_PIPELINE)
		status = execute_pipeline(entry->pipeline, env);
	else if (entry->type == AST_LIST_GROUP)
		status = execute_list(entry->group, env);
	else
		status = 1;
	return (status);
}

/*
    Execute a command list, handling logical operators && and ||
    Commands are executed left to right, evaluating each action for AND/OR
    Returns the exit status of the last executed command
*/
int	execute_list(struct s_ast_list_entry *list_head, t_env *env)
{
	int		status;
	bool	execute_next;

	if (!list_head)
		return (0);
	status = execute_list_entry(list_head, env);
	while (list_head->next)
	{
		execute_next = (list_head->next_op == AST_LIST_AND && status == 0)
			|| (list_head->next_op == AST_LIST_OR && status != 0);
		list_head = list_head->next;
		if (execute_next)
			status = execute_list_entry(list_head, env);
	}
	return (status);
}
