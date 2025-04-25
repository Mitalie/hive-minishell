/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:02:43 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/25 16:02:43 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path_internal.h"
#include "env.h"
#include <string.h>
#include <stdlib.h>
#include <libft.h>

static char	*build_full_path(const char *dir, const char *cmd)
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
