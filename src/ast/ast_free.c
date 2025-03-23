/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 14:22:09 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/23 14:31:52 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

/*
	Frees memory allocated for a command word and all words in its chain.
	Takes a pointer to a command word structure.
*/
void	free_command_word(struct s_ast_command_word *word)
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

/*
	Frees memory allocated for a redirect and all redirects in its chain.
	Takes a pointer to a redirect structure.
*/
void	free_redirect(struct s_ast_redirect *redir)
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

/*
	Frees memory allocated for a simple command and all commands in its chain.
	Takes a pointer to a simple command structure.
*/
void	free_simple_command(struct s_ast_simple_command *cmd)
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

/*
	Frees memory allocated for a list entry and all entries in its chain.
	Takes a pointer to a list entry structure.
*/
void	free_list_entry(struct s_ast_list_entry *entry)
{
	struct s_ast_list_entry	*next;

	while (entry)
	{
		next = entry->next;
		if (entry->type == AST_LIST_GROUP && entry->group)
		{
			free_list_entry(entry->group);
			entry->group = NULL; // Prevent double free
		}
		else if (entry->type == AST_LIST_PIPELINE && entry->pipeline)
		{
			free_simple_command(entry->pipeline);
			entry->pipeline = NULL; // Not strictly necessary but consistent
		}
		free(entry);
		entry = next;
	}
}

/*
	Frees the entire AST starting from the root.
	Takes a pointer to the root list entry.
*/
void	free_ast(struct s_ast_list_entry *root)
{
	free_list_entry(root);
}
