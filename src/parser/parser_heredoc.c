/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:40:32 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/25 15:40:32 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"
#include <stdlib.h>

/*
** Add a heredoc node to the global heredoc list for later processing.
*/
enum e_parser_status	add_heredoc_to_list(
						struct s_parser_state *state,
						struct s_ast_redirect *redirect)
{
	struct s_heredoc_entry	*new_entry;

	redirect->fd = -1;
	new_entry = malloc(sizeof(*new_entry));
	if (!new_entry)
		return (PARSER_ERR_MALLOC);
	new_entry->redirect_node = redirect;
	new_entry->next = state->heredoc_list;
	state->heredoc_list = new_entry;
	return (PARSER_SUCCESS);
}

/*
** Free the heredoc list.
*/
void	free_heredoc_list(struct s_heredoc_entry *list)
{
	struct s_heredoc_entry	*next;

	while (list)
	{
		next = list->next;
		free(list);
		list = next;
	}
}
