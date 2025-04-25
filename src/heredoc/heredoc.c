/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:59:31 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/25 18:55:53 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_internal.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include <readline/readline.h>

/*
** Read heredoc content from stdin until the delimiter is encountered.
** Write content to a temporary file and store the file descriptor in the node.
*/
enum e_parser_status	read_heredoc(struct s_ast_redirect *redirect)
{
	char	*line;
	int		fd;
	char	*tmp_filename;

	fd = create_heredoc_file(&tmp_filename);
	if (fd == -1)
		return (PARSER_ERR_HEREDOC);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (handle_heredoc_line(fd, line, redirect->word))
			break ;
	}
	close(fd);
	fd = reopen_heredoc_file(tmp_filename);
	if (fd == -1)
		return (PARSER_ERR_HEREDOC);
	redirect->fd = fd;
	return (PARSER_SUCCESS);
}
