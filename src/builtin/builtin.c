/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:19:21 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/17 20:05:45 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "builtin_internal.h"

#include <stddef.h>

#include "libft.h"

static const struct s_builtin_func_reg	g_builtins[] = {
{builtin_cmd_exit, "exit"},
{builtin_cmd_echo, "echo"},
{builtin_cmd_pwd, "pwd"},
{builtin_cmd_cd, "cd"},
{builtin_cmd_env, "env"},
{builtin_cmd_export, "export"},
{builtin_cmd_unset, "unset"},
};

t_builtin_func	*builtin_get_func(const char *name)
{
	size_t	cmp_len;
	size_t	i;

	cmp_len = ft_strlen(name) + 1;
	i = sizeof g_builtins / sizeof g_builtins[0];
	while (i--)
		if (ft_strncmp(name, g_builtins[i].name, cmp_len) == 0)
			return (g_builtins[i].func);
	return (NULL);
}
