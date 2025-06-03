/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_pwd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:54:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/03 16:10:28 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "env.h"
#include "libft.h"
#include "status.h"
#include "util.h"

/*
	Writes the current working directory with newline
*/
static t_status	write_cwd(char *cwd, int stdout_fd)
{
	size_t	len;

	len = ft_strlen(cwd);
	cwd[len] = '\n';
	if (!util_write_all(stdout_fd, cwd, len + 1))
		return (status_err(S_BUILTIN_ERR, "pwd", "write failed", errno));
	return (S_OK);
}

/*
 	Prints the current working directory
	Note: getcwd(NULL, 0) is a glibc extension, not POSIX standard
*/
t_status	builtin_cmd_pwd(char **argv, t_env *env,
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
		status_warn("pwd: getcwd failed", NULL, errno);
		*exit_code = 1;
		return (S_OK);
	}
	status = write_cwd(cwd, stdout_fd);
	free(cwd);
	return (status);
}
