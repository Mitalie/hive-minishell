/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:30:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/18 06:05:59 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include <stdlib.h>
#include <unistd.h>

#include "env.h"
#include "status.h"
#include "util.h"
#include "libft.h"
#include "builtin_cmd_export_utils.h"

/*
	Extracts key and value from an export argument
*/
static t_status	extract_key_value(char *arg, char **key_out,
	char **value_out, int *exit_code)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (S_OK);
	key = ft_substr(arg, 0, equal_sign - arg);
	if (!key)
	{
		*exit_code = 1;
		return (status_err(S_RESET_ERR, "malloc", NULL, 0));
	}
	value = ft_strdup(equal_sign + 1);
	if (!value)
	{
		free(key);
		*exit_code = 1;
		return (status_err(S_RESET_ERR, "malloc", NULL, 0));
	}
	*key_out = key;
	*value_out = value;
	return (S_OK);
}

/*
	Handles a single export argument
	Validates the identifier
*/
static t_status	handle_export_arg(char *arg, t_env *env, int *exit_code)
{
	char		*key;
	char		*value;
	t_status	status;

	if (!is_valid_identifier(arg))
	{
		status_warn("export: not a valid identifier", arg, 0);
		*exit_code = 2;
		return (S_COMM_ERR);
	}
	if (!ft_strchr(arg, '='))
		return (S_OK);
	status = extract_key_value(arg, &key, &value, exit_code);
	if (status != S_OK)
		return (status);
	return (set_env_var(key, value, env));
}

/*
	export builtin command - sets environment variables
*/
t_status	builtin_cmd_export(char **argv, t_env *env,
	int *exit_code, int stdout_fd)
{
	int			i;
	t_status	status;

	*exit_code = 0;
	if (!argv[1])
	{
		print_exports(env, stdout_fd);
		return (S_OK);
	}
	i = 1;
	while (argv[i])
	{
		status = handle_export_arg(argv[i], env, exit_code);
		if (status != S_OK)
			return (status);
		i++;
	}
	if (argv[1] && ft_strchr(argv[1], '='))
		execve("/bin/env", (char *[]){"/bin/env", NULL}, env->env_array);
	return (S_OK);
}
