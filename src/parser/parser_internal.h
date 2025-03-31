/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 16:49:57 by amakinen          #+#    #+#             */
/*   Updated: 2025/03/31 20:47:48 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H

# include "parser.h"
# include "ast.h"
# include "tokenizer.h"

/* Command parsing functions */

enum e_parser_status	parser_word(
							struct s_token **tokens,
							struct s_ast_command_word **word);
enum e_parser_status	parser_redirect(
							struct s_token **tokens,
							struct s_ast_redirect **redirect);
enum e_parser_status	parser_simple_command(
							struct s_token **tokens,
							struct s_ast_simple_command **simple_command);
enum e_parser_status	parser_pipeline(
							struct s_token **tokens,
							struct s_ast_simple_command **pipeline_head);
enum e_parser_status	parser_list(
							struct s_token **tokens,
							struct s_ast_list_entry **list_head);
enum e_parser_status	parser_group(
							struct s_token **tokens,
							struct s_ast_list_entry **group_head);

/* Error handling helper */

void					parser_syntax_error(const char *message);

#endif
