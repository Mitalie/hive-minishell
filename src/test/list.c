/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 23:14:19 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/04 20:14:39 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "ast.h"
#include "execute.h"
#include "shenv.h"
#include "status.h"

/*
	/bin/echo "First command" &&
	/bin/echo "Second command (after success)" ||
	/bin/echo "This should not run.."
*/
struct s_ast_list_entry	*g_test_list
	= &(struct s_ast_list_entry){
	.type = AST_LIST_PIPELINE,
	.pipeline = &(struct s_ast_simple_command){
	.args = &(struct s_ast_command_word){
	.word = "/bin/echo",
	.next = &(struct s_ast_command_word){
	.word = "First command ran",
	.next = NULL,
},},
	.redirs = NULL,
	.next = NULL,
},
	.next_op = AST_LIST_AND,
	.next = &(struct s_ast_list_entry){
	.type = AST_LIST_PIPELINE,
	.pipeline = &(struct s_ast_simple_command){
	.args = &(struct s_ast_command_word){
	.word = "/bin/echo",
	.next = &(struct s_ast_command_word){
	.word = "Second command ran(after success)",
	.next = NULL,
},},
	.redirs = NULL,
	.next = NULL,
},
	.next_op = AST_LIST_OR,
	.next = &(struct s_ast_list_entry){
	.type = AST_LIST_PIPELINE,
	.pipeline = &(struct s_ast_simple_command){
	.args = &(struct s_ast_command_word){
	.word = "/bin/echo",
	.next = &(struct s_ast_command_word){
	.word = "This should not run",
	.next = NULL,
},},
	.redirs = NULL,
	.next = NULL,
},
	.next = NULL,
},},};

int	main(void)
{
	t_shenv		env;
	t_status	status;
	int			exit_code;

	status = shenv_init(&env);
	if (status != S_OK)
		return (1);
	exit_code = -1;
	status = execute_list(g_test_list, &env, &exit_code);
	dprintf(STDERR_FILENO,
		"execute_list returned internal status %d, exit code %d\n",
		status, exit_code);
	shenv_free(&env);
}
