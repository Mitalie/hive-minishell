/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:23:12 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/25 19:05:55 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_internal.h"
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"

/*
** Process a line from heredoc input.
** Returns 1 if the line matches the delimiter, 0 otherwise.
*/
int	handle_heredoc_line(int fd, char *line, char *delimiter)
{
	int	delim_len;
	int	line_len;
	int	is_delimiter;

	if (!line)
		return (1);
	delim_len = ft_strlen(delimiter);
	line_len = ft_strlen(line);
	is_delimiter = 0;
	if (line_len == delim_len && ft_strncmp(line, delimiter, delim_len) == 0)
		is_delimiter = 1;
	if (is_delimiter)
	{
		free(line);
		return (1);
	}
	write(fd, line, line_len);
	write(fd, "\n", 1);
	free(line);
	return (0);
}
