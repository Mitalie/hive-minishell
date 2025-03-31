/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 14:22:36 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 20:47:49 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include <stdlib.h>

#include "ast.h"
#include "parser.h"
#include "tokenizer.h"

/*
	Parses a single list entry from the token list.
	A list entry can be either a pipeline or a group.
*/
static enum e_parser_status	parser_list_entry(
	struct s_token **tokens,
	struct s_ast_list_entry **list_append)
{
	enum e_parser_status		status;
	struct s_ast_list_entry		*new_entry;

	new_entry = malloc(sizeof(*new_entry));
	if (!new_entry)
		return (PARSER_ERR_MALLOC);
	*list_append = new_entry;
	new_entry->next = NULL;
	if ((*tokens)->type == TOK_GROUP_START)
	{
		new_entry->type = AST_LIST_GROUP;
		new_entry->group = NULL;
		status = parser_group(tokens, &(new_entry->group));
	}
	else
	{
		new_entry->type = AST_LIST_PIPELINE;
		new_entry->pipeline = NULL;
		status = parser_pipeline(tokens, &(new_entry->pipeline));
	}
	return (status);
}

/*
	Parses a list of commands separated by && or || operators.
*/
enum e_parser_status	parser_list(
	struct s_token **tokens,
	struct s_ast_list_entry **list_head)
{
	enum e_parser_status	status;
	struct s_ast_list_entry	**list_append;

	list_append = list_head;
	while (1)
	{
		status = parser_list_entry(tokens, list_append);
		if (status != PARSER_SUCCESS)
			return (status);
		if ((*tokens)->type == TOK_AND)
			(*list_append)->next_op = AST_LIST_AND;
		else if ((*tokens)->type == TOK_OR)
			(*list_append)->next_op = AST_LIST_OR;
		else
			break ;
		(*tokens)++;
		list_append = &((*list_append)->next);
	}
	return (PARSER_SUCCESS);
}
