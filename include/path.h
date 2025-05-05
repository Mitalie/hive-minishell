/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:03:04 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/05 23:56:56 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_H
# define PATH_H

# include "env.h"
# include <stdbool.h>

/*
	Public interface for path-related operations
	These functions handle searching for commands in PATH and executing them
*/
char	*path_search(const char *cmd, t_env *env);

/*
	Searches for and executes a command by scanning through PATH
	Efficiently modifies path_list in-place to avoid extra allocations
	Returns appropriate exit code (126 for permission denied, 127 for not found)
*/
int		try_path_execve(char *path_list, char **argv, char **envp);

#endif