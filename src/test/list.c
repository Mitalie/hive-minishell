/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 23:14:19 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/01 22:18:51 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

#include "ast.h"
#include "execute.h"
#include "env.h"
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
	t_env		env;
	t_status	env_status;
	int			status;

	env_status = env_init(&env);
	if (env_status != S_OK)
		return (1);
	status = execute_list(g_test_list, &env);
	dprintf(STDERR_FILENO, "execute_list returned status: %d\n", status);
	env_free(&env);
}
