/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:40:32 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/12 22:26:54 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

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
	Process a line for the heredoc
*/
static t_status	process_heredoc_line(
	struct s_heredoc_params *params)
{
	t_status	status;

	status = S_OK;
	if (!params->quoted)
		status = word_heredoc_line(&params->line);
	if (status == S_OK)
		status = add_line_to_heredoc(&params->lines_append, params->line);
	if (status != S_OK)
		free(params->line);
	return (status);
}

/*
	Read heredoc content from stdin until the delimiter is encountered.
	Store the content as a linked list of lines in the redirect node.
*/
t_status	read_heredoc(struct s_ast_redirect *redirect)
{
	struct s_heredoc_params	params;
	t_status				status;

	redirect->heredoc_lines = NULL;
	params.lines_append = &redirect->heredoc_lines;
	params.quoted = word_heredoc_delimiter(redirect->word);
	params.delimiter = redirect->word;
	params.delim_len = ft_strlen(redirect->word);
	status = S_OK;
	while (status == S_OK)
	{
		params.line = readline("> ");
		if (!params.line)
			break ;
		if (is_delimiter(params.line, params.delimiter, params.delim_len))
		{
			free(params.line);
			break ;
		}
		status = process_heredoc_line(&params);
	}
	return (status);
}
