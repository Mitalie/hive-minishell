/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:19:21 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/12 15:26:25 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "builtin_internal.h"

#include <stddef.h>

#include "libft.h"

static const struct s_builtin_func_reg	g_builtins[] = {
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
