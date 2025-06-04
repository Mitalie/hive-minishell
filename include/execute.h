/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:28:43 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/04 22:17:10 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "ast.h"
# include "shenv.h"
# include "status.h"

t_status	execute_list(struct s_ast_list_entry *list_head,
				t_shenv *env);

#endif
