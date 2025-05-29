/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:11:56 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/29 19:32:36 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "ast.h"
#include "env.h"
#include "execute.h"
#include "status.h"

/*
	/bin/echo hello world! > test_simple_trunc_nooutput >> test_simple_app
*/
struct s_ast_simple_command	*g_test_command
	= &(struct s_ast_simple_command){
	.args = &(struct s_ast_command_word){
	.word = "/bin/echo",
	.next = &(struct s_ast_command_word){
	.word = "hello",
	.next = &(struct s_ast_command_word){
	.word = "world!",
	.next = NULL,
},},},
	.redirs = &(struct s_ast_redirect){
	.op = AST_REDIR_OUT,
	.word = "test_simple_trunc_nooutput",
	.next = &(struct s_ast_redirect){
	.op = AST_REDIR_APP,
	.word = "test_simple_app",
	.next = NULL,
},},
	.next = NULL,
};

int	main(void)
{
	t_env		env;
	t_status	status;
	int			exit_code;

	status = env_init(&env);
	if (status != S_OK)
		return (1);
	exit_code = -1;
	status = execute_simple_command(g_test_command, &env, &exit_code, false);
	dprintf(STDERR_FILENO,
		"execute_simple_command returned internal status %d, exit code %d\n",
		status, exit_code);
	env_free(&env);
	return (exit_code);
}
