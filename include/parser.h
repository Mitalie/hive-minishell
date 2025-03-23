/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:50:10 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/23 14:32:09 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "ast.h"

/* Token type enumeration */
enum e_token
{
	TOK_WORD,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APP,
	TOK_HEREDOC,
	TOK_PIPE,
	TOK_AND,
	TOK_OR,
	TOK_GROUP_START,
	TOK_GROUP_END,
	TOK_END,
};

/* Token structure */
struct s_token
{
	enum e_token	type;
	char			*word_content;
};

/* Command parsing functions */
struct s_ast_simple_command	*parse_simple_command(struct s_token **tokens);
struct s_ast_simple_command	*process_command_elements(struct s_token **tokens,
								struct s_ast_simple_command *cmd,
								struct s_ast_command_word **args_append,
								struct s_ast_redirect **redirs_append);

/* Pipeline parsing functions */
struct s_ast_simple_command	*parse_pipeline(struct s_token **tokens);

/* List parsing functions */
struct s_ast_list_entry		*parse_list_entry(struct s_token **tokens);
struct s_ast_list_entry		*parse_list(struct s_token **tokens);
struct s_ast_list_entry		*parse_group(struct s_token **tokens);

/* Top-level parsing function */
struct s_ast_list_entry		*parse_command(struct s_token **tokens);

#endif /* PARSER_H */
