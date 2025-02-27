/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:07:51 by josmanov          #+#    #+#             */
/*   Updated: 2025/02/27 19:19:03 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef AST_H
#define AST_H

enum e_ast_redirect_op
{
    AST_REDIR_IN,
    AST_REDIR_OUT,
    AST_REDIR_APP,
    AST_HEREDOC,
};

struct s_ast_redirect
{
    enum e_ast_redirect_op op;
    char *word;
    struct s_ast_redirect *next;
};

struct s_ast_command_word
{
    char *word;
    struct s_ast_command_word *next;
};

struct s_ast_simple_command
{
    struct s_ast_command_word *args;
    struct s_ast_redirect *redirs;
    struct s_ast_simple_command *next;
};

enum e_ast_list_op
{
    AST_LIST_AND,
    AST_LIST_OR,
};

enum e_ast_list_entry_type
{
    AST_LIST_GROUP,
    AST_LIST_PIPELINE,
};

struct s_ast_list_entry
{
    enum e_ast_list_entry_type type;
    union
    {
        struct s_ast_list_entry *group;
        struct s_ast_simple_command *pipeline;
    };
    enum e_ast_list_op next_op;
    struct s_ast_list_entry *next;
};

// Function prototypes added here
struct s_ast_command_word *create_command_word(char *word);
struct s_ast_redirect *create_redirect(enum e_ast_redirect_op op, char *word);

#endif // AST_H
