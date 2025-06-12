/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:22:31 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/11 22:03:35 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include "ast.h"
#include "status.h"
#include "tokenizer.h"

/*
	Parses a pipeline from the token list.
	Pipelines consist of simple commands separated by pipe state.
*/
t_status	parser_pipeline(
	struct s_parser_state *state,
	struct s_ast_simple_command **pipeline_head)
{
	t_status					status;
	struct s_ast_simple_command	**pipeline_append;

	pipeline_append = pipeline_head;
	while (1)
	{
		status = parser_simple_command(state, pipeline_append);
		if (status != S_OK)
			return (status);
		if (state->curr_tok.type != TOK_PIPE)
			break ;
		status = parser_next_token(state);
		if (status != S_OK)
			return (status);
		pipeline_append = &((*pipeline_append)->next);
	}
	return (S_OK);
}
