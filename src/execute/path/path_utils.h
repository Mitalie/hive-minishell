/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 23:58:18 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/11 00:58:51 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_UTILS_H
# define PATH_UTILS_H

# include <stdbool.h>

void	ft_free_split(char **arr);
bool	path_is_executable(const char *path);
char	*build_full_path(const char *dir, const char *cmd);

#endif
