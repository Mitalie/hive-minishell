/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 20:45:38 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/05 21:05:10 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

#include <signal.h>
#include <stddef.h>

static volatile sig_atomic_t	g_sigint_pending = 0;

static void	signals_handler(int signum)
{
	if (signum == SIGINT)
		g_sigint_pending = 1;
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
	signals_sethandler(SIGINT, signals_handler);
}

void	signals_reset(void)
{
	signals_sethandler(SIGQUIT, SIG_DFL);
	signals_sethandler(SIGINT, SIG_DFL);
}

/*
	Careful with the function below - only reset `g_sigint_pending` if one was
	pending, otherwise we may lose a signal that arrives between the check and
	the write.
*/

/*
	Check if a SIGINT signal was received. Set `reset` to true to clear the
	received status until another one is received.
*/
bool	signals_check_sigint(bool reset)
{
	if (g_sigint_pending)
	{
		if (reset)
			g_sigint_pending = 0;
		return (true);
	}
	return (false);
}
