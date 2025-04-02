/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:22:31 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/02 16:08:48 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include "ast.h"
#include "parser.h"
#include "tokenizer.h"

/*
	Parses a pipeline from the token list.
	Pipelines consist of simple commands separated by pipe state.
*/
enum e_parser_status	parser_pipeline(
	struct s_parser_state *state,
	struct s_ast_simple_command **pipeline_head)
{
	enum e_parser_status		status;
	struct s_ast_simple_command	**pipeline_append;

	pipeline_append = pipeline_head;
	while (1)
	{
		status = parser_simple_command(state, pipeline_append);
		if (status != PARSER_SUCCESS)
			return (status);
		if (state->curr_tok.type != TOK_PIPE)
			break ;
		parser_next_token(state);
		pipeline_append = &((*pipeline_append)->next);
	}
	return (PARSER_SUCCESS);
}
