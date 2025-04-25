/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:32:56 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/24 16:30:32 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../parser/parser_internal.h"
#include "../heredoc/heredoc_internal.h"
#include "ast.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

struct s_ast_redirect	g_test_heredoc = {
	.op = AST_HEREDOC,
	.word = "EOF",
	.fd = -1,
	.next = NULL
};

static void	print_test_result(const char *name, bool passed)
{
	if (passed)
		printf("[%sPASS%s] %s%s%s\n", GREEN, RESET, CYAN, name, RESET);
	else
		printf("[%sFAIL%s] %s%s%s\n", RED, RESET, CYAN, name, RESET);
}

static void	test_temp_filename(void)
{
	char	*names[5];
	bool	success;
	int		i;
	int		j;

	printf("\n%sTesting temp filename generation%s\n", YELLOW, RESET);
	success = true;
	i = 0;
	while (i < 5)
	{
		names[i] = generate_temp_filename();
		if (!names[i])
		{
			printf("%s  ✖ Failed to generate filename #%d\n%s", RED, i + 1, RESET);
			success = false;
			break ;
		}
		printf("  %d: %s\n", i + 1, names[i]);
		j = 0;
		while (j < i)
		{
			if (strcmp(names[i], names[j]) == 0)
			{
				printf("%s  ✖ Duplicate filename generated\n%s", RED, RESET);
				success = false;
			}
			j++;
		}
		i++;
	}
	print_test_result("temp_filename_uniqueness", success);
	i = 0;
	while (i < 5)
	{
		free(names[i]);
		i++;
	}
}

static void	verify_heredoc_content(int fd)
{
	char		buffer[1024];
	ssize_t		bytes;
	bool		content_ok;
	bool		size_ok;
	const char	*expected;

	expected = "first line\n  second line  \n";
	bytes = read(fd, buffer, sizeof(buffer) - 1);
	if (bytes > 0)
		buffer[bytes] = '\0';
	else
		buffer[0] = '\0';
	content_ok = (bytes > 0) && (strcmp(buffer, expected) == 0);
	size_ok = (bytes >= 0 && (size_t)bytes == strlen(expected));
	printf("\n%sHeredoc content verification:%s\n", YELLOW, RESET);
	printf("----------------------------------------\n");
	printf("%s", buffer);
	printf("----------------------------------------\n");
	printf("Expected %zu bytes, got %zd bytes\n", strlen(expected), bytes);
	print_test_result("content_match", content_ok);
	print_test_result("size_match", size_ok);
}

static void	test_heredoc_processing(void)
{
	int						dev_null;
	int						stdout_bak;
	enum e_parser_status	status;
	bool					fd_ok;
	bool					status_ok;

	printf("\n%sTesting heredoc processing%s\n", YELLOW, RESET);
	dev_null = open("/dev/null", O_WRONLY);
	stdout_bak = dup(STDOUT_FILENO);
	dup2(dev_null, STDOUT_FILENO);
	status = read_heredoc(&g_test_heredoc);
	dup2(stdout_bak, STDOUT_FILENO);
	close(dev_null);
	close(stdout_bak);
	fd_ok = (g_test_heredoc.fd >= 0);
	status_ok = (status == PARSER_SUCCESS);
	print_test_result("return_status", status_ok);
	print_test_result("fd_valid", fd_ok);
	if (fd_ok)
	{
		verify_heredoc_content(g_test_heredoc.fd);
		close(g_test_heredoc.fd);
	}
}

int	main(void)
{
	printf("%s\n=== HEREDOC TEST SUITE ===%s\n\n", YELLOW, RESET);
	test_temp_filename();
	test_heredoc_processing();
	printf("\n%sTests completed%s\n", GREEN, RESET);
	return (0);
}
