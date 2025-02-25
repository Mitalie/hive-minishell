/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:21:06 by amakinen          #+#    #+#             */
/*   Updated: 2025/02/25 17:30:53 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

#include "ast.h"

/*
	Execute a simple command, processing word expansions and redirections as
	necessary.
	TODO: expand and apply redirects
	TODO: expand args and build argv
	TODO: path search
	TODO: execve or execute builtin
*/
void	execute_simple_command(struct s_ast_simple_command *command)
{
}
