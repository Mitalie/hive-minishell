/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:58:09 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/02 17:42:56 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parser_internal.h"

#include "ast.h"
#include "tokenizer.h"

/*
	Top-level parsing function. Parses a complete command line.
	Stores the created AST in the pointer pointed to by `root`.
*/
enum e_parser_status	parser_parse(struct s_parser_state *state,
	struct s_ast_list_entry **root)
{
	enum e_parser_status	status;

	status = parser_list(state, root);
	if (status != PARSER_SUCCESS)
		return (status);
	if (state->curr_tok.type != TOK_END)
	{
		parser_syntax_error("unexpected token");
		return (PARSER_ERR_SYNTAX);
	}
	parser_next_token(state);
	return (PARSER_SUCCESS);
}
