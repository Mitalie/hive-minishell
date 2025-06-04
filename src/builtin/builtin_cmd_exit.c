/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:26:54 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/04 20:43:52 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include <stddef.h>

#include "shenv.h"
#include "status.h"
#include "util.h"

t_status	builtin_cmd_exit(char	**argv, t_shenv *env, int stdout_fd)
{
	size_t		parsed_len;
	int			parsed_value;

	(void)env, (void)stdout_fd;
	if (!argv[0] || !argv[1])
		return (S_EXIT_OK);
	if (argv[2])
	{
		status_warn("exit: too many arguments", NULL, 0);
		env->exit_code = 2;
	}
	else if (!util_parse_int(argv[1], &parsed_len, &parsed_value)
		|| argv[1][parsed_len] != '\0'
		|| parsed_value < 0
		|| parsed_value > 255)
	{
		status_warn("exit: argument out of range (0-255)", NULL, 0);
		env->exit_code = 2;
	}
	else
		env->exit_code = parsed_value;
	return (S_EXIT_OK);
}
