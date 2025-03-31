/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:16:56 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/31 03:48:51 by josmanov         ###   ########.fr       */
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
		token.word_content = NULL;
	return (token);
}

/* Print AST structure for verification */
void	print_ast_structure(struct s_ast_list_entry *root, int indent)
{
	struct s_ast_list_entry		*entry;
	struct s_ast_simple_command	*cmd;
	struct s_ast_command_word	*arg;
	int							i;

	if (!root)
		return ;
	entry = root;
	while (entry)
	{
		i = 0;
		while (i++ < indent)
			printf("  ");
		if (entry->type == AST_LIST_GROUP)
		{
			printf("GROUP:\n");
			print_ast_structure(entry->group, indent + 1);
		}
		else
		{
			printf("COMMAND: ");
			cmd = entry->pipeline;
			if (cmd && cmd->args)
			{
				printf("%s", cmd->args->word);
				arg = cmd->args->next;
				while (arg)
				{
					printf(" %s", arg->word);
					arg = arg->next;
				}
			}
			printf("\n");
		}
		if (entry->next)
			printf("OPERATOR: %s\n", entry->next_op == AST_LIST_AND ? "&&" : "||");
		entry = entry->next;
	}
}

/* Alternative approach: duplicate words during parsing */
struct s_token	*duplicate_tokens(struct s_token *tokens)
{
	struct s_token	*result;
	int				i;
	int				j;

	result = malloc(sizeof(struct s_token) * 5);
	if (!result)
		return (NULL);
	i = 0;
	while (i < 5)
	{
		result[i].type = tokens[i].type;
		result[i].word_content = NULL;
		if (tokens[i].type == TOK_WORD && tokens[i].word_content)
		{
			result[i].word_content = strdup(tokens[i].word_content);
			if (!result[i].word_content)
			{
				j = 0;
				while (j < i)
					free(result[j++].word_content);
				free(result);
				return (NULL);
			}
		}
		i++;
	}
	return (result);
}

/* Custom function to clear token content but keep the structs */
void	clean_token_content(struct s_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].type != TOK_END)
	{
		if (tokens[i].type == TOK_WORD && tokens[i].word_content)
		{
			free(tokens[i].word_content);
			tokens[i].word_content = NULL;
		}
		i++;
	}
}

int	main(void)
{
	struct s_token			tokens[5];
	struct s_token			*current_ptr;
	struct s_ast_list_entry	*ast;
	struct s_token			*dup_tokens;
	int						i;

	i = 0;
	while (i < 5)
	{
		tokens[i].type = TOK_END;
		tokens[i].word_content = NULL;
		i++;
	}
	printf("=== Simple Parser Test ===\n\n");
	i = 0;
	tokens[i++] = create_test_token(TOK_WORD, "echo");
	tokens[i++] = create_test_token(TOK_WORD, "hello");
	tokens[i++] = create_test_token(TOK_END, NULL);
	printf("Input Tokens:\n");
	i = 0;
	while (tokens[i].type != TOK_END)
	{
		if (tokens[i].type == TOK_WORD)
			printf("WORD: %s\n", tokens[i].word_content);
		i++;
	}
	printf("END\n\n");
	dup_tokens = duplicate_tokens(tokens);
	if (!dup_tokens)
	{
		printf("Error: Failed to duplicate tokens\n");
		clean_token_content(tokens);
		return (1);
	}
	current_ptr = dup_tokens;
	ast = parse_command(&current_ptr);
	if (!ast)
	{
		printf("Error: Parsing failed\n");
		i = 0;
		while (tokens[i].type != TOK_END)
			free(tokens[i++].word_content);
		clean_token_content(dup_tokens);
		free(dup_tokens);
		return (1);
	}
	printf("AST Structure:\n");
	print_ast_structure(ast, 0);
	i = 0;
	while (tokens[i].type != TOK_END)
		free(tokens[i++].word_content);
	free(dup_tokens);
	free_ast(ast);
	printf("\nTest completed successfully!\n");
	return (0);
}
