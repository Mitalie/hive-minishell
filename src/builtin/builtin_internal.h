/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:19:36 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/12 21:03:26 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_INTERNAL_H
# define BUILTIN_INTERNAL_H

# include "builtin.h"

# include "env.h"
# include "status.h"

struct s_builtin_func_reg
{
	t_builtin_func	*func;
	const char		*name;
};

t_status	builtin_cmd_exit(char	**argv, t_env *env,
				int *exit_code, int stdout_fd);
t_status	builtin_cmd_echo(char **argv, t_env *env,
				int *exit_code, int stdout_fd);
t_status	builtin_cmd_pwd(char	**argv, t_env *env,
				int *exit_code, int stdout_fd);

#endif
