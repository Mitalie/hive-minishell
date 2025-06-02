/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:14:46 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/02 23:58:21 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

#include <stdio.h>
#include <readline/readline.h>

#include "signals.h"

static int	input_hook_sigint(void)
{
	if (signals_accept_sigint())
		rl_done = 1;
	return (0);
}

static int	input_hook_empty(void)
{
	return (0);
}

char	*input_get_line(const char *prompt)
{
	char	*line;

	rl_event_hook = input_hook_empty;
	rl_signal_event_hook = input_hook_sigint;
	line = readline(prompt);
	return (line);
}
