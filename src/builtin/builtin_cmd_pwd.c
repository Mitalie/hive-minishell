/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_pwd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:54:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/27 15:04:30 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include <unistd.h>
#include <stdlib.h>

#include "env.h"
#include "status.h"
#include "util.h"
#include "libft.h"

/*
	Writes the current working directory with newline
*/
static t_status	write_cwd(char *cwd, int stdout_fd, int *exit_code)
{
	size_t	len;

	len = ft_strlen(cwd);
	cwd[len] = '\n';
	if (!util_write_all(stdout_fd, cwd, len + 1))
	{
		cwd[len] = '\0';
		*exit_code = 1;
		return (S_COMM_ERR);
	}
	cwd[len] = '\0';
	return (S_OK);
}

/*
 	Prints the current working directory
	Note: getcwd(NULL, 0) is a glibc extension, not POSIX standard
*/
t_status	builtin_cmd_pwd(char	**argv, t_env *env,
	int *exit_code, int stdout_fd)
{
	char		*cwd;
	t_status	status;

	(void)argv;
	(void)env;
	*exit_code = 0;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		status_warn("pwd: getcwd failed", NULL, 0);
		*exit_code = 1;
		return (S_COMM_ERR);
	}
	status = write_cwd(cwd, stdout_fd, exit_code);
	free(cwd);
	return (status);
}
