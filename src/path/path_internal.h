/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_internal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:02:11 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/05 23:57:56 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_INTERNAL_H
# define PATH_INTERNAL_H

# include <stdbool.h>
# include <stdlib.h>

/*
	Internal functions for path-related operations
	These functions are used by the path module but not exposed to other modules
*/
bool	path_is_executable(const char *path);
void	ft_free_split(char **arr);
char	*build_full_path(const char *dir, const char *cmd);

#endif
