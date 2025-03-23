/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:14:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/23 14:32:04 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	Test for create_simple_command function.
	Creates a simple command and verifies its fields are properly initialized.
*/
static int	test_create_simple_command(void)
{
	struct s_ast_simple_command	*cmd;
	int							result;

	printf("Testing create_simple_command...\n");
	cmd = create_simple_command();
	if (!cmd)
	{
		printf("FAIL: create_simple_command returned NULL\n");
		return (0);
	}
	result = (cmd->args == NULL && cmd->redirs == NULL && cmd->next == NULL);
	if (!result)
		printf("FAIL: fields not properly initialized\n");
	else
		printf("PASS: simple command created correctly\n");
	free(cmd);
	return (result);
}

/*
	Test pipeline parsing functionality.
	Creates tokens for a simple pipeline and verifies parsing works correctly.
*/
static int	test_parse_pipeline(void)
{
	struct s_token				tokens[5];
	struct s_token				*tokens_ptr;
	struct s_ast_simple_command	*pipeline;
	int							result;

	printf("Testing parse_pipeline...\n");
	/* Setup tokens for "ls | wc" */
	tokens[0].type = TOK_WORD;
	tokens[0].word_content = strdup("ls");
	tokens[1].type = TOK_PIPE;
	tokens[1].word_content = NULL;
	tokens[2].type = TOK_WORD;
	tokens[2].word_content = strdup("wc");
	tokens[3].type = TOK_END;
	tokens[3].word_content = NULL;
	tokens_ptr = tokens;
	pipeline = parse_pipeline(&tokens_ptr);
	if (!pipeline)
	{
		printf("FAIL: parse_pipeline returned NULL\n");
		free(tokens[0].word_content);
		free(tokens[2].word_content);
		return (0);
	}
	/* Check first command is "ls" */
	result = pipeline->args && strcmp(pipeline->args->word, "ls") == 0;
	if (!result)
	{
		printf("FAIL: first command not parsed correctly\n");
		free_simple_command(pipeline);
		return (0);
	}
	/* Check second command is "wc" */
	result = pipeline->next && pipeline->next->args
		&& strcmp(pipeline->next->args->word, "wc") == 0;
	if (!result)
		printf("FAIL: second command not parsed correctly\n");
	else
		printf("PASS: pipeline parsed correctly\n");
	free_simple_command(pipeline);
	return (result);
}

int	main(void)
{
	int	passed;

	passed = 0;
	passed += test_create_simple_command();
	passed += test_parse_pipeline();
	printf("\nTests passed: %d/2\n", passed);
	return (passed == 2 ? 0 : 1);
}
