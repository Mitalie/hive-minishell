/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:21:06 by amakinen          #+#    #+#             */
/*   Updated: 2025/02/25 18:28:38 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast.h"

extern char	**environ;

/*
	Convert args list from ast to the argv format required by execve.
	TODO: apply word processing steps
*/
static char	**build_argv(struct s_ast_command_word *args)
{
	size_t						n_args;
	char						**argv;
	struct s_ast_command_word	*current_arg;

	n_args = 0;
	current_arg = args;
	while (current_arg)
	{
		n_args++;
		current_arg = current_arg->next;
	}
	argv = malloc((n_args + 1) * sizeof(*argv));
	if (!argv)
		return (NULL);
	n_args = 0;
	current_arg = args;
	while (current_arg)
	{
		argv[n_args] = current_arg->word;
		n_args++;
		current_arg = current_arg->next;
	}
	argv[n_args] = NULL;
	return (argv);
}

/*
	Execute a simple command, processing word expansions and redirections as
	necessary.
	TODO: expand and apply redirects
	TODO: path search
	TODO: handle builtins
*/
void	execute_simple_command(struct s_ast_simple_command *command)
{
	char	**argv;

	if (!command->args)
		return ;
	argv = build_argv(command->args);
	if (!argv)
		return ;
	execve(argv[0], argv, environ);
	perror("execute_simple_command: execve");
	free(argv);
}
