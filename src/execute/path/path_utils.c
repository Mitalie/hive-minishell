/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:02:24 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/11 01:07:27 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path_utils.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "libft.h"

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

/*
	Constructs a full path by joining a directory and command name
	Handles cases where directory path may or may not end with a slash
	Returns a newly allocated string with the full path
*/
char	*build_full_path(const char *dir, const char *cmd)
{
	size_t	dir_len;
	size_t	cmd_len;
	char	*result;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	result = malloc(dir_len + cmd_len + 2);
	if (!result)
		return (NULL);
	ft_strlcpy(result, dir, dir_len + 1);
	if (dir_len > 0 && dir[dir_len - 1] != '/')
	{
		result[dir_len] = '/';
		ft_strlcpy(result + dir_len + 1, cmd, cmd_len + 1);
	}
	else
		ft_strlcpy(result + dir_len, cmd, cmd_len + 1);
	return (result);
}
