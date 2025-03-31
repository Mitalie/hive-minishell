/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:58:09 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 01:58:09 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "parser.h"
#include "ast.h"
#include <stdlib.h>

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
