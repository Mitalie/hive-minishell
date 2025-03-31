/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_group.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:58:24 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 01:58:24 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "parser.h"
#include "ast.h"
#include <stdlib.h>

/*
	Helper for parse_group to finalize the creation of the group entry.
	Returns the ast list entry for the group, or NULL on failure.
*/
static struct s_ast_list_entry	*finalize_group(struct s_token **tokens,
										struct s_ast_list_entry *group_content)
{
	struct s_ast_list_entry	*group_entry;

	if (!(*tokens) || (*tokens)->type != TOK_GROUP_END)
	{
		print_syntax_error("missing closing parenthesis");
		free_list_entry(group_content);
		return (NULL);
	}
	(*tokens)++;
	group_entry = create_list_entry(AST_LIST_GROUP);
	if (!group_entry)
	{
		free_list_entry(group_content);
		return (NULL);
	}
	group_entry->group = group_content;
	return (group_entry);
}

/*
	Parses a group (commands inside parentheses) from the token list.
	Returns the ast list entry for the group, or NULL on failure.
*/
struct s_ast_list_entry	*parse_group(struct s_token **tokens)
{
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
	return (finalize_group(tokens, group_content));
}
