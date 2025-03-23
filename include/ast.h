/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:07:51 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/23 14:32:08 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

/* Redirection type enumeration */
enum e_ast_redirect_op
{
	AST_REDIR_IN,
	AST_REDIR_OUT,
	AST_REDIR_APP,
	AST_HEREDOC,
};

/* Redirection node structure */
struct s_ast_redirect
{
	enum e_ast_redirect_op	op;
	char					*word;
	struct s_ast_redirect	*next;
};

/* Command word node structure */
struct s_ast_command_word
{
	char						*word;
	struct s_ast_command_word	*next;
};

/* Simple command node structure */
struct s_ast_simple_command
{
	struct s_ast_command_word	*args;
	struct s_ast_redirect		*redirs;
	struct s_ast_simple_command	*next;
};

/* List operation type enumeration */
enum e_ast_list_op
{
	AST_LIST_AND,
	AST_LIST_OR,
};

/* List entry type enumeration */
enum e_ast_list_entry_type
{
	AST_LIST_GROUP,
	AST_LIST_PIPELINE,
};

/* List entry node structure */
struct s_ast_list_entry
{
	enum e_ast_list_entry_type	type;
	union
	{
		struct s_ast_list_entry		*group;
		struct s_ast_simple_command	*pipeline;
	};
	enum e_ast_list_op			next_op;
	struct s_ast_list_entry		*next;
};

/* Command and word creation functions */
struct s_ast_command_word	*create_command_word(char *word);
struct s_ast_simple_command	*create_simple_command(void);

/* Redirect node functions */
struct s_ast_redirect		*create_redirect(
								enum e_ast_redirect_op op, char *word);

/* List entry functions */
struct s_ast_list_entry		*create_list_entry(enum e_ast_list_entry_type type);
int							set_list_entry_group(struct s_ast_list_entry *entry,
								struct s_ast_list_entry *group);
int							set_list_entry_pipeline(
								struct s_ast_list_entry *entry,
								struct s_ast_simple_command *pipeline);

/* Memory cleanup functions */
void						free_command_word(struct s_ast_command_word *word);
void						free_redirect(struct s_ast_redirect *redir);
void						free_simple_command(
								struct s_ast_simple_command *cmd);
void						free_list_entry(struct s_ast_list_entry *entry);
void						free_ast(struct s_ast_list_entry *root);

#endif /* AST_H */
