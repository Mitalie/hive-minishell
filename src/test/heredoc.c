/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:32:56 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/12 22:49:52 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <readline/readline.h>

#include "ast.h"
#include "status.h"
#include "word.h"

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"

/* Declare internal structs and functions from parser and execute modules */

struct s_redir_fds
{
	int	in;
	int	out;
};

t_status	parser_read_heredoc(struct s_ast_redirect *redirect);
t_status	execute_redirect_heredoc(struct s_ast_redirect *redirect,
				struct s_redir_fds *fds);

/* Print test result */
static void	print_result(const char *name, bool passed)
{
	if (passed)
		printf("[%sPASS%s] %s%s%s\n", GREEN, RESET, CYAN, name, RESET);
	else
		printf("[%sFAIL%s] %s%s%s\n", RED, RESET, CYAN, name, RESET);
}

/* Print heredoc lines */
static void	print_heredoc_content(struct s_ast_command_word *lines)
{
	struct s_ast_command_word	*current;
	int							count;

	current = lines;
	count = 0;
	printf("\n%sHeredoc content:%s\n", YELLOW, RESET);
	printf("----------------------------------------\n");
	while (current)
	{
		printf("%s\n", current->word);
		current = current->next;
		count++;
	}
	printf("----------------------------------------\n");
	printf("Total lines: %d\n", count);
}

/* Free heredoc lines */
static void	handle_heredoc_lines(
	struct s_ast_command_word *lines,
	bool print_only)
{
	struct s_ast_command_word	*current;
	struct s_ast_command_word	*next;

	if (print_only)
	{
		print_heredoc_content(lines);
		return ;
	}
	current = lines;
	while (current)
	{
		next = current->next;
		free(current->word);
		free(current);
		current = next;
	}
}

/* Test heredoc execution */
static void	test_heredoc_execution(struct s_ast_redirect *redirect)
{
	t_status			status;
	struct s_redir_fds	fds;
	char				buffer[4096];
	ssize_t				bytes_read;

	printf("\n%sTesting heredoc execution%s\n", YELLOW, RESET);
	status = execute_redirect_heredoc(redirect, &fds);
	print_result("heredoc_temp_file_created", status == S_OK);
	if (status != S_OK)
		return ;
	bytes_read = read(fds.in, buffer, sizeof(buffer) - 1);
	if (bytes_read > 0)
		buffer[bytes_read] = '\0';
	else
		buffer[0] = '\0';
	print_result("heredoc_content_readable", bytes_read >= 0);
	printf("\n%sContent from file:%s\n", YELLOW, RESET);
	printf("----------------------------------------\n%s", buffer);
	printf("----------------------------------------\n");
	close(fds.in);
}

/* Setup heredoc test */
static char	*setup_heredoc_test(struct s_ast_redirect *redirect)
{
	char	*delimiter;
	char	*parsed;

	printf("\n%sTesting heredoc parser - INTERACTIVE TEST%s\n", YELLOW, RESET);
	printf("%sPlease enter a delimiter (e.g. EOF):%s ", MAGENTA, RESET);
	delimiter = readline("");
	if (!delimiter || !*delimiter)
	{
		printf("%sInvalid delimiter%s\n", RED, RESET);
		free(delimiter);
		return (NULL);
	}
	parsed = strdup(delimiter);
	word_heredoc_delimiter(parsed);
	printf("%sEnter lines. Type '%s' to end.%s\n", MAGENTA, parsed, RESET);
	free(parsed);
	redirect->op = AST_HEREDOC;
	redirect->word = delimiter;
	redirect->heredoc_lines = NULL;
	redirect->next = NULL;
	return (delimiter);
}

/* Test heredoc parser */
static void	test_heredoc_parser(void)
{
	struct s_ast_redirect	redirect;
	char					*delimiter;
	t_status				status;
	bool					success;

	delimiter = setup_heredoc_test(&redirect);
	if (!delimiter)
		return ;
	status = parser_read_heredoc(&redirect);
	success = (status == S_OK);
	print_result("heredoc_parser", success);
	if (!success)
	{
		free(delimiter);
		return ;
	}
	handle_heredoc_lines(redirect.heredoc_lines, true);
	test_heredoc_execution(&redirect);
	handle_heredoc_lines(redirect.heredoc_lines, false);
	free(delimiter);
}

/* Main function */
int	main(void)
{
	printf("%s\n=== INTERACTIVE HEREDOC TEST ===%s\n\n", YELLOW, RESET);
	test_heredoc_parser();
	printf("\n%sTest completed%s\n", GREEN, RESET);
	return (0);
}
