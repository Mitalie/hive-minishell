/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:58:09 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/07 16:57:24 by amakinen         ###   ########.fr       */
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
enum e_parser_status	parser_parse(char *line, struct s_ast_list_entry **root)
{
	enum e_parser_status	status;
	struct s_parser_state	state;

	state.tok_state.line_pos = line;
	parser_next_token(&state);
	*root = NULL;
	status = PARSER_SUCCESS;
	if (state.curr_tok.type != TOK_END)
		status = parser_list(&state, root);
	if (status == PARSER_SUCCESS && state.curr_tok.type != TOK_END)
	{
		parser_syntax_error("unexpected token");
		status = PARSER_ERR_SYNTAX;
	}
	return (status);
}
