/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:21:06 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/29 19:19:57 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "execute_internal.h"

#include <stdbool.h>
#include <stdlib.h>

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
	Executes a simple command from the AST
	Handles redirections, builds argument array, and executes the command
	Exits with appropriate status code based on command execution result
	TODO: handle builtins
*/
t_status	execute_simple_command(struct s_ast_simple_command *command,
	t_env *env, int *exit_code)
{
	t_status			status;
	struct s_word_field	*arg_fields;
	char				**argv;
	t_builtin_func		*builtin;
	struct s_redir_fds	fds;

	status = execute_redirect_prepare(&fds, command->redirs);
	if (status == S_OK)
		status = execute_redirect_finish(&fds, true);
	if (status == S_OK)
		status = execute_expand_args(command->args, &arg_fields, &argv);
	if (status != S_OK || !argv)
		return (status);
	builtin = builtin_get_func(argv[0]);
	if (builtin)
		status = builtin(argv, env, exit_code, 1);
	else
		handle_path_search(argv, env, exit_code);
	free(argv);
	word_free(arg_fields);
	return (status);
}
