/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export_utils.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:30:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/12 23:59:34 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_CMD_EXPORT_UTILS_H
# define BUILTIN_CMD_EXPORT_UTILS_H

# include <stdbool.h>

# include "shenv.h"
# include "status.h"

t_status	bi_export_print_env(t_shenv *env, int stdout_fd);
bool		bi_export_validate_key(const char *str);

#endif
