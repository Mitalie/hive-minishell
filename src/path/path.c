/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:02:43 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/05 23:57:48 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path_internal.h"
#include "env.h"
#include <string.h>
#include <stdlib.h>
#include <libft.h>

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

/*
	Searches for an executable command in the provided directories
	Returns the full path to the first matching executable, or NULL if not found
	Frees the dirs array and path_copy before returning
*/
static char	*search_in_dirs(char **dirs, const char *cmd, char *path_copy)
{
	char	*full_path;
	int		i;

	i = 0;
	while (dirs[i])
	{
		full_path = build_full_path(dirs[i], cmd);
		if (full_path && path_is_executable(full_path))
		{
			ft_free_split(dirs);
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_split(dirs);
	free(path_copy);
	return (NULL);
}

/*
	Splits the PATH string into directories and searches for the command
	Returns the full path to the command if found, NULL otherwise
*/
static char	*process_path_dirs(char *path_copy, const char *cmd)
{
	char	**dirs;

	dirs = ft_split(path_copy, ':');
	if (!dirs)
	{
		free(path_copy);
		return (NULL);
	}
	return (search_in_dirs(dirs, cmd, path_copy));
}

/*
	Searches for a command in the PATH environment variable
	Handles absolute/relative paths and commands without path
	Returns the full path to the command if found, NULL otherwise
*/
char	*path_search(const char *cmd, t_env *env)
{
	char	*path_var;
	char	*path_copy;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = env_get(env, "PATH");
	if (!path_var)
		return (NULL);
	path_copy = ft_strdup(path_var);
	if (!path_copy)
		return (NULL);
	return (process_path_dirs(path_copy, cmd));
}
