/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:43:18 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/25 15:56:00 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_internal.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

static char	*append_counter_to_filename(char *filename, int counter)
{
	char	*counter_str;
	char	*temp;

	counter_str = ft_itoa(counter);
	if (!counter_str)
		return (NULL);
	temp = filename;
	filename = ft_strjoin(filename, "_");
	free(temp);
	if (!filename)
	{
		free(counter_str);
		return (NULL);
	}
	temp = filename;
	filename = ft_strjoin(filename, counter_str);
	free(temp);
	free(counter_str);
	return (filename);
}

static char	*create_filename_prefix(void)
{
	char	*filename;
	char	*pid_str;

	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (NULL);
	filename = ft_strjoin(HEREDOC_TMP_PREFIX, pid_str);
	free(pid_str);
	if (!filename)
		return (NULL);
	return (filename);
}

char	*generate_temp_filename(void)
{
	char		*filename;
	static int	counter = 0;

	filename = create_filename_prefix();
	if (!filename)
		return (NULL);
	filename = append_counter_to_filename(filename, counter++);
	if (!filename)
		return (NULL);
	return (filename);
}
