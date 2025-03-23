/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:38:26 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/23 14:32:01 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "parser.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Print the content of a simple command
void	print_simple_command(struct s_ast_simple_command *cmd)
{
	struct s_ast_command_word	*arg;
	struct s_ast_redirect		*redir;

	if (!cmd)
	{
		printf("Command is NULL\n");
		return ;
	}
	printf("Simple Command:\n");
	printf("  Arguments:\n");
	arg = cmd->args;
	while (arg)
	{
		printf("    %s\n", arg->word);
		arg = arg->next;
	}
	printf("  Redirections:\n");
	redir = cmd->redirs;
	while (redir)
	{
		printf("    ");
		if (redir->op == AST_REDIR_IN)
			printf("< ");
		else if (redir->op == AST_REDIR_OUT)
			printf("> ");
		else if (redir->op == AST_REDIR_APP)
			printf(">> ");
		else if (redir->op == AST_HEREDOC)
			printf("<< ");
		printf("%s\n", redir->word);
		redir = redir->next;
	}
}

// Mock execution function
void	execute_simple_command(struct s_ast_simple_command *cmd)
{
	if (cmd && cmd->args)
	{
		printf("Executing command: %s\n", cmd->args->word);
		// Just print the command rather than actually executing it
	}
}

int	main(void)
{
	// Test 1: Simple command with arguments
	printf("=== Test 1: Simple command with arguments ===\n");
	struct s_token	tokens1[] = {
		{TOK_WORD, strdup("/bin/echo")},
		{TOK_WORD, strdup("hello")},
		{TOK_WORD, strdup("world!")},
		{TOK_END, NULL}
	};
	struct s_token				*token_ptr1 = tokens1;
	struct s_ast_simple_command	*cmd1 = parse_simple_command(&token_ptr1);
	if (cmd1)
	{
		print_simple_command(cmd1);
		execute_simple_command(cmd1);
	}
	else
	{
		printf("Parsing failed.\n");
	}
	// Test 2: Command with redirections
	printf("\n=== Test 2: Command with redirections ===\n");
	struct s_token	tokens2[] = {
		{TOK_WORD, strdup("cat")},
		{TOK_REDIR_IN, NULL},
		{TOK_WORD, strdup("input.txt")},
		{TOK_REDIR_OUT, NULL},
		{TOK_WORD, strdup("output.txt")},
		{TOK_END, NULL}
	};
	struct s_token				*token_ptr2 = tokens2;
	struct s_ast_simple_command	*cmd2 = parse_simple_command(&token_ptr2);
	if (cmd2)
	{
		print_simple_command(cmd2);
		execute_simple_command(cmd2);
	}
	else
	{
		printf("Parsing failed.\n");
	}
	return 0;
}
