/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:22:31 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 19:37:48 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include "ast.h"
#include "parser.h"
#include "tokenizer.h"

/*
	Parses a pipeline from the token list.
	Pipelines consist of simple commands separated by pipe tokens.
*/
enum e_parser_status	parse_pipeline(
	struct s_token **tokens,
	struct s_ast_simple_command **pipeline_head)
{
	enum e_parser_status		status;
	struct s_ast_simple_command	**pipeline_append;

	pipeline_append = pipeline_head;
	while (1)
	{
		status = parse_simple_command(tokens, pipeline_append);
		if (status != PARSER_SUCCESS)
			return (status);
		if ((*tokens)->type != TOK_PIPE)
			break ;
		(*tokens)++;
		pipeline_append = &((*pipeline_append)->next);
	}
	return (PARSER_SUCCESS);
}
