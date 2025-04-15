/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:55:33 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/13 14:43:07 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "ast.h"
#include "parser.h"
#include "execute.h"

#include "shell.h"

/*
	TODO: exit status for errors
	TODO: message and cleanup for malloc and other fatal errors
*/

t_shell	g_shell;

int	main(void)
{
	char					*line;
	struct s_ast_list_entry	*ast;
	enum e_parser_status	status;

	g_shell.env = env_init();
	if (!g_shell.env)
		return (1);
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break ;
		status = parser_parse(line, &ast);
		if (status == PARSER_SUCCESS && ast)
			add_history(line);
		free(line);
		if (status == PARSER_SUCCESS)
			execute_list(ast);
		free_ast(ast);
		if (status == PARSER_ERR_MALLOC)
			return (1);
	}
	env_free(g_shell.env);
}
