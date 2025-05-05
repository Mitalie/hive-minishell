/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:02:24 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/05 23:58:00 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path_internal.h"
#include <unistd.h>
#include <sys/stat.h>

/*
	Frees a NULL-terminated array of strings and the array itself
	Used to clean up after ft_split and similar functions
*/
void	ft_free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

/*
	Checks if a file exists, is accessible, and is a regular executable file
	Uses access() to verify execution permissions and stat() to verify file type
	Returns true if the path points to an executable file, false otherwise
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
