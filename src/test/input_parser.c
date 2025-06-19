/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:17:11 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/20 02:41:55 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "ast.h"
#include "input.h"
#include "status.h"
#include "parser.h"

#define INDENT_STEP 4

static char	*redir_op_sym(enum e_ast_redirect_op redir_op)
{
	if (redir_op == AST_REDIR_IN)
		return ("<");
	else if (redir_op == AST_REDIR_OUT)
		return (">");
	else if (redir_op == AST_REDIR_APP)
		return (">>");
	else if (redir_op == AST_HEREDOC)
		return ("<<");
	else
		return (NULL);
}

static void	print_command(struct s_ast_simple_command *command, int indent)
{
	struct s_ast_command_word	*arg;
	struct s_ast_redirect		*redir;
	char						*redir_sym;

	printf("%*sSimple command:\n", indent, "");
	indent += INDENT_STEP;
	arg = command->args;
	while (arg)
	{
		printf("%*sArg: [%s]\n", indent, "", arg->word);
		arg = arg->next;
	}
	redir = command->redirs;
	while (redir)
	{
		redir_sym = redir_op_sym(redir->op);
		if (redir_sym)
			printf("%*sRedirect: %s [%s]\n",
				indent, "", redir_sym, redir->word);
		else
			printf("%*sRedirect: \e[91mBad redir op %d [%s]\n",
				indent, "", redir->op, redir->word);
		redir = redir->next;
	}
}

static void	print_pipeline(struct s_ast_simple_command *pipeline, int indent)
{
	printf("%*sPipeline:\n", indent, "");
	indent += INDENT_STEP;
	while (1)
	{
		print_command(pipeline, indent);
		if (!pipeline->next)
			break ;
		pipeline = pipeline->next;
	}
}

static void	print_list(struct s_ast_list_entry *list, int indent)
{
	printf("%*sList:\n", indent, "");
	indent += INDENT_STEP;
	while (1)
	{
		if (list->type == AST_LIST_GROUP)
			print_list(list->group, indent);
		else if (list->type == AST_LIST_PIPELINE)
			print_pipeline(list->pipeline, indent);
		else
			printf("%*s\e[91mBad list entry type %d\e[m\n",
				indent, "", list->type);
		if (!list->next)
			break ;
		if (list->next_op == AST_LIST_AND)
			printf("%*s&&\n", indent, "");
		else if (list->next_op == AST_LIST_OR)
			printf("%*s||\n", indent, "");
		else
			printf("%*s\e[91mBad list op %d\e[m\n", indent, "", list->next_op);
		list = list->next;
	}
}

int	main(void)
{
	t_status				status;
	t_input					input;
	struct s_ast_list_entry	*root;
	char					*line;

	input_init(&input);
	while (1)
	{
		line = readline("input parser> ");
		if (!line)
			break ;
		status = parser_parse(&input, line, &root);
		if (status == S_RESET_SYNTAX || status == S_RESET_SIGINT)
			printf("try again\n");
		else if (status != S_OK)
			return (1);
		else if (!root)
			printf("empty command\n");
		else
			print_list(root, 0);
		ast_free(root);
		free(line);
	}
	input_free(&input);
	printf("done\n");
}
