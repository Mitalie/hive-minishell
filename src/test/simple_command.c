/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:11:56 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/04 22:20:36 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "ast.h"
#include "execute.h"
#include "shenv.h"
#include "status.h"

/*
	We're testing internal function of execute module - just forward declare it.
*/
t_status	execute_simple_command(struct s_ast_simple_command *command,
				t_shenv *env, bool is_child);

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
	t_shenv		env;
	t_status	status;

	status = shenv_init(&env);
	if (status != S_OK)
		return (1);
	env.exit_code = -1;
	status = execute_simple_command(g_test_command, &env, false);
	dprintf(STDERR_FILENO,
		"execute_simple_command returned internal status %d, exit code %d\n",
		status, env.exit_code);
	shenv_free(&env);
	return (env.exit_code);
}
