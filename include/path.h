/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:03:04 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/24 16:03:04 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_H
# define PATH_H

# include "env.h"

/*
** Search for command in PATH
** Returns allocated string with full path if found, NULL otherwise
*/
char	*path_search(const char *cmd, t_env *env);

#endif