/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 11:22:25 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/02 16:05:41 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include <stdlib.h>

#include "ast.h"
#include "parser.h"

/*
	Parses a word and adds it to the AST.
	Next token must be TOK_WORD.
*/
enum e_parser_status	parser_word(
	struct s_parser_state *state,
	struct s_ast_command_word **word)
{
	struct s_ast_command_word	*new_word;

	new_word = malloc(sizeof(*new_word));
	if (!new_word)
		return (PARSER_ERR_MALLOC);
	*word = new_word;
	new_word->next = NULL;
	new_word->word = state->curr_tok.word_content;
	parser_next_token(state);
	return (PARSER_SUCCESS);
}
