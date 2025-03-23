/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_group.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:16:56 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/23 14:56:10 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Create a token with specified type and content */
struct s_token	create_test_token(enum e_token type, char *content)
{
	struct s_token	token;

	token.type = type;
	if (content && type == TOK_WORD)
	{
		token.word_content = strdup(content);
		if (!token.word_content)
		{
			fprintf(stderr, "Error: Memory allocation failed\n");
			exit(1);
		}
	}
	else
	{
		token.word_content = NULL;
	}
	return (token);
}

/* Print AST structure for verification */
void	print_ast_structure(struct s_ast_list_entry *root, int indent)
{
	if (!root)
		return ;
	struct s_ast_list_entry	*entry = root;
	while (entry)
	{
		for (int i = 0; i < indent; i++)
			printf("  ");
		if (entry->type == AST_LIST_GROUP)
		{
			printf("GROUP:\n");
			print_ast_structure(entry->group, indent + 1);
		}
		else
		{
			printf("COMMAND: ");
			struct s_ast_simple_command	*cmd = entry->pipeline;
			if (cmd && cmd->args)
			{
				printf("%s", cmd->args->word);
				struct s_ast_command_word	*arg = cmd->args->next;
				while (arg)
				{
					printf(" %s", arg->word);
					arg = arg->next;
				}
			}
			printf("\n");
		}
		if (entry->next)
		{
			printf("OPERATOR: %s\n", entry->next_op == AST_LIST_AND ? "&&" : "||");
		}
		entry = entry->next;
	}
}

/* Modified version that only frees token structures but not word_content */
void	free_tokens(struct s_token *tokens)
{
	for (int i = 0; tokens[i].type != TOK_END; i++)
	{
		if (tokens[i].type == TOK_WORD && tokens[i].word_content)
		{
			tokens[i].word_content = NULL; // Just nullify the pointer
		}
	}
}

/* Alternative approach: duplicate words during parsing */
struct s_token	*duplicate_tokens(struct s_token *tokens)
{
	struct s_token	*result = malloc(sizeof(struct s_token) * 5);
	if (!result)
		return (NULL);
	for (int i = 0; i < 5; i++)
	{
		result[i].type = tokens[i].type;
		result[i].word_content = NULL;
		if (tokens[i].type == TOK_WORD && tokens[i].word_content)
		{
			result[i].word_content = strdup(tokens[i].word_content);
			if (!result[i].word_content)
			{
				// Clean up on failure
				for (int j = 0; j < i; j++)
				{
					free(result[j].word_content);
				}
				free(result);
				return (NULL);
			}
		}
	}
	return (result);
}

int	main(void)
{
	struct s_token			tokens[5];
	struct s_token			*current_ptr;
	struct s_ast_list_entry	*ast;

	/* Initialize tokens array */
	for (int i = 0; i < 5; i++)
	{
		tokens[i].type = TOK_END;
		tokens[i].word_content = NULL;
	}
	printf("=== Simple Parser Test ===\n\n");
	/* Create test tokens for: echo hello */
	int	i = 0;
	tokens[i++] = create_test_token(TOK_WORD, "echo");
	tokens[i++] = create_test_token(TOK_WORD, "hello");
	tokens[i++] = create_test_token(TOK_END, NULL);
	/* Print the tokens */
	printf("Input Tokens:\n");
	for (i = 0; tokens[i].type != TOK_END; i++)
	{
		if (tokens[i].type == TOK_WORD)
			printf("WORD: %s\n", tokens[i].word_content);
	}
	printf("END\n\n");
	/* Create a duplicated set of tokens for parsing */
	struct s_token	*dup_tokens = duplicate_tokens(tokens);
	if (!dup_tokens)
	{
		printf("Error: Failed to duplicate tokens\n");
		free_tokens(tokens);
		return (1);
	}
	/* Parse the duplicated tokens into an AST */
	current_ptr = dup_tokens;
	ast = parse_command(&current_ptr);
	if (!ast)
	{
		printf("Error: Parsing failed\n");
		free_tokens(tokens);
		free_tokens(dup_tokens);
		free(dup_tokens);
		return (1);
	}
	/* Print and verify the AST structure */
	printf("AST Structure:\n");
	print_ast_structure(ast, 0);
	/* Clean up */
	free_tokens(tokens);  // Free token structs and original word_content
	free(dup_tokens);     // Free duplicated token structs (content handled by AST)
	free_ast(ast);        // Free AST including duplicated word_content
	printf("\nTest completed successfully!\n");
	return (0);
}
