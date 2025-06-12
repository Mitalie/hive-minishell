/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_group.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:58:24 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/12 15:53:34 by amakinen         ###   ########.fr       */
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

	if (state->group_level >= 1000)
		return (parser_syntax_error("too many nested groups"));
	status = parser_next_token(state);
	if (status != S_OK)
		return (status);
	state->group_level++;
	status = parser_list(state, group_head);
	state->group_level--;
	if (status != S_OK)
		return (status);
	if (state->curr_tok.type != TOK_GROUP_END)
		return (parser_syntax_error("expected closing parenthesis"));
	return (parser_next_token(state));
}
