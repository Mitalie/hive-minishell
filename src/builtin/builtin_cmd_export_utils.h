/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export_utils.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:30:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/04 20:14:39 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_CMD_EXPORT_UTILS_H
# define BUILTIN_CMD_EXPORT_UTILS_H

# include <stdbool.h>

# include "shenv.h"
# include "status.h"

t_status	print_exports(t_shenv *env, int stdout_fd);
bool		is_valid_identifier(const char *str);

#endif
