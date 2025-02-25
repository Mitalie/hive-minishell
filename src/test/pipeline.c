/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:35:02 by amakinen          #+#    #+#             */
/*   Updated: 2025/02/25 19:36:07 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "ast.h"
#include "execute.h"

/*
	/bin/echo -e hello\n world! |
	/bin/grep world |
	/bin/wc -c
*/
struct s_ast_simple_command	*g_test_pipeline
	= &(struct s_ast_simple_command){
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
	.redirs = NULL,
	.next = NULL
},},};

int	main(void)
{
	execute_pipeline(g_test_pipeline);
	write(STDERR_FILENO, "execute_pipeline returned\n", 26);
}
