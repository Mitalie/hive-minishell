/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export_utils.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:30:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/27 15:04:57 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_CMD_EXPORT_UTILS_H
# define BUILTIN_CMD_EXPORT_UTILS_H

# include <stdbool.h>
# include "env.h"
# include "status.h"

bool		print_exports(t_env *env, int stdout_fd);
bool		is_valid_identifier(const char *str);

#endif
