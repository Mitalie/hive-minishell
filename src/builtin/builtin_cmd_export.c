/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:30:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/30 10:04:09 by josmanov         ###   ########.fr       */
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
	Extracts key and value from an export argument using argv modification
*/
static t_status	extract_key_value(char *arg, char **key_out,
	char **value_out, int *exit_code)
{
	char	*equal_sign;

	(void)exit_code;
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (S_OK);
	*equal_sign = '\0';
	*key_out = arg;
	*value_out = equal_sign + 1;
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

	key = NULL;
	value = NULL;
	status = extract_key_value(arg, &key, &value, exit_code);
	if (status != S_OK)
		return (status);
	if (!key)
		key = arg;
	if (!is_valid_identifier(key))
	{
		status_warn("export: not a valid identifier", key, 0);
		*exit_code = 2;
		return (S_OK);
	}
	if (value)
		return (env_set(env, key, value));
	return (S_OK);
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
		if (!print_exports(env, stdout_fd))
		{
			*exit_code = 1;
			return (S_OK);
		}
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
