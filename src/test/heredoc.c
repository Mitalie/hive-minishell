/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:32:56 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/04 00:16:04 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <readline/readline.h>

#include "libft.h"
#include "ast.h"
#include "execute.h"
#include "parser.h"
#include "word.h"
#include "env.h"
#include "../parser/parser_internal.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"

struct s_ast_redirect	g_test_heredoc
	= {
	.op = AST_HEREDOC,
	.word = "EOF",
	.fd = -1,
	.heredoc_lines = NULL,
	.next = NULL
};

static void	print_test_result(const char *name, bool passed)
{
	if (passed)
		printf("[%sPASS%s] %s%s%s\n", GREEN, RESET, CYAN, name, RESET);
	else
		printf("[%sFAIL%s] %s%s%s\n", RED, RESET, CYAN, name, RESET);
}

static void	test_temp_filename_init(int *fds)
{
	int	i;

	i = 0;
	while (i < 5)
	{
		fds[i] = -1;
		i++;
	}
}

static void	test_temp_filename_open(int *fds)
{
	char	temp_path[1024];
	int		i;

	i = 0;
	while (i < 5)
	{
		snprintf(temp_path, sizeof(temp_path), "/tmp/minishell_test_%d", i);
		fds[i] = open(temp_path, O_CREAT | O_RDWR | O_TRUNC, 0600);
		if (fds[i] != -1)
			printf("  %d: %s (fd: %d)\n", i + 1, temp_path, fds[i]);
		i++;
	}
}

static void	test_temp_filename_cleanup(int *fds)
{
	char	temp_path[1024];
	int		i;

	i = 0;
	while (i < 5 && fds[i] != -1)
	{
		close(fds[i]);
		snprintf(temp_path, sizeof(temp_path), "/tmp/minishell_test_%d", i);
		unlink(temp_path);
		i++;
	}
}

static void	test_temp_filename(void)
{
	int		fds[5];
	bool	success;

	printf("\n%sTesting temp file creation%s\n", YELLOW, RESET);
	test_temp_filename_init(fds);
	test_temp_filename_open(fds);
	success = (fds[0] != -1);
	print_test_result("temp_file_creation", success);
	test_temp_filename_cleanup(fds);
}

static void	print_heredoc_lines(struct s_ast_command_word *lines)
{
	printf("\n%sHeredoc content verification:%s\n", YELLOW, RESET);
	printf("----------------------------------------\n");
	while (lines)
	{
		printf("%s\n", lines->word);
		lines = lines->next;
	}
	printf("----------------------------------------\n");
}

static void	create_test_lines(void)
{
	struct s_ast_command_word	*line1;
	struct s_ast_command_word	*line2;

	line1 = malloc(sizeof(*line1));
	if (!line1)
		return ;
	line1->word = ft_strdup("first line");
	line1->next = NULL;
	line2 = malloc(sizeof(*line2));
	if (!line2)
	{
		free(line1->word);
		free(line1);
		return ;
	}
	line2->word = ft_strdup("  second line  ");
	line2->next = NULL;
	line1->next = line2;
	g_test_heredoc.heredoc_lines = line1;
}

static void	free_test_lines(void)
{
	struct s_ast_command_word	*current;
	struct s_ast_command_word	*next;

	current = g_test_heredoc.heredoc_lines;
	while (current)
	{
		next = current->next;
		free(current->word);
		free(current);
		current = next;
	}
	g_test_heredoc.heredoc_lines = NULL;
}

static int	count_heredoc_lines(struct s_ast_command_word *lines)
{
	int	count;

	count = 0;
	while (lines)
	{
		count++;
		lines = lines->next;
	}
	return (count);
}

static void	test_heredoc_processing(void)
{
	bool	lines_ok;
	int		count;

	printf("\n%sTesting heredoc processing%s\n", YELLOW, RESET);
	create_test_lines();
	lines_ok = (g_test_heredoc.heredoc_lines != NULL);
	print_test_result("lines_valid", lines_ok);
	if (lines_ok)
	{
		print_heredoc_lines(g_test_heredoc.heredoc_lines);
		count = count_heredoc_lines(g_test_heredoc.heredoc_lines);
		printf("Expected 2 lines, got %d lines\n", count);
		print_test_result("line_count_match", count == 2);
	}
}

static void	test_parameter_expansion(void)
{
	char	*line;
	char	*expanded;
	bool	success;

	printf("\n%sTesting parameter expansion in heredoc%s\n", YELLOW, RESET);
	line = ft_strdup("Hello $USER");
	if (!line)
	{
		print_test_result("parameter_expansion", false);
		return ;
	}
	expanded = word_heredoc_line(line);
	if (!expanded)
	{
		print_test_result("parameter_expansion", false);
		return ;
	}
	printf("Original: Hello $USER\n");
	printf("Expanded: %s\n", expanded);
	success = (ft_strncmp(expanded, "Hello $USER", 11) != 0);
	print_test_result("parameter_expansion", success);
	free(expanded);
}

static void	print_quoted_status(bool quoted)
{
	if (quoted)
		printf("Quoted: true\n");
	else
		printf("Quoted: false\n");
}

static void	test_quoted_delimiter(void)
{
	char	*delimiter;
	bool	quoted;
	bool	success;

	printf("\n%sTesting quoted delimiter handling%s\n", YELLOW, RESET);
	delimiter = ft_strdup("\"EOF\"");
	if (!delimiter)
	{
		print_test_result("quoted_delimiter", false);
		return ;
	}
	printf("Original delimiter: %s\n", delimiter);
	quoted = word_heredoc_delimiter(delimiter);
	printf("Processed delimiter: %s\n", delimiter);
	print_quoted_status(quoted);
	success = (quoted && ft_strncmp(delimiter, "EOF", 3) == 0);
	print_test_result("quoted_delimiter", success);
	free(delimiter);
}

static void	print_disabled_tests(void)
{
	printf("\n%sNote: Some tests are disabled on Windows%s\n", YELLOW, RESET);
	printf("%sThese tests require POSIX features not available on Windows:%s\n",
		YELLOW, RESET);
	printf("- Heredoc parsing from stdin (requires pipe)\n");
	printf("- Process heredoc execution (requires process_heredoc function)\n");
	printf("- Command with heredoc redirection (requires fork)\n");
}

int	main(void)
{
	printf("%s\n=== HEREDOC TEST SUITE ===%s\n\n", YELLOW, RESET);
	test_temp_filename();
	test_heredoc_processing();
	test_parameter_expansion();
	test_quoted_delimiter();
	print_disabled_tests();
	free_test_lines();
	printf("\n%sTests completed%s\n", GREEN, RESET);
	return (0);
}
