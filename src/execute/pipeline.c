/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:13:08 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/04 20:45:58 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_internal.h"
#include "execute.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast.h"
#include "shenv.h"
#include "status.h"

/*
	Create a pipe if necessary and determine input/output FDs for the current
	command, then fork a child process and store the result in fork_result.
*/
static t_status	execute_pipeline_create_pipe_and_fork(
	struct s_pipeline_fds *fds, bool first, bool last, pid_t *fork_result)
{
	int	pipe_fd[2];

	fds->in = NO_PIPE;
	if (!first)
		fds->in = fds->next_in;
	fds->next_in = NO_PIPE;
	fds->out = NO_PIPE;
	if (!last)
	{
		if (pipe(pipe_fd) < 0)
			return (status_err(S_RESET_ERR, "execute_pipeline",
					"pipe() failed", errno));
		fds->next_in = pipe_fd[0];
		fds->out = pipe_fd[1];
	}
	*fork_result = fork();
	if (*fork_result < 0)
		return (status_err(S_RESET_ERR, "execute_pipeline",
				"fork() failed", errno));
	return (S_OK);
}

/*
	Move the input and output pipes to correct FD numbers, close the read end of
	the output pipe, and execute the command. execute_simple_command ensures the
	child process returns with an exiting status.

	We don't need to check for close() errors here. They are only useful for
	reporting potential data loss in case of a delayed write error, or reporting
	that a delayed write may still be pending in case of EINTR. Such delayed
	writes that are flushed on close don't apply to pipes, and we haven't
	written anything yet anyway.
*/
static t_status	execute_pipeline_child(struct s_pipeline_fds *fds,
	struct s_ast_simple_command *child_command, t_shenv *env)
{
	t_status	status;

	status = S_OK;
	if (fds->in != NO_PIPE)
	{
		if (dup2(fds->in, STDIN_FILENO) < 0)
			status = status_err(S_EXIT_ERR, "execute_pipeline_child",
					"dup2() failed", errno);
		close(fds->in);
	}
	if (fds->next_in != NO_PIPE)
		close(fds->next_in);
	if (fds->out != NO_PIPE)
	{
		if (dup2(fds->out, STDOUT_FILENO) < 0)
			status = status_err(S_EXIT_ERR, "execute_pipeline_child",
					"dup2() failed", errno);
		close(fds->out);
	}
	if (status == S_OK)
		status = execute_simple_command(child_command, env, true);
	return (status);
}

/*
	Close the pipe FDs created for the current command and not needed for the
	next one. Also close read end of the next pipe if fork failed and we're not
	continuing to next command.

	We don't need to check for close() errors here. They are only useful for
	reporting potential data loss in case of a delayed write error, or reporting
	that a delayed write may still be pending in case of EINTR. Such delayed
	writes that are flushed on close don't apply to pipes, and we haven't
	written anything yet anyway.
*/
static void	execute_pipeline_cleanup(struct s_pipeline_fds *fds, bool had_error)
{
	if (fds->in != NO_PIPE)
		close(fds->in);
	if (had_error && fds->next_in != NO_PIPE)
		close(fds->next_in);
	if (fds->out != NO_PIPE)
		close(fds->out);
}

/*
	Wait until all child processes have exited. Store the exit status of the
	last process of the pipeline. Pass through the given t_status unless an
	unexpected error happens.

	If fork fails, last_child is -1 and exit_code remains unmodified, but in an
	error case the returned status should override exit_code anyway.

	When there are no more child processes, wait fails with ECHILD. The actual
	error case should be impossible to hit, but no harm in checking.
*/
static t_status	execute_pipeline_wait_for_children(
	t_status status, pid_t last_child, t_shenv *env)
{
	int		wait_status;
	pid_t	waited_child;

	while (true)
	{
		waited_child = wait(&wait_status);
		if (waited_child < 0)
		{
			if (errno == EINTR)
				continue ;
			if (errno == ECHILD)
				return (status);
			return (status_err(S_EXIT_ERR, "execute_pipeline: internal error",
					"wait failed", errno));
		}
		if (waited_child == last_child)
		{
			if (WIFEXITED(wait_status))
				env->exit_code = WEXITSTATUS(wait_status);
			else if (WIFSIGNALED(wait_status))
				env->exit_code = 128 + WTERMSIG(wait_status);
		}
	}
}

/*
	Execute all commands in a pipeline in parallel, connecting the stdout of
	each command to the stdin of the next one.
*/
t_status	execute_pipeline(struct s_ast_simple_command *pipeline_head,
	t_shenv *env)
{
	t_status				status;
	pid_t					child;
	bool					first;
	struct s_pipeline_fds	fds;

	status = S_OK;
	first = true;
	child = 0;
	if (pipeline_head && !pipeline_head->next)
		return (execute_simple_command(pipeline_head, env, false));
	while (pipeline_head)
	{
		status = execute_pipeline_create_pipe_and_fork(
				&fds, first, !pipeline_head->next, &child);
		if (status == S_OK && child == 0)
			return (execute_pipeline_child(&fds, pipeline_head, env));
		execute_pipeline_cleanup(&fds, status != S_OK);
		if (status != S_OK)
			break ;
		pipeline_head = pipeline_head->next;
		first = false;
	}
	return (execute_pipeline_wait_for_children(status, child, env));
}
