/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:21:06 by amakinen          #+#    #+#             */
/*   Updated: 2025/02/25 21:24:35 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast.h"

extern char	**environ;

/*
	TODO: handle open() error
	TODO: handle dup2() error
	TODO: report close() error
*/
static void	do_redirect(const char *path, int target_fd, int open_flags)
{
	int	new_fd;

	new_fd = open(path, open_flags, 0666);
	dup2(new_fd, target_fd);
	close(new_fd);
}

/*
	Apply redirections from the ast redirection list.
	TODO: handle heredoc
	TODO: apply word processing steps
*/
static void	apply_redirects(struct s_ast_redirect *redirs)
{
	while (redirs)
	{
		if (redirs->op == AST_REDIR_IN)
			do_redirect(redirs->word, STDIN_FILENO, O_RDONLY);
		else if (redirs->op == AST_REDIR_OUT)
			do_redirect(redirs->word, STDOUT_FILENO,
				O_CREAT | O_WRONLY | O_TRUNC);
		else if (redirs->op == AST_REDIR_APP)
			do_redirect(redirs->word, STDOUT_FILENO,
				O_CREAT | O_WRONLY | O_APPEND);
		redirs = redirs->next;
	}
}

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
	TODO: path search
	TODO: handle builtins
*/
void	execute_simple_command(struct s_ast_simple_command *command)
{
	char	**argv;

	apply_redirects(command->redirs);
	if (!command->args)
		return ;
	argv = build_argv(command->args);
	if (!argv)
		return ;
	execve(argv[0], argv, environ);
	perror("execute_simple_command: execve");
	free(argv);
}
