/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:40:32 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/06 18:08:00 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include <stdbool.h>
#include <stdlib.h>

#include "ast.h"
#include "input.h"
#include "libft.h"
#include "status.h"
#include "word.h"

/*
	Check if a line matches the delimiter
	Returns true if the line is the delimiter
*/
static bool	is_delimiter(char *line, char *delimiter, size_t delim_len)
{
	if (!line)
		return (true);
	return (ft_strncmp(line, delimiter, delim_len + 1) == 0);
}

/*
	Add a line to the heredoc lines list
*/
static t_status	add_line_to_heredoc(
	struct s_ast_command_word ***lines_append,
	char *line)
{
	struct s_ast_command_word	*new_line;

	new_line = malloc(sizeof(*new_line));
	if (!new_line)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	new_line->word = line;
	new_line->next = NULL;
	**lines_append = new_line;
	*lines_append = &new_line->next;
	return (S_OK);
}

/*
	Read heredoc content from stdin until the delimiter is encountered.
	Store the content as a linked list of lines in the redirect node.
*/
t_status	read_heredoc(struct s_ast_redirect *redirect)
{
	t_status					status;
	struct s_ast_command_word	**lines_append;
	char						*delimiter;
	size_t						delim_len;
	char						*line;

	redirect->heredoc_lines = NULL;
	lines_append = &redirect->heredoc_lines;
	redirect->heredoc_quoted = word_heredoc_delimiter(redirect->word);
	delimiter = redirect->word;
	delim_len = ft_strlen(redirect->word);
	status = S_OK;
	while (status == S_OK)
	{
		status = input_get_line(&line, "> ");
		if (status != S_OK)
			return (status);
		if (is_delimiter(line, delimiter, delim_len))
			break ;
		status = add_line_to_heredoc(&lines_append, line);
	}
	free(line);
	return (status);
}
