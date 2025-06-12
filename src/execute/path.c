/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:02:43 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/12 23:39:49 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_internal.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft.h"
#include "shenv.h"
#include "status.h"

/*
	Construct a full path by joining a directory and command name.
	Handles cases where directory path may or may not end with a slash.
	Stores a newly allocated string with the full path in result_out.
*/
static t_status	execute_external_path_join(
	const char *dir, const char *cmd, char **result_out)
{
	size_t	dir_len;
	size_t	cmd_len;
	char	*result;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	result = malloc(dir_len + cmd_len + 2);
	if (!result)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	*result_out = result;
	ft_strlcpy(result, dir, dir_len + 1);
	if (dir_len > 0 && dir[dir_len - 1] != '/')
	{
		result[dir_len] = '/';
		ft_strlcpy(result + dir_len + 1, cmd, cmd_len + 1);
	}
	else
		ft_strlcpy(result + dir_len, cmd, cmd_len + 1);
	return (S_OK);
}

/*
	Try to execute a command in a specific candidate directory.
	If execve fails for reason other than ENOENT, stores the fact that a
	candidate was found and the resulting errno in the search struct.
*/
static t_status	execute_external_path_candidate(
	struct s_path_search *search, char **argv, char **envp)
{
	t_status	status;
	char		*full_path;

	status = execute_external_path_join(search->cand_dir, argv[0], &full_path);
	if (status != S_OK)
		return (status);
	execve(full_path, argv, envp);
	if (errno != ENOENT)
	{
		search->found = true;
		search->found_errno = errno;
	}
	free(full_path);
	return (S_OK);
}

/*
	Find and execute a command in path list.
	Efficiently separates candidate paths by modifying path list in-place.
	If execve fails for all candidates, prints appropriate error message and
	returns status code.
*/
static t_status	execute_external_path_search(
	char *path_list, char **argv, char **envp)
{
	t_status				status;
	char					*scan;
	struct s_path_search	search;

	scan = path_list;
	search.cand_dir = path_list;
	search.found = false;
	while (*scan)
	{
		if (*scan == ':')
		{
			*scan = '\0';
			status = execute_external_path_candidate(&search, argv, envp);
			if (status != S_OK)
				return (status);
			search.cand_dir = scan + 1;
		}
		scan++;
	}
	status = execute_external_path_candidate(&search, argv, envp);
	if (status != S_OK)
		return (status);
	if (search.found)
		return (status_err(S_EXEC_ERR, argv[0], NULL, search.found_errno));
	return (status_err(S_EXEC_NOTFOUND, argv[0], "command not found", 0));
}

/*
	Executs a command with an absolute path
	If execve fails, prints appropriate error message and returns status code
*/
static t_status	execute_external_absolute(char **argv, char **envp)
{
	execve(argv[0], argv, envp);
	if (errno == ENOENT)
		return (status_err(S_EXEC_NOTFOUND, argv[0], NULL, errno));
	else
		return (status_err(S_EXEC_ERR, argv[0], NULL, errno));
}

/*
	Searches for and executes a command by looking through PATH
	Sets exit_code to 126 for permission denied, 127 for command not found
	Reports appropriate error message using status_err
*/
t_status	execute_external_command(char **argv, t_shenv *env)
{
	t_status	status;
	char		*path_var;
	char		*path_copy;

	if (ft_strchr(argv[0], '/'))
		return (execute_external_absolute(argv, env->var_array));
	path_var = shenv_var_get(env, "PATH");
	if (!path_var)
		return (status_err(S_EXEC_NOTFOUND, argv[0], "command not found", 0));
	path_copy = ft_strdup(path_var);
	if (!path_copy)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	status = execute_external_path_search(path_copy, argv, env->var_array);
	free(path_copy);
	return (status);
}
