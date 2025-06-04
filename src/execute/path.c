/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:02:43 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/04 20:44:30 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_internal.h"

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft.h"
#include "shenv.h"
#include "status.h"

/*
	Constructs a full path by joining a directory and command name
	Handles cases where directory path may or may not end with a slash
	Returns a newly allocated string with the full path
*/
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

/*
	Tries to execute a command in a specific directory
	Returns 1 if permission denied, 0 otherwise
	Used to track if any permission errors occurred during path search
*/
static int	try_path_element(char *dir, char **argv, char **envp)
{
	char	*full_path;
	int		result;

	full_path = build_full_path(dir, argv[0]);
	if (!full_path)
		return (0);
	execve(full_path, argv, envp);
	result = 0;
	if (errno == EACCES)
		result = 1;
	free(full_path);
	return (result);
}

/*
	Executes a command by searching through PATH environment variable
	Efficiently searches by modifying path_list in-place
	Returns appropriate exit code:
	- 0 on successful execution (never returns in this case)
	- 126 if command was found but permission was denied
	- 127 if command was not found in any PATH directory
*/
static int	try_path_execve(char *path_list, char **argv, char **envp)
{
	char	*elem_start;
	char	*scan;
	int		had_permission_error;

	if (!path_list || !argv || !argv[0] || !*argv[0])
		return (127);
	had_permission_error = 0;
	elem_start = path_list;
	scan = path_list;
	while (*scan)
	{
		if (*scan == ':')
		{
			*scan = '\0';
			had_permission_error |= try_path_element(elem_start, argv, envp);
			elem_start = scan + 1;
		}
		scan++;
	}
	had_permission_error |= try_path_element(elem_start, argv, envp);
	if (had_permission_error)
		return (126);
	return (127);
}

/*
	Executes a command with an absolute path
	Sets exit_code to 126 for permission denied, 127 for other errors
	Reports appropriate error message using status_err
*/
void	handle_absolute_path(char **argv, t_shenv *env)
{
	execve(argv[0], argv, env->var_array);
	if (errno == EACCES)
		env->exit_code = 126;
	else
		env->exit_code = 127;
	status_err(S_COMM_ERR, argv[0], NULL, errno);
}

/*
	Searches for and executes a command by looking through PATH
	Sets exit_code to 126 for permission denied, 127 for command not found
	Reports appropriate error message using status_err
*/
void	handle_path_search(char **argv, t_shenv *env)
{
	char	*path_var;
	char	*path_copy;

	if (ft_strchr(argv[0], '/'))
		return (handle_absolute_path(argv, env));
	path_var = shenv_var_get(env, "PATH");
	if (!path_var)
	{
		status_err(S_COMM_ERR, argv[0], "command not found", 0);
		env->exit_code = 127;
		return ;
	}
	path_copy = ft_strdup(path_var);
	if (!path_copy)
	{
		status_err(S_COMM_ERR, "malloc", NULL, errno);
		env->exit_code = 127;
		return ;
	}
	env->exit_code = try_path_execve(path_copy, argv, env->var_array);
	if (env->exit_code == 126)
		status_err(S_COMM_ERR, argv[0], "Permission denied", 0);
	else
		status_err(S_COMM_ERR, argv[0], "command not found", 0);
	free(path_copy);
}
