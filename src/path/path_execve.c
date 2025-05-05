/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_execve.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:30:33 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/06 00:01:00 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path_internal.h"
#include <unistd.h>
#include <errno.h>
#include <libft.h>

/*
	Builds a full path from directory and command, then checks if it's executable
	Returns the full path if it exists and is executable, NULL otherwise
*/
static char	*build_path_and_try(char *dir, char **argv)
{
	char	*full_path;

	full_path = build_full_path(dir, argv[0]);
	if (!full_path)
		return (NULL);
	if (path_is_executable(full_path))
		return (full_path);
	free(full_path);
	return (NULL);
}

/*
	Handles execution of commands with absolute paths
	Attempts to execute the command and returns appropriate exit code
	Returns 126 for permission denied, 127 for other errors
*/
static int	handle_absolute_path(char **argv, char **envp)
{
	execve(argv[0], argv, envp);
	if (errno == EACCES)
		return (126);
	return (127);
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

	full_path = build_path_and_try(dir, argv);
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
int	try_path_execve(char *path_list, char **argv, char **envp)
{
	char	*elem_start;
	char	*scan;
	int		had_permission_error;

	if (!path_list || !argv || !argv[0] || !*argv[0])
		return (127);
	if (ft_strchr(argv[0], '/'))
		return (handle_absolute_path(argv, envp));
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
