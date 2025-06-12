/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_pwd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:54:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/13 00:08:06 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft.h"
#include "shenv.h"
#include "status.h"
#include "util.h"

/*
 	Prints the current working directory
	Note: getcwd(NULL, 0) is a glibc extension, not POSIX standard
*/
t_status	builtin_cmd_pwd(char **argv, t_shenv *env, int stdout_fd)
{
	t_status	status;
	char		*cwd;
	size_t		len;

	(void)argv;
	(void)env;
	env->exit_code = 0;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (status_err(S_BUILTIN_ERR, "pwd", "getcwd failed", errno));
	len = ft_strlen(cwd);
	cwd[len] = '\n';
	status = S_OK;
	if (!util_write_all(stdout_fd, cwd, len + 1))
		status = status_err(S_BUILTIN_ERR, "pwd", "write failed", errno);
	free(cwd);
	return (status);
}
