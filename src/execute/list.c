/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:35:32 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/04 22:19:05 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "execute_internal.h"

#include <stdbool.h>

#include "ast.h"
#include "shenv.h"
#include "status.h"

/*
	Execute a single list entry (pipeline or group)

	The error case should be impossible to hit if the parser works correctly.
*/
static t_status	execute_list_entry(struct s_ast_list_entry *entry, t_shenv *env)
{
	if (entry->type == AST_LIST_PIPELINE)
		return (execute_pipeline(entry->pipeline, env));
	else if (entry->type == AST_LIST_GROUP)
		return (execute_list(entry->group, env));
	else
		return (status_err(S_EXIT_ERR, "execute_list: internal error",
				"invalid ast_list_entry type", 0));
}

/*
	Execute a command list, handling logical operators && and ||
	Commands are executed left to right, evaluating each action for AND/OR
*/
t_status	execute_list(struct s_ast_list_entry *list_head, t_shenv *env)
{
	t_status	status;
	bool		execute_next;
	int			exit_code;

	if (!list_head)
		return (S_OK);
	status = execute_list_entry(list_head, env);
	while (list_head->next && status == S_OK)
	{
		exit_code = env->exit_code;
		execute_next = (list_head->next_op == AST_LIST_AND && exit_code == 0)
			|| (list_head->next_op == AST_LIST_OR && exit_code != 0);
		list_head = list_head->next;
		if (execute_next)
			status = execute_list_entry(list_head, env);
	}
	return (status);
}
