/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:11:56 by amakinen          #+#    #+#             */
/*   Updated: 2025/02/25 22:00:53 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "ast.h"
#include "execute.h"

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
	execute_simple_command(g_test_command);
	write(STDERR_FILENO, "execute_simple_command returned\n", 32);
}
