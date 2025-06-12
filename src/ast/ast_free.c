/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 14:22:09 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/12 23:56:04 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

#include <stdlib.h>

static void	ast_free_command_word(struct s_ast_command_word *word);
static void	ast_free_redirect(struct s_ast_redirect *redir);
static void	ast_free_simple_command(struct s_ast_simple_command *cmd);
static void	ast_free_list_entry(struct s_ast_list_entry *entry);

/*
	Frees the entire AST starting from the root.
	Takes a pointer to the root list entry.
*/
void	ast_free(struct s_ast_list_entry *root)
{
	ast_free_list_entry(root);
}

static void	ast_free_command_word(struct s_ast_command_word *word)
{
	struct s_ast_command_word	*next;

	while (word)
	{
		next = word->next;
		free(word->word);
		free(word);
		word = next;
	}
}

static void	ast_free_redirect(struct s_ast_redirect *redir)
{
	struct s_ast_redirect	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->word);
		if (redir->op == AST_HEREDOC && redir->heredoc_lines)
			ast_free_command_word(redir->heredoc_lines);
		free(redir);
		redir = next;
	}
}

static void	ast_free_simple_command(struct s_ast_simple_command *cmd)
{
	struct s_ast_simple_command	*next;

	while (cmd)
	{
		next = cmd->next;
		ast_free_command_word(cmd->args);
		ast_free_redirect(cmd->redirs);
		free(cmd);
		cmd = next;
	}
}

static void	ast_free_list_entry(struct s_ast_list_entry *entry)
{
	struct s_ast_list_entry	*next;

	while (entry)
	{
		next = entry->next;
		if (entry->type == AST_LIST_GROUP)
			ast_free_list_entry(entry->group);
		else if (entry->type == AST_LIST_PIPELINE)
			ast_free_simple_command(entry->pipeline);
		free(entry);
		entry = next;
	}
}
