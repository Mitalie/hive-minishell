/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:58:09 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/02 17:56:55 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parser_internal.h"

#include <stddef.h>

#include "ast.h"
#include "tokenizer.h"

void	parser_next_token(struct s_parser_state *state)
{
	state->curr_tok = tokenizer_get_next(&state->tok_state);
}

/*
	Top-level parsing function. Parses a complete command line.
	Stores the created AST in the pointer pointed to by `root`.
*/
enum e_parser_status	parser_parse(struct s_ast_list_entry **root)
{
	enum e_parser_status	status;
	struct s_parser_state	state;

	state.tok_state.line = NULL;
	state.tok_state.eof_reached = false;
	parser_next_token(&state);
	if (state.curr_tok.type == TOK_END)
	{
		*root = NULL;
		return (PARSER_SUCCESS);
	}
	status = parser_list(&state, root);
	if (status != PARSER_SUCCESS)
		return (status);
	if (state.curr_tok.type != TOK_END)
	{
		parser_syntax_error("unexpected token");
		return (PARSER_ERR_SYNTAX);
	}
	return (PARSER_SUCCESS);
}
