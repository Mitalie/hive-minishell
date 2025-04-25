/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:02:24 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/25 16:36:41 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path_internal.h"
#include <unistd.h>
#include <sys/stat.h>

void	ft_free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

/*
** Check if command is executable at given path
** Uses access() to verify execution permissions
*/
bool	path_is_executable(const char *path)
{
	struct stat	st;

	if (access(path, X_OK) == 0 && stat(path, &st) == 0)
	{
		if (S_ISREG(st.st_mode))
			return (true);
	}
	return (false);
}
