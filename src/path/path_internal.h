/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_internal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:02:11 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/25 16:37:07 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_INTERNAL_H
# define PATH_INTERNAL_H

# include <stdbool.h>
# include <stdlib.h>

/*
** Check if command is executable at given path
** Returns true if executable, false otherwise
*/
bool	path_is_executable(const char *path);
void	ft_free_split(char **arr);

#endif
