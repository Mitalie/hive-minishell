/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_pwd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:54:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/18 06:01:53 by josmanov         ###   ########.fr       */
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
 	Prints the current working directory
*/
t_status	builtin_cmd_pwd(char	**argv, t_env *env,
	int *exit_code, int stdout_fd)
{
	char	*cwd;

	(void)argv;
	(void)env;
	*exit_code = 0;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		*exit_code = 1;
		return (S_COMM_ERR);
	}
	util_write_all(stdout_fd, cwd, ft_strlen(cwd));
	util_write_all(stdout_fd, "\n", 1);
	free(cwd);
	return (S_OK);
}
