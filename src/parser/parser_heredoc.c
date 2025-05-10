/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:40:32 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/10 22:42:03 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "libft.h"
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
static enum e_parser_status	add_line_to_heredoc(
	struct s_ast_command_word ***lines_append,
	char *line)
{
	struct s_ast_command_word	*new_line;

	new_line = malloc(sizeof(*new_line));
	if (!new_line)
		return (PARSER_ERR_MALLOC);
	new_line->word = line;
	new_line->next = NULL;
	**lines_append = new_line;
	*lines_append = &new_line->next;
	return (PARSER_SUCCESS);
}

/*
	Process a line for the heredoc
*/
static enum e_parser_status	process_heredoc_line(
	struct s_heredoc_params *params)
{
	enum e_parser_status	status;

	if (!params->quoted)
		params->line = word_heredoc_line(params->line);
	if (!params->line)
		return (PARSER_ERR_MALLOC);
	status = add_line_to_heredoc(&params->lines_append, params->line);
	if (status != PARSER_SUCCESS)
		free(params->line);
	return (status);
}

/*
	Read heredoc content from stdin until the delimiter is encountered.
	Store the content as a linked list of lines in the redirect node.
*/
enum e_parser_status	read_heredoc(struct s_ast_redirect *redirect)
{
	struct s_heredoc_params		params;
	enum e_parser_status		status;

	redirect->heredoc_lines = NULL;
	params.lines_append = &redirect->heredoc_lines;
	params.quoted = word_heredoc_delimiter(redirect->word);
	params.delimiter = redirect->word;
	params.delim_len = ft_strlen(redirect->word);
	status = PARSER_SUCCESS;
	while (status == PARSER_SUCCESS)
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
