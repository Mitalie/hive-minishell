/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:33:08 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/18 19:00:10 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "input_internal.h"

#include <stdlib.h>
#include <unistd.h>

#include "libft.h"

/*
	Split a line from the potentially multiline input block stored in the state.

	If there are no newline characters and we're at start of the block, output
	the block as is and clear it from the state. Otherwise allocate and output a
	separate string with just the first line, and store the start position of
	the next line for a future call. The stored block is freed when the last
	line is consumed.
*/
static t_status	input_split_multiline(t_input *input, char **line_out)
{
	char	*newline;
	char	*line_start;

	line_start = input->multiline + input->multiline_pos;
	newline = ft_strchr(line_start, '\n');
	if (!newline && input->multiline_pos == 0)
	{
		*line_out = line_start;
		input->multiline = NULL;
		return (S_OK);
	}
	if (newline)
	{
		*newline = '\0';
		input->multiline_pos = newline - input->multiline + 1;
	}
	*line_out = ft_strdup(line_start);
	if (!*line_out)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	if (!newline)
	{
		free(input->multiline);
		input->multiline = NULL;
	}
	return (S_OK);
}

/*
	Get a line of input from input reader.

	Because Readline can actually return multiple lines at once (a string with
	newline characters in it) by multiline paste or by manually typing literal
	newlines with ^V^J, this function splits any multiline input and stores the
	remainder for a future call.
*/
t_status	input_get_line(t_input *input, char **line_out, const char *prompt)
{
	t_status	status;

	*line_out = NULL;
	if (!input->multiline)
	{
		if (isatty(STDIN_FILENO))
			status = input_tty_get_line(&input->multiline, prompt);
		else
			status = input_notty_get_line(&input->multiline, prompt);
		if (status != S_OK || !input->multiline)
			return (status);
		input->multiline_pos = 0;
	}
	return (input_split_multiline(input, line_out));
}

void	input_add_history(const char *line)
{
	if (isatty(STDIN_FILENO))
		input_tty_add_history(line);
}

void	input_clear_history(void)
{
	if (isatty(STDIN_FILENO))
		input_tty_clear_history();
}
