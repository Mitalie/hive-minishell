/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:50:10 by josmanov          #+#    #+#             */
/*   Updated: 2025/02/27 19:19:24 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

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

struct s_token
{
    enum e_token type;
    char *word_content;
};

struct s_ast_simple_command *parse_simple_command(struct s_token **tokens);

#endif // PARSER_H
