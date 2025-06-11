/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   die_with_sig.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:30:24 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/11 16:58:40 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft.h"
#include "status.h"
#include "util.h"

/*
	If the shell receives a SIGINT after forking children, it checks whether the
	children died with SIGINT. If not, the shell assumes the child handled the
	signal as part of its normal operation and doesn't abort a list of commands.

	A child process should not handle signals unless an external command sets
	its own signal handlers, but it's possible we catch one just before fork, or
	after fork but before clearing signal handlers.
	
	Ideally, we'd block signals from before fork until we clear signal handlers.
	But `sigprocmask()` is not allowed. As a workaround, resend the signal that
	was caught after clearing signal handlers. But `raise()` is not allowed. As
	a workaround, we can send it to ourselves with `kill()`. But `getpid()` is
	not allowed. As a workaround, read PID from `/proc/self/stat`. We assume
	that proc is available and that `pid_t` fits in an int. 

	This function will not return and should only be called after any cleanup
	has been performed.
*/
void	die_with_sig(int signum)
{
	int		fd;
	char	buf[13];
	size_t	len;
	int		pid;
	bool	got_pid;

	got_pid = false;
	ft_memset(buf, 0, sizeof(buf));
	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		status_warn("failed to die with signal", "open() failed", errno);
	else if (read(fd, buf, sizeof(buf) - 1) >= 0)
		status_warn("failed to die with signal", "read() failed", errno);
	else if (!util_parse_int(buf, &len, &pid) || buf[len] != ' ' || pid <= 0)
		status_warn("failed to die with signal", "invalid pid read", 0);
	else
		got_pid = true;
	if (fd >= 0)
		close(fd);
	if (got_pid)
		kill(pid, signum);
	if (got_pid)
		status_warn("failed to die with signal", "kill() failed", errno);
	exit(1);
}
