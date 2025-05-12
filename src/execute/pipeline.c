/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:13:08 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/12 17:36:45 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_internal.h"
#include "execute.h"

#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast.h"

/*
	Create a pipe if necessary and determine input/output FDs for the current
	command.
	TODO: handle pipe() error
*/
static void	pipeline_create_pipe(struct s_pipeline_fds *fds,
	bool first, bool last)
{
	int	pipe_fd[2];

	if (first)
		fds->in = NO_PIPE;
	else
		fds->in = fds->next_in;
	if (last)
	{
		fds->next_in = NO_PIPE;
		fds->out = NO_PIPE;
	}
	else
	{
		pipe(pipe_fd);
		fds->next_in = pipe_fd[0];
		fds->out = pipe_fd[1];
	}
}

/*
	Move the input and output pipes to correct FD numbers, and close the read
	end of the pipe as it belongs to the next command and not this one.
	TODO: handle dup2() error
	TODO: report close() error
*/
static void	pipeline_cleanup_child(struct s_pipeline_fds *fds)
{
	if (fds->in != NO_PIPE)
	{
		dup2(fds->in, STDIN_FILENO);
		close(fds->in);
	}
	if (fds->next_in != NO_PIPE)
		close(fds->next_in);
	if (fds->out != NO_PIPE)
	{
		dup2(fds->out, STDOUT_FILENO);
		close(fds->out);
	}
}

/*
	Close the pipe FDs created for the current command and not needed for the
	next one.
	TODO: report close() error
*/
static void	pipeline_cleanup_parent(struct s_pipeline_fds *fds)
{
	if (fds->in != NO_PIPE)
		close(fds->in);
	if (fds->out != NO_PIPE)
		close(fds->out);
}

static int	pipeline_wait_for_children(int num_children, pid_t last_child)
{
	int		pipeline_status;
	int		wait_status;
	pid_t	waited_child;

	pipeline_status = 0;
	while (num_children--)
	{
		waited_child = wait(&wait_status);
		if (waited_child == last_child)
		{
			if (WIFEXITED(wait_status))
				pipeline_status = WEXITSTATUS(wait_status);
			else if (WIFSIGNALED(wait_status))
				pipeline_status = 128 + WTERMSIG(wait_status);
		}
	}
	return (pipeline_status);
}

/*
	Execute all commands in a pipeline in parallel, connecting the stdout of
	each command to the stdin of the next one.
	TODO: handle fork() error
	TODO: in child, cleanup and exit with correct exit code after empty command
	or builtin (no execve)
	TODO: handle single builtin in main process
*/
int	execute_pipeline(struct s_ast_simple_command *pipeline_head, t_env *env)
{
	pid_t					child;
	bool					first;
	struct s_pipeline_fds	fds;
	int						num_children;

	first = true;
	num_children = 0;
	child = 0;
	while (pipeline_head)
	{
		num_children++;
		pipeline_create_pipe(&fds, first, !pipeline_head->next);
		child = fork();
		if (child == 0)
		{
			pipeline_cleanup_child(&fds);
			execute_simple_command(pipeline_head, env);
			exit(0);
		}
		else
			pipeline_cleanup_parent(&fds);
		pipeline_head = pipeline_head->next;
		first = false;
	}
	return (pipeline_wait_for_children(num_children, child));
}
