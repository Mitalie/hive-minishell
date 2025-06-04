/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:55:33 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/04 20:14:39 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "ast.h"
#include "parser.h"
#include "execute.h"
#include "shenv.h"
#include "status.h"

static t_status	minishell_do_line(t_shenv *env, int *exit_code)
{
	t_status				status;
	char					*line;
	struct s_ast_list_entry	*ast;

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
	status_set_exit_code(status, exit_code);
	return (status);
}

int	main(void)
{
	t_status	status;
	t_shenv		env;
	int			exit_code;

	exit_code = 0;
	status = shenv_init(&env);
	if (status != S_OK)
		return (1);
	while (status != S_EXIT_ERR && status != S_EXIT_OK)
		status = minishell_do_line(&env, &exit_code);
	shenv_free(&env);
	return (exit_code);
}
