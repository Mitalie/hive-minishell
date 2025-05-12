/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_execve.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:30:33 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/12 19:15:57 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include "env.h"

void	handle_path_search(char **argv, t_env *env, int *exit_code);

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

static void	print_result(const char *name, bool passed)
{
	if (passed)
		printf("[%sPASS%s] %s%s%s\n", GREEN, RESET, CYAN, name, RESET);
	else
		printf("[%sFAIL%s] %s%s%s\n", RED, RESET, CYAN, name, RESET);
}

static int	test_in_child(char **argv, t_env *env)
{
	pid_t	child_pid;
	int		status;
	int		exit_code;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (child_pid == 0)
	{
		handle_path_search(argv, env, &exit_code);
		exit(exit_code);
	}
	waitpid(child_pid, &status, 0);
	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		return (exit_code);
	}
	return (-1);
}

static void	test_not_found(void)
{
	t_env	env;
	char	*path_list;
	char	*argv[2];
	int		exit_code;

	printf("\n%sTesting command not found%s\n", YELLOW, RESET);
	path_list = strdup("/bin:/usr/bin:/usr/local/bin");
	env_init(&env);
	env_set(&env, "PATH", path_list);
	argv[0] = "nonexistentcommand123456789";
	argv[1] = NULL;
	printf("  Command: %s\n", argv[0]);
	printf("  PATH: %s\n", path_list);
	exit_code = test_in_child(argv, &env);
	printf("  Exit code: %d\n", exit_code);
	print_result("command_not_found_returns_127", exit_code == 127);
	env_free(&env);
	free(path_list);
}

static void	test_found(void)
{
	t_env	env;
	char	*path_list;
	char	*argv[3];
	int		exit_code;

	printf("\n%sTesting command found%s\n", YELLOW, RESET);
	path_list = strdup("/bin:/usr/bin:/usr/local/bin");
	env_init(&env);
	env_set(&env, "PATH", path_list);
	argv[0] = "echo";
	argv[1] = "test";
	argv[2] = NULL;
	printf("  Command: %s %s\n", argv[0], argv[1]);
	printf("  PATH: %s\n", path_list);
	exit_code = test_in_child(argv, &env);
	printf("  Exit code: %d\n", exit_code);
	print_result("command_found_executes", exit_code == 0);
	env_free(&env);
	free(path_list);
}

static void	test_absolute(void)
{
	t_env	env;
	char	*path_list;
	char	*argv[3];
	int		exit_code;

	printf("\n%sTesting absolute path%s\n", YELLOW, RESET);
	path_list = strdup("/bin:/usr/bin:/usr/local/bin");
	env_init(&env);
	env_set(&env, "PATH", path_list);
	argv[0] = "/bin/echo";
	argv[1] = "test";
	argv[2] = NULL;
	printf("  Command: %s %s\n", argv[0], argv[1]);
	printf("  PATH: %s\n", path_list);
	exit_code = test_in_child(argv, &env);
	printf("  Exit code: %d\n", exit_code);
	print_result("absolute_path_executes", exit_code == 0);
	env_free(&env);
	free(path_list);
}

int	main(void)
{
	printf("%s\n=== PATH EXECVE TEST SUITE ===%s\n\n", YELLOW, RESET);
	test_not_found();
	test_found();
	test_absolute();
	printf("\n%sTests completed%s\n", GREEN, RESET);
	return (0);
}
