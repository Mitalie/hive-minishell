/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:48:53 by amakinen          #+#    #+#             */
/*   Updated: 2025/02/24 16:57:54 by amakinen         ###   ########.fr       */
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
node. Variants are either specified with an enum value, except command_elem
whose variants are separated in the parent node.
*/

// no node for redirect_op, store the variant in redirect node
enum	e_ast_redirect_op
{
	AST_REDIR_IN,
	AST_REDIR_OUT,
	AST_REDIR_APP,
	AST_HEREDOC,
};

struct	s_ast_redirect
{
	enum e_ast_redirect_op	op;
	char					*word;
	struct s_ast_redirect	*next;
};

struct	s_ast_command_word
{
	char						*word;
	struct s_ast_command_word	*next;
};

// no node for command_elem, simple_command stores command_word and redirect
//  nodes directly in separate linked lists

struct	s_ast_simple_command
{
	struct s_ast_command_word	*args;
	struct s_ast_redirect		*redirs;
	struct s_ast_simple_command	*next;
};

// no node for pipeline, link directly to first simple_commmand node

// no node for list_op, store the variant in preceding list_entry node
enum	e_ast_list_op
{
	AST_LIST_AND,
	AST_LIST_OR,
};

// no node for group, link directly to first list_entry node of inner list

// list_entry must distinguish inner variant
enum	e_ast_list_entry_type
{
	AST_LIST_GROUP,
	AST_LIST_PIPELINE,
};

struct	s_ast_list_entry
{
	enum e_ast_list_entry_type		type;
	union
	{
		struct s_ast_list_entry		*group;
		struct s_ast_simple_command	*pipeline;
	};
	enum e_ast_list_op				next_op;
	struct s_ast_list_entry			*next;
};

// no node for list, link directly to first list_entry node

// no node for command, link directly to first list_entry node of top-level list

#endif
