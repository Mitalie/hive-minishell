/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:24:56 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/06 19:38:40 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include "signals_internal.h"

#include <signal.h>
#include <stddef.h>

static void	signals_set_handler(int signum, void handler(int))
{
	struct sigaction	sa;

	sa = (struct sigaction){0};
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(signum, &sa, NULL);
}

void	signals_set_normal(void)
{
	signals_set_handler(SIGINT, signals_handler_normal);
	signals_set_handler(SIGQUIT, SIG_IGN);
}

void	signals_set_input(void)
{
	signals_set_handler(SIGINT, signals_handler_input);
	signals_set_handler(SIGQUIT, SIG_IGN);
}

void	signals_reset(void)
{
	signals_set_handler(SIGINT, SIG_DFL);
	signals_set_handler(SIGQUIT, SIG_DFL);
}

/*
	Global variable written in signal handlers and read in the function below.
*/
volatile sig_atomic_t	g_signals_caught_signum;

/*
	Check if a handled signal has been received. The received status is reset.

	The status is only written if it was set in the first place to avoid the
	risk of losing a signal that arrives between the check and the write.
*/
bool	signals_check_caught(void)
{
	if (g_signals_caught_signum != 0)
	{
		g_signals_caught_signum = 0;
		return (true);
	}
	return (false);
}
