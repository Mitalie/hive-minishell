/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:47:16 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/29 19:19:38 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "execute_internal.h"

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>

#include "ast.h"
#include "status.h"
#include "word.h"

/*
	TODO (entire file): Does close error need handling? Store path with fds for
	close error?
*/

/*
	Prepare heredoc for redirection and store the file descriptor in the correct
	fds field. If the field holds another file descriptor, close it first.

	No error message printed here, because process_heredoc prints its own.
	TODO: Refactor heredoc handling to skip this wrapper.
*/
static t_status	execute_redirect_heredoc(struct s_ast_redirect *redir,
	struct s_redir_fds *fds)
{
	if (fds->in != NO_REDIR)
		close(fds->in);
	fds->in = process_heredoc(redir);
	if (fds->in == -1)
		return (S_COMM_ERR);
	return (S_OK);
}

/*
	Open a path for redirection and store the file descriptor in the correct
	fds field. If the field holds another file descriptor, close it first.
*/
static t_status	execute_redirect_open(enum e_ast_redirect_op op,
	const char *path, struct s_redir_fds *fds)
{
	if (op == AST_REDIR_IN && fds->in != NO_REDIR)
		close(fds->in);
	else if (op != AST_REDIR_IN && fds->out != NO_REDIR)
		close(fds->out);
	if (op == AST_REDIR_IN)
		fds->in = open(path, O_RDONLY);
	else if (op == AST_REDIR_OUT)
		fds->out = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	else if (op == AST_REDIR_APP)
		fds->out = open(path, O_CREAT | O_WRONLY | O_APPEND, 0666);
	if ((op == AST_REDIR_IN && fds->in == -1)
		|| (op != AST_REDIR_IN && fds->out == -1))
		return (status_err(S_COMM_ERR, "failed to open redirection",
				path, errno));
	return (S_OK);
}

/*
	Expand the word of a file redirection and open it as a path. If expansion
	doesn't generate exactly one field, an error is reported instead.
*/
static t_status	execute_redirect_file(struct s_ast_redirect *redir,
	struct s_redir_fds *fds)
{
	t_status			status;
	struct s_word_field	*expanded;
	struct s_word_field	**exp_append;

	expanded = NULL;
	exp_append = &expanded;
	status = word_expand(redir->word, &exp_append);
	if (status == S_OK && expanded && !expanded->next)
		status = execute_redirect_open(redir->op, expanded->value, fds);
	else if (status == S_OK)
		status = status_err(S_COMM_ERR, "ambiguous redirect", redir->word, 0);
	word_free(expanded);
	return (status);
}

/*
	Finish working with redirection file descriptors. If apply is set, move the
	file descriptors to the correct FD numbers (before executing an external
	command). Otherwise, close the file descriptors (after an error, or after
	executing a builtin or an empty command).
*/
t_status	execute_redirect_finish(struct s_redir_fds *fds, bool apply)
{
	t_status	status;

	status = S_OK;
	if (fds->in != NO_REDIR)
	{
		if (apply && dup2(fds->in, STDIN_FILENO) < 0)
			status = status_err(S_EXIT_ERR, "execute_redirect_apply",
					"dup2() failed", errno);
		close(fds->in);
	}
	if (fds->out != NO_REDIR)
	{
		if (apply && dup2(fds->out, STDOUT_FILENO) < 0)
			status = status_err(S_EXIT_ERR, "execute_redirect_apply",
					"dup2() failed", errno);
		close(fds->out);
	}
	return (status);
}

/*
	Process all redirection AST nodes in a list, storing the final redirection
	file descriptors in the fds struct.

	If an error happens, file descriptors are closed and values in fds are
	undefined.
*/
t_status	execute_redirect_prepare(struct s_redir_fds *fds,
	struct s_ast_redirect *redirs)
{
	t_status				status;
	enum e_ast_redirect_op	op;

	fds->in = NO_REDIR;
	fds->out = NO_REDIR;
	status = S_OK;
	while (redirs && status == S_OK)
	{
		op = redirs->op;
		if (op == AST_REDIR_IN || op == AST_REDIR_OUT || op == AST_REDIR_APP)
			status = execute_redirect_file(redirs, fds);
		else if (op == AST_HEREDOC)
			status = execute_redirect_heredoc(redirs, fds);
		else
			status = status_err(S_EXIT_ERR, "execute_redirect: internal error",
					"invalid ast_redirect type", 0);
		redirs = redirs->next;
	}
	if (status != S_OK)
		execute_redirect_finish(fds, false);
	return (status);
}
