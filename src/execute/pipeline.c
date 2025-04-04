/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:13:08 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/04 21:59:48 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline_internal.h"
#include "execute.h"

#include <stdbool.h>
#include <sys/types.h>
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

/*
	Execute all commands in a pipeline in parallel, connecting the stdout of
	each command to the stdin of the next one.
	TODO: handle fork() error
	TODO: store info about children, wait for children before return
	TODO: store exit status
	TODO: in child, cleanup and exit after builtin (no execve)
	TODO: handle single builtin in main process
*/
int	execute_pipeline(struct s_ast_simple_command *pipeline_head)
{
	pid_t					child;
	bool					first;
	struct s_pipeline_fds	fds;
	int						status;

	first = true;
	status = 0;
	if (pipeline_head && pipeline_head->next)
		status = 1;
	while (pipeline_head)
	{
		pipeline_create_pipe(&fds, first, !pipeline_head->next);
		child = fork();
		if (child == 0)
		{
			pipeline_cleanup_child(&fds);
			execute_simple_command(pipeline_head);
		}
		else
			pipeline_cleanup_parent(&fds);
		pipeline_head = pipeline_head->next;
		first = false;
	}
	return (status);
}
