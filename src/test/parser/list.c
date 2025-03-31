/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:55:28 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 03:48:48 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "parser.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

/* Print the list entry node details */
void	print_list_entry(struct s_ast_list_entry *entry)
{
	if (!entry)
	{
		printf("List entry is NULL\n");
		return ;
	}
	printf("List Entry:\n");
	printf("  Type: ");
	if (entry->type == AST_LIST_GROUP)
		printf("Group\n");
	else if (entry->type == AST_LIST_PIPELINE)
		printf("Pipeline\n");
	printf("  Next operator: ");
	if (entry->next_op == AST_LIST_AND)
		printf("AND (&&)\n");
	else if (entry->next_op == AST_LIST_OR)
		printf("OR (||)\n");
	if (entry->type == AST_LIST_GROUP)
	{
		if (entry->group)
			printf("  Has group pointer: Yes\n");
		else
			printf("  Has group pointer: No\n");
	}
	else if (entry->type == AST_LIST_PIPELINE)
	{
		if (entry->pipeline)
			printf("  Has pipeline pointer: Yes\n");
		else
			printf("  Has pipeline pointer: No\n");
	}
}

/* Create a dummy pipeline for testing without using create_simple_command */
struct s_ast_simple_command	*create_dummy_pipeline(void)
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

int	main(void)
{
	struct s_ast_list_entry		*group_entry;
	struct s_ast_list_entry		*pipeline_entry;
	struct s_ast_list_entry		*inner_group;
	struct s_ast_list_entry		*current;
	struct s_ast_simple_command	*cmd;
	int							group_result;
	int							invalid_group_result;
	int							pipeline_result;
	int							invalid_pipeline_result;
	int							i;

	printf("=== Test 1: Create List Entries ===\n");
	group_entry = create_list_entry(AST_LIST_GROUP);
	pipeline_entry = create_list_entry(AST_LIST_PIPELINE);
	printf("Group Entry:\n");
	print_list_entry(group_entry);
	printf("\nPipeline Entry:\n");
	print_list_entry(pipeline_entry);
	printf("\n=== Test 2: Set Group Pointer ===\n");
	inner_group = create_list_entry(AST_LIST_GROUP);
	group_result = set_list_entry_group(group_entry, inner_group);
	printf("Setting group pointer result: %s\n", group_result ? "Success" : "Failed");
	print_list_entry(group_entry);
	invalid_group_result = set_list_entry_group(pipeline_entry, inner_group);
	printf("Setting group pointer on pipeline entry: %s\n",
		invalid_group_result ? "Success (ERROR)" : "Failed (Expected)");
	printf("\n=== Test 3: Set Pipeline Pointer ===\n");
	cmd = create_dummy_pipeline();
	pipeline_result = set_list_entry_pipeline(pipeline_entry, cmd);
	printf("Setting pipeline pointer result: %s\n", pipeline_result ? "Success" : "Failed");
	print_list_entry(pipeline_entry);
	invalid_pipeline_result = set_list_entry_pipeline(group_entry, cmd);
	printf("Setting pipeline pointer on group entry: %s\n",
		invalid_pipeline_result ? "Success (ERROR)" : "Failed (Expected)");
	printf("\n=== Test 4: Chain List Entries ===\n");
	group_entry->next_op = AST_LIST_AND;
	group_entry->next = pipeline_entry;
	pipeline_entry->next_op = AST_LIST_OR;
	current = group_entry;
	i = 1;
	while (current)
	{
		printf("List Entry %d:\n", i++);
		printf("  Type: %s\n", current->type == AST_LIST_GROUP ? "Group" : "Pipeline");
		printf("  Next operator: %s\n",
			current->next_op == AST_LIST_AND ? "AND (&&)" : "OR (||)");
		current = current->next;
	}
	free(cmd);
	free(inner_group);
	free(group_entry);
	free(pipeline_entry);
	printf("\n=== All tests completed successfully ===\n");
	return (0);
}
