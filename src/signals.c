/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 18:29:27 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/03 00:15:25 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

#include <signal.h>
#include <stdbool.h>
#include <stddef.h>

static volatile sig_atomic_t	g_sigint_pending = 0;

extern int	rl_done;

static void	signals_handle_sigint(int signum)
{
	if (signum == SIGINT)
	{
		rl_done = 1;
		g_sigint_pending = 1;
	}
}

static void	signals_sethandler(int signum, void (*handler)(int))
{
	struct sigaction	sa;

	sa = (struct sigaction){0};
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(signum, &sa, NULL);
}

void	signals_init(void)
{
	signals_sethandler(SIGQUIT, SIG_IGN);
	signals_sethandler(SIGINT, signals_handle_sigint);
}

void	signals_reset(void)
{
	signals_sethandler(SIGQUIT, SIG_DFL);
	signals_sethandler(SIGINT, SIG_DFL);
}

/*
	Check whether a SIGINT signal was received since the last check.

	We have to be careful here - we only reset the pending variable if we've
	determined that a signal was received and we're going to return true.
	Otherwise we might find no signal received, then the signal handler runs,
	and then we reset the variable and lose knowledge of that signal.
*/
bool	signals_accept_sigint(void)
{
	if (g_sigint_pending)
	{
		g_sigint_pending = 0;
		return (true);
	}
	return (false);
}
