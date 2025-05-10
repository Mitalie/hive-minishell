/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:03:04 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/10 23:57:13 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_H
# define PATH_H

# include "env.h"

/*
	Searches for and executes a command by scanning through PATH
	Efficiently modifies path_list in-place to avoid extra allocations
	Returns appropriate exit code (126 for permission denied, 127 for not found)
*/
int		try_path_execve(char *path_list, char **argv, char **envp);

#endif