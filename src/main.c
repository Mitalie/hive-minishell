/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:55:33 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/18 18:23:48 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

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

	If not interactive, exit with a caught SIGINT or any S_RESET_* status.
*/
static t_status	minishell_do_line(t_input *input, t_shenv *env)
{
	t_status				status;
	char					*line;
	struct s_ast_list_entry	*ast;

	line = NULL;
	ast = NULL;
	if (signals_check_sigint(true) && !isatty(STDIN_FILENO))
		return (S_EXIT_OK);
	status = input_get_line(input, &line, "minishell> ");
	if (status == S_OK && !line)
		return (S_EXIT_OK);
	if (status == S_OK)
		status = parser_parse(input, line, &ast);
	if (status == S_OK && ast)
		input_add_history(line);
	free(line);
	if (status == S_OK)
		status = execute_list(ast, env);
	ast_free(ast);
	status_set_exit_code(status, env);
	if (!isatty(STDIN_FILENO) && (status == S_RESET_ERR
			|| status == S_RESET_SIGINT || status == S_RESET_SYNTAX))
		status = S_EXIT_OK;
	return (status);
}

int	main(void)
{
	t_status	status;
	t_shenv		env;
	t_input		input;

	signals_set_handlers();
	input_init(&input);
	status = shenv_init(&env);
	if (status != S_OK)
		return (1);
	while (status != S_EXIT_ERR && status != S_EXIT_OK && !env.is_child)
		status = minishell_do_line(&input, &env);
	input_clear_history();
	if (!env.is_child)
		status_warn("exiting", NULL, 0);
	shenv_free(&env);
	return (env.exit_code);
}
