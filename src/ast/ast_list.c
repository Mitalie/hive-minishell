/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:24:49 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 03:34:01 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

/*
	Creates a list entry node for the AST.
	Allocates memory for the node and initializes it with the provided type.
	Returns the created node or NULL on failure.
*/
struct s_ast_list_entry	*create_list_entry(enum e_ast_list_entry_type type)
{
	struct s_ast_list_entry	*node;

	node = malloc(sizeof(struct s_ast_list_entry));
	if (!node)
		return (NULL);
	node->type = type;
	node->next_op = AST_LIST_AND;
	node->next = NULL;
	if (type == AST_LIST_GROUP)
		node->group = NULL;
	else
		node->pipeline = NULL;
	return (node);
}

/*
	Sets the group pointer for a list entry of type AST_LIST_GROUP.
	Returns 1 on success, 0 if entry is not of the correct type.
*/
int	set_list_entry_group(struct s_ast_list_entry *entry,
						struct s_ast_list_entry *group)
{
	if (!entry || entry->type != AST_LIST_GROUP)
		return (0);
	entry->group = group;
	return (1);
}

/*
	Sets the pipeline pointer for a list entry of type AST_LIST_PIPELINE.
	Returns 1 on success, 0 if entry is not of the correct type.
*/
int	set_list_entry_pipeline(struct s_ast_list_entry *entry,
							struct s_ast_simple_command *pipeline)
{
	if (!entry || entry->type != AST_LIST_PIPELINE)
		return (0);
	entry->pipeline = pipeline;
	return (1);
}
