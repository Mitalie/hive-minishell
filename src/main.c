/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:55:33 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/12 20:28:40 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "ast.h"
#include "parser.h"
#include "execute.h"
#include "env.h"
#include "status.h"

/*
	TODO: exit status for errors
	TODO: message and cleanup for malloc and other fatal errors
*/

int	main(void)
{
	char					*line;
	struct s_ast_list_entry	*ast;
	t_status				status;
	t_env					env;

	status = env_init(&env);
	if (status != S_OK)
		return (1);
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break ;
		status = parser_parse(line, &ast);
		if (status == S_OK && ast)
			add_history(line);
		free(line);
		if (status == S_OK)
			execute_list(ast, &env);
		free_ast(ast);
		if (status == S_EXIT_ERR)
			return (1);
	}
	env_free(&env);
}
