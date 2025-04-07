/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:55:33 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/07 17:24:06 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "ast.h"
#include "parser.h"
#include "execute.h"

/*
	TODO: exit status for errors
	TODO: message and cleanup for malloc and other fatal errors
*/

int	main(void)
{
	char					*line;
	struct s_ast_list_entry	*ast;
	enum e_parser_status	status;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break ;
		status = parser_parse(line, &ast);
		free(line);
		if (status == PARSER_SUCCESS)
			execute_list(ast);
		free_ast(ast);
		if (status == PARSER_ERR_MALLOC)
			return (1);
	}
}
