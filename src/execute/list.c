/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:35:32 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/04 22:02:07 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "ast.h"
#include <stdbool.h>
/*
	To Do: 
	Create a condition to decide when to execute the next command in the list.
*/
static int	execute_list_entry(struct s_ast_list_entry *entry)
{
	int	status;

	if (entry->type == AST_LIST_PIPELINE)
		status = execute_pipeline(entry->pipeline);
	else if (entry->type == AST_LIST_GROUP)
		status = execute_list(entry->group);
	else
		status = 1;
	return (status);
}

int	execute_list(struct s_ast_list_entry *list_head)
{
	int		status;
	bool	execute_next;

	if (!list_head)
		return (0);
	status = execute_list_entry(list_head);
	while (list_head->next)
	{
		list_head = list_head->next;
		if (execute_next)
			status = execute_list_entry(list_head);
	}
	return (status);
}
