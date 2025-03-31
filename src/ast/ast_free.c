/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 14:22:09 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 19:03:15 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

#include <stdlib.h>

static void	free_command_word(struct s_ast_command_word *word);
static void	free_redirect(struct s_ast_redirect *redir);
static void	free_simple_command(struct s_ast_simple_command *cmd);
static void	free_list_entry(struct s_ast_list_entry *entry);

/*
	Frees the entire AST starting from the root.
	Takes a pointer to the root list entry.
*/
void	free_ast(struct s_ast_list_entry *root)
{
	free_list_entry(root);
}

static void	free_command_word(struct s_ast_command_word *word)
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

static void	free_redirect(struct s_ast_redirect *redir)
{
	struct s_ast_redirect	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->word);
		free(redir);
		redir = next;
	}
}

static void	free_simple_command(struct s_ast_simple_command *cmd)
{
	struct s_ast_simple_command	*next;

	while (cmd)
	{
		next = cmd->next;
		free_command_word(cmd->args);
		free_redirect(cmd->redirs);
		free(cmd);
		cmd = next;
	}
}

static void	free_list_entry(struct s_ast_list_entry *entry)
{
	struct s_ast_list_entry	*next;

	while (entry)
	{
		next = entry->next;
		if (entry->type == AST_LIST_GROUP)
			free_list_entry(entry->group);
		else if (entry->type == AST_LIST_PIPELINE)
			free_simple_command(entry->pipeline);
		free(entry);
		entry = next;
	}
}
