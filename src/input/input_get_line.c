/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_get_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 20:25:17 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/05 22:12:05 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "input_internal.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <readline/readline.h>

#include "libft.h"
#include "signals.h"

/*
	The readline sets its own event handler for certain signals if they are not
	ignored by the application. Those signal handlers simply store the signal,
	which is then processed after the interrupted system call fails with EINTR.
	The processing involves calling the application signal handler, and then
	rl_signal_event_hook, before retrying for input.
*/

/*
	This function is called by readline (via rl_signal_event_hook) after it is
	interrupted by a signal. We check if our own handler determined the signal
	was SIGINT, and if it was, we move down a line, clear partial input and
	reset the prompt. Use printf for output here to ensure synchronization as
	readline internally writes using stdio.
*/
static int	input_readline_signal_hook(void)
{
	if (signals_check_sigint(false))
	{
		printf("\n");
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	return (0);
}

// static t_status	input_split_line(t_input *input, char **line_out)
// {
// 	char	*line_start;
// 	char	*line_end;

// 	line_start = input->input_pos;
// 	line_end = line_start;
// 	while (*line_end && *line_end != '\n')
// 		line_end++;
// 	if (!*line_end)
// 	{
// 		*line_out = ft_strdup(line_start);
// 		free(input->input_buf);
// 		input->input_buf = NULL;
// 		input->input_pos = NULL;
// 	}
// 	else
// 	{
// 		*line_end++ = '\0';
// 		input->input_pos = line_end;
// 		*line_out = ft_strdup(line_start);
// 	}
// 	if (!*line_out)
// 		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
// 	return (S_OK);
// }

// static t_status	input_store_line(t_input *input, char *line)
// {
// 	struct s_linelist	*list_entry;

// 	list_entry = malloc(sizeof(*list_entry));
// 	if (!list_entry)
// 		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
// 	list_entry->next = NULL;
// 	list_entry->line = line;
// 	list_entry->owned = true;
// 	*input->current_lines_append = list_entry;
// 	input->current_lines_append = &list_entry->next;
// 	return (S_OK);
// }

/*
	TODO: handle errors from readline?
*/
t_status	input_get_line(t_input *input, char **line_out)
{
	rl_signal_event_hook = input_readline_signal_hook;
	if (!input->input_buf && !input->input_eof)
	{
		input->input_buf = readline("PROMPT? ");
		if (!input->input_buf)
			input->input_eof = true;
		if (signals_check_sigint(true))
			return (S_RESET_SIGINT);
	}
	*line_out = input->input_buf;
	input->input_buf = NULL;
	return (S_OK);
}
