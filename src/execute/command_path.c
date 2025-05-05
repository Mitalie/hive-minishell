/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:02:18 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/05 23:50:25 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_internal.h"
#include <errno.h>
#include <stdlib.h>
#include "path.h"
#include "env.h"
#include "status.h"
#include "libft.h"

/*
	Executes a command with an absolute path
	Sets exit_code to 126 for permission denied, 127 for other errors
	Reports appropriate error message using status_err
*/
void	handle_absolute_path(char **argv, t_env *env, int *exit_code)
{
	execve(argv[0], argv, env_get_array(env));
	if (errno == EACCES)
		*exit_code = 126;
	else
		*exit_code = 127;
	status_err(S_COMM_ERR, argv[0], NULL, errno);
}

/*
	Searches for and executes a command by looking through PATH
	Sets exit_code to 126 for permission denied, 127 for command not found
	Reports appropriate error message using status_err
*/
void	handle_path_search(char **argv, t_env *env, int *exit_code)
{
	char	*path_var;
	char	*path_copy;

	path_var = env_get(env, "PATH");
	if (!path_var)
	{
		status_err(S_COMM_ERR, argv[0], "command not found", 0);
		*exit_code = 127;
		return ;
	}
	path_copy = ft_strdup(path_var);
	if (!path_copy)
	{
		status_err(S_COMM_ERR, "malloc", NULL, errno);
		*exit_code = 127;
		return ;
	}
	*exit_code = try_path_execve(path_copy, argv, env_get_array(env));
	if (*exit_code == 126)
		status_err(S_COMM_ERR, argv[0], "Permission denied", 0);
	else
		status_err(S_COMM_ERR, argv[0], "command not found", 0);
	free(path_copy);
}
