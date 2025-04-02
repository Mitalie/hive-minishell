/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_group.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:58:24 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/02 16:08:48 by amakinen         ###   ########.fr       */
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
	struct s_parser_state *state,
	struct s_ast_list_entry **group_head)
{
	enum e_parser_status		status;

	parser_next_token(state);
	status = parser_list(state, group_head);
	if (status != PARSER_SUCCESS)
		return (status);
	if (state->curr_tok.type != TOK_GROUP_END)
	{
		parser_syntax_error("expected closing parenthesis");
		return (PARSER_ERR_SYNTAX);
	}
	parser_next_token(state);
	return (PARSER_SUCCESS);
}
