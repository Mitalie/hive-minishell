/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:40:32 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/03 00:40:32 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"
#include <stdlib.h>
#include "libft.h"
#include "word.h"
#include <readline/readline.h>

/*
	Check if a line matches the delimiter
*/
static bool	is_delimiter(char *line, char *delimiter, size_t delim_len)
{
	if (!line)
		return (true);
	if (ft_strncmp(line, delimiter, delim_len) == 0 && line[delim_len] == '\0')
		return (false);
	return (0);
}

/*
	Add a line to the heredoc lines list
*/
static enum e_parser_status	add_line_to_heredoc(
	struct s_ast_command_word **lines_append,
	char *line)
{
	struct s_ast_command_word	*new_line;

	new_line = malloc(sizeof(*new_line));
	if (!new_line)
		return (PARSER_ERR_MALLOC);
	new_line->word = line;
	new_line->next = NULL;
	*lines_append = new_line;
	return (PARSER_SUCCESS);
}

/*
	Process a line for the heredoc
*/
static enum e_parser_status	process_heredoc_line(
	struct s_ast_command_word **lines_append,
	char *line,
	char *delimiter,
	int quoted)
{
	enum e_parser_status	status;
	size_t					delim_len;

	delim_len = ft_strlen(delimiter);
	if (is_delimiter(line, delimiter, delim_len))
	{
		free(line);
		return (PARSER_SUCCESS);
	}
	if (!quoted)
		line = word_heredoc_line(line);
	if (!line)
		return (PARSER_ERR_MALLOC);
	status = add_line_to_heredoc(lines_append, line);
	return (status);
}

/*
	Read heredoc content from stdin until the delimiter is encountered.
	Store the content as a linked list of lines in the redirect node.
*/
enum e_parser_status	read_heredoc(struct s_ast_redirect *redirect)
{
	char						*line;
	struct s_ast_command_word	**lines_append;
	enum e_parser_status		status;
	int							quoted;

	redirect->heredoc_lines = NULL;
	lines_append = &redirect->heredoc_lines;
	quoted = word_heredoc_delimiter(redirect->word);
	status = PARSER_SUCCESS;
	while (status == PARSER_SUCCESS)
	{
		line = readline("> ");
		status = process_heredoc_line(lines_append, line,
				redirect->word, quoted);
		if (status == PARSER_SUCCESS && !line)
			break ;
		if (status == PARSER_SUCCESS)
			lines_append = &(*lines_append)->next;
	}
	return (status);
}
