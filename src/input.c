/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:49:06 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/06 20:03:33 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "signals.h"
#include "status.h"

static t_status	input_prepare(int *backup_fd)
{
	*backup_fd = dup(STDIN_FILENO);
	if (*backup_fd < 0)
		return (status_err(S_EXIT_ERR, "input: internal error",
				"dup() failed", errno));
	return (S_OK);
}

static t_status	input_restore(int *backup_fd)
{
	if (dup2(*backup_fd, STDIN_FILENO) < 0)
		return (status_err(S_EXIT_ERR, "input: internal error",
				"dup2() failed", errno));
	close(*backup_fd);
	return (S_OK);
}

t_status	input_get_line(char **line_out, const char *prompt)
{
	t_status	status;
	int			backup_fd;
	char		*line;
	bool		sigint;

	status = input_prepare(&backup_fd);
	if (status != S_OK)
		return (status);
	signals_set_input();
	errno = 0;
	line = readline(prompt);
	sigint = signals_check_caught();
	signals_set_normal();
	if (!line && !sigint && errno)
		return (status_err(S_EXIT_ERR, "input error",
				"readline() failed", errno));
	status = input_restore(&backup_fd);
	if (status == S_OK && sigint)
		status = S_RESET_SIGINT;
	if (status != S_OK)
		free(line);
	else
		*line_out = line;
	return (status);
}

void	input_add_history(const char *line)
{
	add_history(line);
}

void	input_clear_history(void)
{
	rl_clear_history();
}
