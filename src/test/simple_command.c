/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:11:56 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/15 15:20:39 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "ast.h"
#include "execute.h"
#include "shell.h"

t_shell						g_shell;

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
	g_shell.env = env_init();
	if (!g_shell.env)
		return (1);
	execute_simple_command(g_test_command);
	write(STDERR_FILENO, "execute_simple_command returned\n", 32);
	env_free(g_shell.env);
}
