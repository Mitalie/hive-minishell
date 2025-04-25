/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:43:11 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/25 15:56:05 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_internal.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"

/*
** Creates a temporary file for heredoc content
*/
int	create_heredoc_file(char **tmp_filename)
{
	int	fd;

	*tmp_filename = generate_temp_filename();
	if (!*tmp_filename)
		return (-1);
	fd = open(*tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(*tmp_filename);
		return (-1);
	}
	return (fd);
}

/*
** Reopens the heredoc file for reading
*/
int	reopen_heredoc_file(char *tmp_filename)
{
	int	fd;

	fd = open(tmp_filename, O_RDONLY);
	if (fd == -1)
	{
		unlink(tmp_filename);
		free(tmp_filename);
		return (-1);
	}
	unlink(tmp_filename);
	free(tmp_filename);
	return (fd);
}
