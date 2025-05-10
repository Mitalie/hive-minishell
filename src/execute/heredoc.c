/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:10:22 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/10 22:38:00 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "execute.h"
#include "ast.h"

#include <fcntl.h>
#include <unistd.h>
#include "libft.h"
#include "status.h"
#include "util.h"

/*
	Create a temporary file using O_TMPFILE
	Returns the file descriptor or -1 on error
*/
static int	create_tmpfile(void)
{
	int	fd;

	fd = open("/tmp", O_TMPFILE | O_RDWR, 0600);
	if (fd == -1)
	{
		status_err(S_COMM_ERR, "Failed to create temporary file", NULL, 0);
		return (-1);
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
	Reopen the temporary file for reading using /proc/self/fd/
	Returns the new file descriptor or -1 on error
*/
static int	reopen_tmpfile(int fd)
{
	int		new_fd;
	char	proc_path[32];

	ft_strlcpy(proc_path, "/proc/self/fd/", 32);
	util_itoa_base(fd, "0123456789", proc_path + 14);
	new_fd = open(proc_path, O_RDONLY);
	if (new_fd == -1)
		status_err(S_COMM_ERR, "Failed to reopen temporary file", NULL, 0);
	close(fd);
	return (new_fd);
}

/*
	Process a heredoc redirection
	Creates a temporary file, writes the heredoc content, and sets up redirection
	Returns the file descriptor or -1 on error
*/
int	process_heredoc(struct s_ast_redirect *redirect)
{
	int	fd;
	int	read_fd;

	if (!redirect || !redirect->heredoc_lines)
	{
		status_err(S_COMM_ERR, "Invalid heredoc redirect", NULL, 0);
		return (-1);
	}
	fd = create_tmpfile();
	if (fd == -1)
		return (-1);
	if (write_heredoc_lines(fd, redirect->heredoc_lines) == -1)
	{
		close(fd);
		return (-1);
	}
	read_fd = reopen_tmpfile(fd);
	return (read_fd);
}
