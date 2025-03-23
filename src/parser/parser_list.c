/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 14:22:36 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/23 14:55:25 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "parser.h"
#include "ast.h"
#include <stdlib.h>

/*
	Parses a group (commands inside parentheses) from the token list.
	Returns the ast list entry for the group, or NULL on failure.
*/
struct s_ast_list_entry	*parse_group(struct s_token **tokens)
{
	struct s_ast_list_entry	*group_entry;
	struct s_ast_list_entry	*group_content;

	if (!tokens || !(*tokens) || (*tokens)->type != TOK_GROUP_START)
		return (NULL);
	(*tokens)++;
	if (!(*tokens) || (*tokens)->type == TOK_END)
	{
		print_syntax_error("unexpected end after group start");
		return (NULL);
	}
	group_content = parse_list(tokens);
	if (!group_content)
		return (NULL);
	if (!(*tokens) || (*tokens)->type != TOK_GROUP_END)
	{
		print_syntax_error("missing closing parenthesis");
		return (NULL);
	}
	(*tokens)++; /* Skip the closing parenthesis */
	group_entry = create_list_entry(AST_LIST_GROUP);
	if (!group_entry)
		return (NULL);
	if (!set_list_entry_group(group_entry, group_content))
		return (NULL);
	return (group_entry);
}

/*
	Parses a single list entry from the token list.
	A list entry can be either a pipeline or a group (commands in parentheses).
	Returns the AST list entry, or NULL on failure.
*/
struct s_ast_list_entry	*parse_list_entry(struct s_token **tokens)
{
	struct s_ast_list_entry		*entry;
	struct s_ast_simple_command	*pipeline;

	if (!tokens || !(*tokens))
		return (NULL);
	if ((*tokens)->type == TOK_GROUP_START)
		return (parse_group(tokens));
	pipeline = parse_pipeline(tokens);
	if (!pipeline)
		return (NULL);
	entry = create_list_entry(AST_LIST_PIPELINE);
	if (!entry)
	{
		free_simple_command(pipeline);
		return (NULL);
	}
	if (!set_list_entry_pipeline(entry, pipeline))
	{
		free_simple_command(pipeline);
		free_list_entry(entry);
		return (NULL);
	}
	return (entry);
}

/*
	Parses a list of commands separated by && or || operators.
	Returns the AST list entry for the first command, or NULL on failure.
*/
struct s_ast_list_entry	*parse_list(struct s_token **tokens)
{
	struct s_ast_list_entry	*first_entry;
	struct s_ast_list_entry	*current_entry;
	struct s_ast_list_entry	**next_entry_ptr;

	if (!tokens || !(*tokens))
		return (NULL);
	first_entry = parse_list_entry(tokens);
	if (!first_entry)
		return (NULL);
	next_entry_ptr = &(first_entry->next);
	while (*tokens && ((*tokens)->type == TOK_AND || (*tokens)->type == TOK_OR))
	{
		if ((*tokens)->type == TOK_AND)
			first_entry->next_op = AST_LIST_AND;
		else
			first_entry->next_op = AST_LIST_OR;
		(*tokens)++;
		if (!(*tokens) || (*tokens)->type == TOK_END)
		{
			print_syntax_error("unexpected end after logical operator");
			free_list_entry(first_entry);
			return (NULL);
		}
		current_entry = parse_list_entry(tokens);
		if (!current_entry)
		{
			free_list_entry(first_entry);
			return (NULL);
		}
		*next_entry_ptr = current_entry;
		next_entry_ptr = &(current_entry->next);
	}
	return (first_entry);
}

/*
	Top-level parsing function. Parses a complete command line.
	Returns the root of the AST, or NULL on failure.
*/
struct s_ast_list_entry	*parse_command(struct s_token **tokens)
{
	struct s_ast_list_entry	*root;

	if (!tokens || !(*tokens) || (*tokens)->type == TOK_END)
		return (NULL);
	root = parse_list(tokens);
	if (!root)
		return (NULL);
	if (*tokens && (*tokens)->type != TOK_END)
	{
		print_syntax_error("syntax error near unexpected token");
		free_list_entry(root);
		return (NULL);
	}
	return (root);
}
