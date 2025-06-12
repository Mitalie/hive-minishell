/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:55:33 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/12 19:00:46 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>

#include "ast.h"
#include "execute.h"
#include "input.h"
#include "parser.h"
#include "shenv.h"
#include "signals.h"
#include "status.h"

/*
	Read input for one (complex) command and process it.

	Clear previously caught SIGINT before starting - user expects a fresh
	prompt which we're going to do here anyway.
*/
static t_status	minishell_do_line(t_shenv *env)
{
	t_status				status;
	char					*line;
	struct s_ast_list_entry	*ast;

	line = NULL;
	ast = NULL;
	signals_check_sigint(true);
	status = input_get_line(&line, "minishell> ");
	if (status == S_OK && !line)
		return (S_EXIT_OK);
	if (status == S_OK)
		status = parser_parse(line, &ast);
	if (status == S_OK && ast)
		input_add_history(line);
	free(line);
	if (status == S_OK)
		status = execute_list(ast, env);
	free_ast(ast);
	status_set_exit_code(status, env);
	return (status);
}

int	main(void)
{
	t_status	status;
	t_shenv		env;

	signals_set_handlers();
	status = shenv_init(&env);
	if (status != S_OK)
		return (1);
	while (status != S_EXIT_ERR && status != S_EXIT_OK)
		status = minishell_do_line(&env);
	input_clear_history();
	if (!env.is_child)
		status_warn("exiting", NULL, 0);
	shenv_free(&env);
	return (env.exit_code);
}
