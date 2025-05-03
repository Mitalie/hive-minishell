/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:07:51 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/02 23:56:50 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

/*
AST for the following grammar:

	command = list END
	list = list_entry (list_op list_entry)*
	list_entry = group | pipeline
	group = GROUP_START list GROUP_END
	list_op = AND | OR
	pipeline = simple_command (PIPE simple_command)*
	simple_command = command_elem command_elem*
	command_elem = command_word | redirect
	command_word = WORD
	redirect = redirect_op WORD
	redirect_op = REDIR_IN | REDIR_OUT | REDIR_APP | HEREDOC

Repetitions are stored in linked lists, with a next-pointer in each repeatable
node. Variants are specified with an enum value, except command_elem whose
variants are separated in the parent node.

Pipeline and list don't generate separate nodes, as their parent can point
directly at the first element of the linked list. Command and group don't
generate separate nodes as they only contain one child and no additional
information, so the parent can point directly at that child. Command_elem
node is eliminated by having simple_command store separate lists for
command_word and redirect.
*/

enum e_ast_redirect_op
{
	AST_REDIR_IN,
	AST_REDIR_OUT,
	AST_REDIR_APP,
	AST_HEREDOC,
};

struct s_ast_redirect
{
	enum e_ast_redirect_op		op;
	char						*word;
	int							fd;
	struct s_ast_command_word	*heredoc_lines;
	struct s_ast_redirect		*next;
};

struct s_ast_command_word
{
	char						*word;
	struct s_ast_command_word	*next;
};

struct s_ast_simple_command
{
	struct s_ast_command_word	*args;
	struct s_ast_redirect		*redirs;
	struct s_ast_simple_command	*next;
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
	enum e_ast_list_entry_type	type;
	union
	{
		struct s_ast_list_entry		*group;
		struct s_ast_simple_command	*pipeline;
	};
	enum e_ast_list_op			next_op;
	struct s_ast_list_entry		*next;
};

/*
	Frees the entire AST starting from the root.
	Takes a pointer to the root list entry.
*/
void	free_ast(struct s_ast_list_entry *root);

#endif
