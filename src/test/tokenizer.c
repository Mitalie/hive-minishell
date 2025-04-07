/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:42:39 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/07 16:26:16 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "tokenizer.h"

static const char		*g_tok_names[] = {
[TOK_WORD] = "WORD",
[TOK_REDIR_IN] = "REDIR_IN",
[TOK_REDIR_OUT] = "REDIR_OUT",
[TOK_REDIR_APP] = "REDIR_APP",
[TOK_HEREDOC] = "HEREDOC",
[TOK_PIPE] = "PIPE",
[TOK_AND] = "AND",
[TOK_OR] = "OR",
[TOK_GROUP_START] = "GROUP_START",
[TOK_GROUP_END] = "GROUP_END",
[TOK_END] = "END",
};

void	debug_print_token(t_token token)
{
	if (token.type == TOK_WORD)
		printf("WORD(%s)\n", token.word_content);
	else
		printf("%s\n", g_tok_names[token.type]);
}

int	main(void)
{
	t_token				token;
	t_tokenizer_state	ts;
	char				*line;

	line = NULL;
	token.type = TOK_END;
	while (1)
	{
		if (token.type == TOK_END)
		{
			free(line);
			line = readline("tokenizer> ");
			if (!line)
				break ;
			ts.line_pos = line;
		}
		token = tokenizer_get_next(&ts);
		debug_print_token(token);
		free(token.word_content);
	}
	return (0);
}
