/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:30:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/03 16:11:14 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"
#include "builtin_cmd_export_utils.h"

#include "env.h"
#include "libft.h"
#include "status.h"

/*
	Extracts key and value from an export argument using argv modification
*/
static bool	extract_key_value(char *arg, char **key_out,
	char **value_out)
{
	char	*equal_sign;

	*key_out = NULL;
	*value_out = NULL;
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (false);
	*equal_sign = '\0';
	*key_out = arg;
	*value_out = equal_sign + 1;
	return (true);
}

/*
	Handles a single export argument
	Validates the identifier
*/
static t_status	handle_export_arg(char *arg, t_env *env, int *exit_code)
{
	char		*key;
	char		*value;

	if (!extract_key_value(arg, &key, &value))
		return (S_OK);
	if (!is_valid_identifier(key))
	{
		status_warn("export: not a valid identifier", key, 0);
		*exit_code = 2;
		return (S_OK);
	}
	return (env_set(env, key, value));
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
		return (print_exports(env, stdout_fd));
	i = 1;
	while (argv[i])
	{
		status = handle_export_arg(argv[i], env, exit_code);
		if (status != S_OK)
			return (status);
		i++;
	}
	return (S_OK);
}
