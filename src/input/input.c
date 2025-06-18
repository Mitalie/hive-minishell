/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:33:08 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/18 15:43:12 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "input_internal.h"

#include <unistd.h>

t_status	input_get_line(char **line_out, const char *prompt)
{
	if (isatty(STDIN_FILENO))
		return (input_tty_get_line(line_out, prompt));
	return (input_notty_get_line(line_out, prompt));
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
