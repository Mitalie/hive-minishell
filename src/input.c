/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:49:06 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/13 19:21:48 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "signals.h"
#include "status.h"

/*
	We want to always abort input and return to main loop if a SIGINT is caught.
	If we just check for caught SIGINT before the readline() call, a signal
	could arrive after the check but before Readline clears rl_done, allowing
	readline() to wait for input instead of returning immediately. Instead check
	in a callback that runs after rl_done has been cleared.
*/
static int	input_rl_startup_hook(void)
{
	if (signals_check_sigint(false))
		rl_done = 1;
	return (0);
}

/*
	We don't need to do anything in the event hook, but we need to set one to
	make Readline run in a mode that checks rl_done instead of just retrying on
	EINTR caused by SIGINT.
*/
static int	input_rl_event_hook(void)
{
	return (0);
}

t_status	input_get_line(char **line_out, const char *prompt)
{
	char	*line;

	if (!isatty(STDIN_FILENO))
		return (input_notty_get_line(line_out, prompt));
	rl_startup_hook = input_rl_startup_hook;
	rl_event_hook = input_rl_event_hook;
	errno = 0;
	line = readline(prompt);
	if (signals_check_sigint(true))
	{
		free(line);
		return (S_RESET_SIGINT);
	}
	if (!line && errno)
		return (status_err(S_EXIT_ERR, "input error",
				"readline() failed", errno));
	*line_out = line;
	return (S_OK);
}

void	input_add_history(const char *line)
{
	if (!isatty(STDIN_FILENO))
		add_history(line);
}

void	input_clear_history(void)
{
	rl_clear_history();
}
