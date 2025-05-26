/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:10:22 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/26 22:45:22 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "execute.h"
#include "ast.h"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "libft.h"
#include "status.h"
#include "util.h"

#define TMP_FILE_PATH "/tmp"

/*
	Create a temporary file using O_TMPFILE.
	Reopen it for reading using /proc/self/fd/.
	On error, no file descriptors are left open.

	The reopen is necessary as writing the heredoc data into the file leaves the
	file offset at the end of the file, so programs that attempt to read from it
	would not read anything and think they've reached EOF. The project doesn't
	allow using lseek to reset the offset, so reopen is the only option.

	O_TMPFILE and /proc/self/fd are Linux-specific, and the latter requires
	procfs being mounted at /proc. If these aren't available, we could use a
	named temporary file or a pipe (with a child process for writes, in case
	data is larger than the pipe buffer).
*/
static t_status	execute_heredoc_create(int *writefd_out, int *readfd_out)
{
	int		writefd;
	int		readfd;
	char	proc_path[32];

	writefd = open(TMP_FILE_PATH, O_TMPFILE | O_WRONLY, 0600);
	if (writefd == -1)
		return (status_err(S_COMM_ERR, "execute_heredoc",
				"Failed to create temporary file", errno));
	ft_strlcpy(proc_path, "/proc/self/fd/", 32);
	util_itoa_base(writefd, "0123456789", proc_path + 14);
	readfd = open(proc_path, O_RDONLY);
	if (readfd == -1)
	{
		close(writefd);
		return (status_err(S_COMM_ERR, "execute_heredoc",
				"Failed to reopen temporary file", errno));
	}
	*writefd_out = writefd;
	*readfd_out = readfd;
	return (S_OK);
}

/*
	Write heredoc lines to the temporary file.

	Write doesn't need a null terminator, so we can insert the newline in its
	place and not need a separate write call. Put the terminator back afterwards
	in case some other code tries to process the string after this function.
*/
static t_status	execute_heredoc_write(int fd, struct s_ast_command_word *lines)
{
	struct s_ast_command_word	*current;
	size_t						len;

	current = lines;
	while (current)
	{
		len = ft_strlen(current->word);
		current->word[len] = '\n';
		if (!util_write_all(fd, current->word, len + 1))
		{
			return (status_err(S_COMM_ERR, "execute_heredoc",
					"Failed to write to temporary file", errno));
		}
		current->word[len] = '\0';
		current = current->next;
	}
	return (S_OK);
}

/*
	Process a heredoc redirection
	Creates a temporary file, writes the heredoc content, and sets up redirection
	Returns the file descriptor or -1 on error
*/
t_status	execute_prepare_heredoc(struct s_ast_redirect *redirect, int *fd_out)
{
	t_status	status;
	int			writefd;
	int			readfd;

	status = execute_heredoc_create(&writefd, &readfd);
	if (status != S_OK)
		return (status);
	status = execute_heredoc_write(writefd, redirect->heredoc_lines);
	if (close(writefd) < 0 && status == S_OK)
		status = status_err(S_COMM_ERR, "execute_heredoc",
				"Error closing the heredoc after writing", errno);
	if (status == S_OK)
		*fd_out = readfd;
	return (status);
}
