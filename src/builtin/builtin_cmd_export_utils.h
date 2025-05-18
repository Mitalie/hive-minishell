/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export_utils.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:30:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/18 06:15:33 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_CMD_EXPORT_UTILS_H
# define BUILTIN_CMD_EXPORT_UTILS_H

# include <stdbool.h>
# include "env.h"
# include "status.h"

void		print_exports(t_env *env, int stdout_fd);
bool		is_valid_identifier(const char *str);
t_status	set_env_var(char *key, char *value, t_env *env);

#endif
