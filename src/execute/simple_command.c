/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:21:06 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/12 17:56:02 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_internal.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "libft.h"
#include "ast.h"
#include "builtin.h"
#include "status.h"
#include "env.h"

int	process_heredoc(struct s_ast_redirect *redirect);

/*
	Performs a file redirection by opening a file and duplicating descriptors
	Handles different types of redirections based on the open_flags parameter
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
	Processes and applies all redirections from the AST redirection list
	Supports input (<), output (>), and append (>>) redirections
	TODO: apply word processing steps
*/
static void	apply_redirects(struct s_ast_redirect *redirs)
{
	int	fd;

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
		else if (redirs->op == AST_HEREDOC)
		{
			fd = process_heredoc(redirs);
			if (fd != -1)
			{
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
		}
		redirs = redirs->next;
	}
}

/*
	Converts the AST command word list to an argv array for execve
	Allocates a new array with command and arguments, NULL-terminated
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
	Executes a simple command from the AST
	Handles redirections, builds argument array, and executes the command
	Exits with appropriate status code based on command execution result
	TODO: handle builtins
*/
void	execute_simple_command(struct s_ast_simple_command *command, t_env *env)
{
	char			**argv;
	t_builtin_func	*builtin;
	int				exit_code;

	apply_redirects(command->redirs);
	if (!command->args)
		return ;
	argv = build_argv(command->args);
	if (!argv)
		return ;
	builtin = builtin_get_func(argv[0]);
	if (builtin)
	{
		builtin(argv, env, &exit_code, 1);
		free(argv);
		exit(exit_code);
	}
	handle_path_search(argv, env, &exit_code);
	free(argv);
	exit(exit_code);
}
