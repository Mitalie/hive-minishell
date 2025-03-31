/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:38:44 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 14:36:56 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "parser.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

void	free_tokens(struct s_token *tokens)
{
	if (!tokens)
		return ;
	for (int i = 0; tokens[i].type != TOK_END; i++)
	{
		if (tokens[i].type == TOK_WORD && tokens[i].word_content)
		{
			free(tokens[i].word_content); // Free the word content
			tokens[i].word_content = NULL; // Nullify the pointer
		}
	}
	free(tokens); // Free the tokens array itself
}

/* Print the redirect node details */
void	print_redirect(struct s_ast_redirect *redir)
{
	if (!redir)
	{
		printf("Redirect is NULL\n");
		return ;
	}
	printf("Redirect:\n");
	printf("  Operation: ");
	if (redir->op == AST_REDIR_IN)
		printf("Input (<)\n");
	else if (redir->op == AST_REDIR_OUT)
		printf("Output (>)\n");
	else if (redir->op == AST_REDIR_APP)
		printf("Append (>>)\n");
	else if (redir->op == AST_HEREDOC)
		printf("Heredoc (<<)\n");
	printf("  File/Word: %s\n", redir->word);
}

int	main(void)
{
	struct s_ast_redirect	*input_redir;
	struct s_ast_redirect	*output_redir;
	struct s_ast_redirect	*append_redir;
	struct s_ast_redirect	*heredoc_redir;
	struct s_ast_redirect	*current;
	int						i;

	printf("=== Test 1: Input Redirect ===\n");
	input_redir = create_redirect(AST_REDIR_IN, strdup("input.txt"));
	print_redirect(input_redir);
	printf("\n=== Test 2: Output Redirect ===\n");
	output_redir = create_redirect(AST_REDIR_OUT, strdup("output.txt"));
	print_redirect(output_redir);
	printf("\n=== Test 3: Append Redirect ===\n");
	append_redir = create_redirect(AST_REDIR_APP, strdup("append.txt"));
	print_redirect(append_redir);
	printf("\n=== Test 4: Heredoc Redirect ===\n");
	heredoc_redir = create_redirect(AST_HEREDOC, strdup("EOF"));
	print_redirect(heredoc_redir);
	printf("\n=== Test 5: Chained Redirects ===\n");
	input_redir->next = output_redir;
	output_redir->next = append_redir;
	append_redir->next = heredoc_redir;
	current = input_redir;
	i = 1;
	while (current)
	{
		printf("Redirect %d:\n", i++);
		printf("  Operation: ");
		if (current->op == AST_REDIR_IN)
			printf("Input (<)\n");
		else if (current->op == AST_REDIR_OUT)
			printf("Output (>)\n");
		else if (current->op == AST_REDIR_APP)
			printf("Append (>>)\n");
		else if (current->op == AST_HEREDOC)
			printf("Heredoc (<<)\n");
		printf("  File/Word: %s\n", current->word);
		current = current->next;
	}
	free_redirect(input_redir);
	return (0);
}
