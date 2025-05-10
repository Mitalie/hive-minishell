/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:32:56 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/10 22:33:37 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <readline/readline.h>

#include "ast.h"
#include "parser.h"
#include "execute.h"
#include "libft.h"
#include "status.h"

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"

/* Function declarations */
enum e_parser_status	read_heredoc(struct s_ast_redirect *redirect);

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
	int		fd;
	bool	success;
	char	buffer[4096];
	ssize_t	bytes_read;

	printf("\n%sTesting heredoc execution%s\n", YELLOW, RESET);
	fd = process_heredoc(redirect);
	success = (fd != -1);
	print_result("heredoc_temp_file_created", success);
	if (!success)
		return ;
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	if (bytes_read > 0)
		buffer[bytes_read] = '\0';
	else
		buffer[0] = '\0';
	success = (bytes_read > 0);
	print_result("heredoc_content_readable", success);
	printf("\n%sContent from file:%s\n", YELLOW, RESET);
	printf("----------------------------------------\n%s", buffer);
	printf("----------------------------------------\n");
	close(fd);
}

/* Setup heredoc test */
static char	*setup_heredoc_test(struct s_ast_redirect *redirect)
{
	char	*delimiter;

	printf("\n%sTesting heredoc parser - INTERACTIVE TEST%s\n", YELLOW, RESET);
	printf("%sPlease enter a delimiter (e.g. EOF):%s ", MAGENTA, RESET);
	delimiter = readline("");
	if (!delimiter || !*delimiter)
	{
		printf("%sInvalid delimiter%s\n", RED, RESET);
		free(delimiter);
		return (NULL);
	}
	printf("%sEnter lines. Type '%s' to end.%s\n", MAGENTA, delimiter, RESET);
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
	enum e_parser_status	status;
	bool					success;

	delimiter = setup_heredoc_test(&redirect);
	if (!delimiter)
		return ;
	status = read_heredoc(&redirect);
	success = (status == PARSER_SUCCESS);
	print_result("heredoc_parser", success);
	if (!success || !redirect.heredoc_lines)
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
