/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 16:49:57 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/18 18:23:17 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H

# include "ast.h"
# include "input.h"
# include "status.h"
# include "tokenizer.h"

struct	s_parser_state
{
	t_input						*input;
	struct s_token				curr_tok;
	struct s_tokenizer_state	tok_state;
	int							group_level;
};

t_status	parser_next_token(struct s_parser_state *state);

/* Command parsing functions */

t_status	parser_word(
				struct s_parser_state *state,
				struct s_ast_command_word **word);
t_status	parser_redirect(
				struct s_parser_state *state,
				struct s_ast_redirect **redirect);
t_status	parser_simple_command(
				struct s_parser_state *state,
				struct s_ast_simple_command **simple_command);
t_status	parser_pipeline(
				struct s_parser_state *state,
				struct s_ast_simple_command **pipeline_head);
t_status	parser_list(
				struct s_parser_state *state,
				struct s_ast_list_entry **list_head);
t_status	parser_group(
				struct s_parser_state *state,
				struct s_ast_list_entry **group_head);

t_status	parser_read_heredoc(
				struct s_parser_state *state,
				struct s_ast_redirect *redirect);

/* Error handling helper */

t_status	parser_syntax_error(const char *message);

#endif
