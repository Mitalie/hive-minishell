/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_group.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:58:24 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/12 21:36:54 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include "ast.h"
#include "status.h"
#include "tokenizer.h"

/*
	Parses a group (commands inside parentheses) from the token list.
	Next token must be TOK_GROUP_START.
*/
t_status	parser_group(
	struct s_parser_state *state,
	struct s_ast_list_entry **group_head)
{
	t_status	status;

	parser_next_token(state);
	status = parser_list(state, group_head);
	if (status != S_OK)
		return (status);
	if (state->curr_tok.type != TOK_GROUP_END)
	{
		parser_syntax_error("expected closing parenthesis");
		return (S_RESET_SYNTAX);
	}
	parser_next_token(state);
	return (S_OK);
}
