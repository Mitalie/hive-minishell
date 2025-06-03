/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export_utils.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:30:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/03 15:21:35 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_CMD_EXPORT_UTILS_H
# define BUILTIN_CMD_EXPORT_UTILS_H

# include <stdbool.h>
# include "env.h"
# include "status.h"

t_status	print_exports(t_env *env, int stdout_fd);
bool		is_valid_identifier(const char *str);

#endif
