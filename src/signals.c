/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:24:56 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/09 08:37:56 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

#include <signal.h>
#include <stddef.h>
#include <stdio.h>

#include <readline/readline.h>

static volatile sig_atomic_t	g_signals_caught_signum = 0;

/*
	Store the caught signal number.

	Also set rl_done to break out of a readline() call. Readline must be in
	event hook mode (rl_event_hook set) or it won't check rl_done.
*/
static void	signals_sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signals_caught_signum = signum;
		rl_done = 1;
	}
}

/*
	Use SA_RESTART to avoid needing to handle EINTR everywhere. Readline sets
	its own signal handlers so this doesn't prevent interrupting readline().
*/
static void	signals_set_handler(int signum, void handler(int))
{
	struct sigaction	sa;

	sa = (struct sigaction){0};
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sa.sa_flags = SA_RESTART;
	sigaction(signum, &sa, NULL);
}

/*
	Set the applicable signal handlers for the shell.
*/
void	signals_set_handlers(void)
{
	signals_set_handler(SIGINT, signals_sigint_handler);
	signals_set_handler(SIGQUIT, SIG_IGN);
}

/*
	Restore signal handling to default (for child processes).
*/
void	signals_clear_handlers(void)
{
	signals_set_handler(SIGINT, SIG_DFL);
	signals_set_handler(SIGQUIT, SIG_DFL);
}

/*
	Check if a SIGINT signal has been caught. If reset is true, the caught
	status is reset.

	The status is only written if it was set in the first place to avoid the
	risk of losing a signal that arrives between the check and the write.
*/
bool	signals_check_sigint(bool reset)
{
	if (g_signals_caught_signum == SIGINT)
	{
		if (reset)
			g_signals_caught_signum = 0;
		return (true);
	}
	return (false);
}
