/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:35:02 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/04 22:20:49 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "ast.h"
#include "execute.h"
#include "shenv.h"
#include "status.h"

/*
	We're testing internal function of execute module - just forward declare it.
*/
t_status	execute_pipeline(struct s_ast_simple_command *pipeline_head,
				t_shenv *env);

/*
	/bin/echo -e first\n\n\ntest > test_pipeline_tmpout |
	/bin/echo -e hello\n world! |
	/bin/grep world |
	/bin/wc -c > /dev/stderr |
	/bin/wc < test_pipeline_tmpout
*/
struct s_ast_simple_command	*g_test_pipeline
	= &(struct s_ast_simple_command){
	.args = &(struct s_ast_command_word){
	.word = "/bin/echo",
	.next = &(struct s_ast_command_word){
	.word = "-e",
	.next = &(struct s_ast_command_word){
	.word = "first\\n\\n\\ntest",
	.next = NULL,
},},},
	.redirs = &(struct s_ast_redirect){
	.op = AST_REDIR_OUT,
	.word = "test_pipeline_tmpout",
	.next = NULL,
},
	.next = &(struct s_ast_simple_command){
	.args = &(struct s_ast_command_word){
	.word = "/bin/echo",
	.next = &(struct s_ast_command_word){
	.word = "-e",
	.next = &(struct s_ast_command_word){
	.word = "hello\\n",
	.next = &(struct s_ast_command_word){
	.word = "world!",
	.next = NULL,
},},},},
	.redirs = NULL,
	.next = &(struct s_ast_simple_command){
	.args = &(struct s_ast_command_word){
	.word = "/bin/grep",
	.next = &(struct s_ast_command_word){
	.word = "world",
	.next = NULL,
},},
	.redirs = NULL,
	.next = &(struct s_ast_simple_command){
	.args = &(struct s_ast_command_word){
	.word = "/bin/wc",
	.next = &(struct s_ast_command_word){
	.word = "-c",
	.next = NULL,
},},
	.redirs = &(struct s_ast_redirect){
	.op = AST_REDIR_OUT,
	.word = "/dev/stderr",
	.next = NULL,
},
	.next = &(struct s_ast_simple_command){
	.args = &(struct s_ast_command_word){
	.word = "/bin/wc",
	.next = NULL,
},
	.redirs = &(struct s_ast_redirect){
	.op = AST_REDIR_IN,
	.word = "test_pipeline_tmpout",
	.next = NULL,
},
	.next = NULL,
},},},},};

int	main(void)
{
	t_shenv		env;
	t_status	status;

	status = shenv_init(&env);
	if (status != S_OK)
		return (1);
	env.exit_code = -1;
	status = execute_pipeline(g_test_pipeline, &env);
	dprintf(STDERR_FILENO,
		"execute_pipeline returned internal status %d, exit code %d\n",
		status, env.exit_code);
	shenv_free(&env);
	return (env.exit_code);
}
