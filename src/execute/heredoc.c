/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:10:22 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/04 00:16:44 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "ast.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include "status.h"
#include <limits.h>

/*
	Create a temporary file
	Returns the file descriptor or -1 on error
*/
static int	create_tmpfile(char **filename)
{
	int		fd;
	char	temp_path[PATH_MAX];

	snprintf(temp_path, PATH_MAX, "/tmp/minishell_heredoc_%d_%d",
		getpid(), rand());
	*filename = ft_strdup(temp_path);
	if (!*filename)
		return (-1);
	fd = open(*filename, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(*filename);
		*filename = NULL;
		status_err(S_COMM_ERR, "Failed to create temporary file", NULL, 0);
	}
	return (fd);
}

/*
	Write heredoc lines to the temporary file
*/
static int	write_heredoc_lines(int fd, struct s_ast_command_word *lines)
{
	struct s_ast_command_word	*current;
	size_t						len;

	current = lines;
	while (current)
	{
		len = ft_strlen(current->word);
		if (write(fd, current->word, len) == -1)
		{
			status_err(S_COMM_ERR, "Failed to write to temporary file",
				NULL, 0);
			return (-1);
		}
		if (write(fd, "\n", 1) == -1)
		{
			status_err(S_COMM_ERR, "Failed to write to temporary file",
				NULL, 0);
			return (-1);
		}
		current = current->next;
	}
	return (0);
}

/*
	Reopen the temporary file for reading
	Returns the new file descriptor or -1 on error
*/
static int	reopen_tmpfile(int fd, char *filename)
{
	int	new_fd;

	close(fd);
	new_fd = open(filename, O_RDONLY);
	if (new_fd == -1)
		status_err(S_COMM_ERR, "Failed to reopen temporary file", NULL, 0);
	unlink(filename);
	free(filename);
	return (new_fd);
}

/*
	Process a heredoc redirection
	Creates a temporary file, writes the heredoc content, and sets up redirection
	Returns the file descriptor or -1 on error
*/
int	process_heredoc(struct s_ast_redirect *redirect)
{
	int		fd;
	int		read_fd;
	char	*filename;

	if (!redirect || !redirect->heredoc_lines)
	{
		status_err(S_COMM_ERR, "Invalid heredoc redirect", NULL, 0);
		return (-1);
	}
	fd = create_tmpfile(&filename);
	if (fd == -1)
		return (-1);
	if (write_heredoc_lines(fd, redirect->heredoc_lines) == -1)
	{
		close(fd);
		unlink(filename);
		free(filename);
		return (-1);
	}
	read_fd = reopen_tmpfile(fd, filename);
	return (read_fd);
}
