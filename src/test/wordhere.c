/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordhere.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:51:26 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/04 23:30:19 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "shenv.h"
#include "status.h"
#include "word.h"

#define RLYELLOW "\001\e[33m\002"
#define RLRESET "\001\e[0m\002"
#define RED "\e[31m"
#define BRED "\e[91m"
#define GREEN "\e[32m"
#define RESETNL "\e[0m\n"

static void	error_exit(char *msg)
{
	printf(RED "%s" RESETNL, msg);
	exit(1);
}

static void	heredoc_loop(char *delimiter, bool quoted, t_shenv *env)
{
	char	*line;

	while (true)
	{
		line = readline(RLYELLOW "Heredoc: " RLRESET);
		if (!line)
			error_exit("Readline returned null");
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			printf(GREEN "Found delimiter, exiting" RESETNL);
			return ;
		}
		if (quoted)
			printf(GREEN "Received line >" BRED "%s" GREEN "<" RESETNL, line);
		else if (word_heredoc_line(&line, env) == S_OK)
			printf(GREEN "Expanded line >" BRED "%s" GREEN "<" RESETNL, line);
		else
		{
			free(line);
			error_exit("Error in word_heredoc_line");
		}
		free(line);
	}
}

int	main(void)
{
	char	*delimiter;
	bool	quoted;
	t_shenv	env;

	shenv_init(&env);
	delimiter = readline(RLYELLOW "Heredoc delimiter: " RLRESET);
	if (!delimiter)
		error_exit("Readline returned null");
	quoted = word_heredoc_delimiter(delimiter);
	printf(GREEN "Looking for delimiter >" BRED "%s" GREEN "<" RESETNL,
		delimiter);
	heredoc_loop(delimiter, quoted, &env);
	free(delimiter);
}
