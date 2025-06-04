/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:19:36 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/04 20:14:39 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_INTERNAL_H
# define BUILTIN_INTERNAL_H

# include "builtin.h"

# include "shenv.h"
# include "status.h"

struct s_builtin_func_reg
{
	t_builtin_func	*func;
	const char		*name;
};

t_status	builtin_cmd_exit(char	**argv, t_shenv *env,
				int *exit_code, int stdout_fd);
t_status	builtin_cmd_echo(char **argv, t_shenv *env,
				int *exit_code, int stdout_fd);
t_status	builtin_cmd_pwd(char	**argv, t_shenv *env,
				int *exit_code, int stdout_fd);
t_status	builtin_cmd_cd(char **argv, t_shenv *env,
				int *exit_code, int stdout_fd);
t_status	builtin_cmd_env(char **argv, t_shenv *env,
				int *exit_code, int stdout_fd);
t_status	builtin_cmd_export(char **argv, t_shenv *env,
				int *exit_code, int stdout_fd);
t_status	builtin_cmd_unset(char **argv, t_shenv *env,
				int *exit_code, int stdout_fd);

#endif
