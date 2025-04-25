/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:44:04 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/25 15:56:19 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "heredoc_internal.h"
#include "../parser/parser_internal.h"

/*
** Process all heredocs in the heredoc list.
** This function serves as a bridge between the parser and heredoc modules.
*/
enum e_parser_status	process_heredocs(struct s_heredoc_entry *heredoc_list)
{
	struct s_heredoc_entry	*current;
	enum e_parser_status	status;

	status = PARSER_SUCCESS;
	current = heredoc_list;
	while (current && status == PARSER_SUCCESS)
	{
		status = read_heredoc(current->redirect_node);
		current = current->next;
	}
	return (status);
}
