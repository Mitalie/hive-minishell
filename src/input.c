/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:49:06 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/06 17:58:24 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

#include <errno.h>
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "status.h"

t_status	input_get_line(char **line_out, const char *prompt)
{
	char	*line;

	errno = 0;
	line = readline(prompt);
	if (!line && errno)
		return (status_err(S_EXIT_ERR, "input error",
				"readline() failed", errno));
	*line_out = line;
	return (S_OK);
}

void	input_add_history(const char *line)
{
	add_history(line);
}

void	input_clear_history(void)
{
	rl_clear_history();
}
