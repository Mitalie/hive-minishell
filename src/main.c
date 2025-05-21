/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:55:33 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/21 04:01:05 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "ast.h"
#include "parser.h"
#include "execute.h"
#include "env.h"
#include "status.h"

static t_status	minishell_do_line(t_env *env, int *exit_code)
{
	char					*line;
	struct s_ast_list_entry	*ast;
	t_status				status;

	line = readline("minishell> ");
	if (!line)
		return (S_EXIT_OK);
	status = parser_parse(line, &ast);
	if (status == S_OK && ast)
		add_history(line);
	free(line);
	if (status == S_OK)
		status = execute_list(ast, env, exit_code);
	free_ast(ast);
	if (status == S_EXIT_ERR || status == S_RESET_ERR)
		*exit_code = 1;
	if (status == S_RESET_SYNTAX)
		*exit_code = 2;
	if (status == S_RESET_SIGINT)
		*exit_code = 128 + SIGINT;
	return (status);
}

int	main(void)
{
	t_status	status;
	t_env		env;
	int			exit_code;

	exit_code = 0;
	status = env_init(&env);
	if (status != S_OK)
		return (1);
	while (status != S_EXIT_ERR && status != S_EXIT_OK)
		status = minishell_do_line(&env, &exit_code);
	env_free(&env);
	return (exit_code);
}
