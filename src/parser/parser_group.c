/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_group.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:58:24 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 20:47:46 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include "ast.h"
#include "parser.h"
#include "tokenizer.h"

/*
	Parses a group (commands inside parentheses) from the token list.
	Next token must be TOK_GROUP_START.
*/
enum e_parser_status	parser_group(
	struct s_token **tokens,
	struct s_ast_list_entry **group_head)
{
	enum e_parser_status		status;

	(*tokens)++;
	status = parser_list(tokens, group_head);
	if (status != PARSER_SUCCESS)
		return (status);
	if ((*tokens)->type != TOK_GROUP_END)
	{
		parser_syntax_error("expected closing parenthesis");
		return (PARSER_ERR_SYNTAX);
	}
	(*tokens)++;
	return (PARSER_SUCCESS);
}
