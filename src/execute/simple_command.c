/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:21:06 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/29 20:09:55 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "execute_internal.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast.h"
#include "builtin.h"
#include "env.h"
#include "status.h"
#include "word.h"

/*
	Count args in the given field list, allocate an argv-style NULL-terminated
	pointer array, and fill it with pointers to field values.
*/
static t_status	execute_arg_fields_to_argv(
	struct s_word_field *args, char ***argv_out)
{
	size_t				n_args;
	char				**argv;
	struct s_word_field	*current_arg;

	n_args = 0;
	current_arg = args;
	while (current_arg)
	{
		n_args++;
		current_arg = current_arg->next;
	}
	argv = malloc((n_args + 1) * sizeof(*argv));
	*argv_out = argv;
	if (!argv)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	n_args = 0;
	current_arg = args;
	while (current_arg)
	{
		argv[n_args] = current_arg->value;
		n_args++;
		current_arg = current_arg->next;
	}
	argv[n_args] = NULL;
	return (S_OK);
}

/*
	Expand all words in AST command word list into a field list, and build an
	argv array of field values that can be passed to execve.

	The argv array entries point to strings owned by the fields list, so the
	field list must not be freed while the argv array is in use. The caller
	is responsible for freeing the fields list, even in case of failure.

	If expansion produces no fields, no memory is allocated and the out pointers
	are set to null pointers.
*/
static t_status	execute_expand_args(struct s_ast_command_word *args,
	struct s_word_field **fields_out, char ***argv_out)
{
	t_status			status;
	struct s_word_field	**fields_append;

	*fields_out = NULL;
	*argv_out = NULL;
	fields_append = fields_out;
	while (args)
	{
		status = word_expand(args->word, &fields_append);
		if (status != S_OK)
			return (status);
		args = args->next;
	}
	if (!*fields_out)
		return (S_OK);
	return (execute_arg_fields_to_argv(*fields_out, argv_out));
}

/*
	Fork a child process for executing an external command. Child process
	sets is_child and returns, while main process waits for child to exit and
	stores its exit status in exit_code.
*/
static t_status	execute_command_fork(bool *is_child, int *exit_code)
{
	pid_t	child_pid;
	int		wait_status;

	child_pid = fork();
	if (child_pid < 0)
		return (status_err(S_RESET_ERR, "execute_command",
				"fork() failed", errno));
	if (child_pid == 0)
	{
		*is_child = true;
		return (S_OK);
	}
	while (wait(&wait_status) < 0)
	{
		if (errno == EINTR)
			continue ;
		return (status_err(S_EXIT_ERR, "execute_command: internal error",
				"wait() failed", errno));
	}
	if (WIFEXITED(wait_status))
		*exit_code = WEXITSTATUS(wait_status);
	else if (WIFSIGNALED(wait_status))
		*exit_code = 128 + WTERMSIG(wait_status);
	return (S_OK);
}

/*
	Apply redirections and execute the command according to its type. External
	commands move redirections to their correct file descriptor numbers, while
	builtins and empty commands use the the temporary file descriptors directly
	and close them afterwards.
*/
static t_status	execute_command_execute(struct s_ast_redirect *redirs,
	char **argv, t_env *env, int *exit_code)
{
	t_status			status;
	struct s_redir_fds	fds;
	t_builtin_func		*builtin;
	bool				is_external;

	status = execute_redirect_prepare(&fds, redirs);
	if (status != S_OK)
		return (status);
	builtin = NULL;
	if (argv)
		builtin = builtin_get_func(argv[0]);
	is_external = argv && !builtin;
	if (is_external)
		status = execute_redirect_finish(&fds, true);
	if (is_external && status == S_OK)
		handle_path_search(argv, env, exit_code);
	if (builtin && fds.out == NO_REDIR)
		status = builtin(argv, env, exit_code, STDOUT_FILENO);
	else if (builtin)
		status = builtin(argv, env, exit_code, fds.out);
	if (!is_external)
		execute_redirect_finish(&fds, false);
	return (status);
}

/*
	Execute a simple command from the AST

	Expands arguments, builds argv array and forks a child process if necessary.
	If forked, parent waits for child and collects its exit code. If not forked
	or in child, applies redirections and executes the command. Forces the child
	process to exit instead of staying around as a duplicate shell instance.
*/
t_status	execute_simple_command(struct s_ast_simple_command *command,
	t_env *env, int *exit_code, bool is_child)
{
	t_status			status;
	struct s_word_field	*arg_fields;
	char				**argv;
	bool				need_child;

	status = execute_expand_args(command->args, &arg_fields, &argv);
	if (status == S_OK)
		need_child = argv && !builtin_get_func(argv[0]);
	if (status == S_OK && need_child && !is_child)
		status = execute_command_fork(&is_child, exit_code);
	if (status == S_OK && (!need_child || is_child))
		status = execute_command_execute(command->redirs, argv, env, exit_code);
	if (is_child)
		status = status_force_exit(status, exit_code);
	free(argv);
	word_free(arg_fields);
	return (status);
}
