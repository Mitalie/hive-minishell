/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:42:17 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/06 19:16:22 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals_internal.h"

#include <unistd.h>

void	signals_handler_normal(int signum)
{
	g_signals_caught_signum = signum;
}

/*
	Close standard input to force readline to fail. The standard input file
	descriptor must be duplicated to safety while this signal handler is in use,
	and restored after switching to normal signal handler.
*/
void	signals_handler_input(int signum)
{
	g_signals_caught_signum = signum;
	close(STDIN_FILENO);
}
