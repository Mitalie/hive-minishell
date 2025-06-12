/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_execve.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:30:33 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/12 23:38:48 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shenv.h"
#include "status.h"

t_status	execute_external_command(char **argv, t_shenv *env);

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

static void	test_in_child(char **argv, t_shenv *env)
{
	pid_t	child_pid;
	int		status;

	env->exit_code = -1;
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		return ;
	}
	if (child_pid == 0)
	{
		status = execute_external_command(argv, env);
		status_set_exit_code(status, env);
		exit(env->exit_code);
	}
	waitpid(child_pid, &status, 0);
	if (WIFEXITED(status))
		env->exit_code = WEXITSTATUS(status);
}

static void	test_not_found(void)
{
	t_shenv	env;
	char	*path_list;
	char	*argv[2];

	printf("\n%sTesting command not found%s\n", YELLOW, RESET);
	path_list = strdup("/bin:/usr/bin:/usr/local/bin");
	shenv_init(&env);
	shenv_var_set(&env, "PATH", path_list);
	argv[0] = "nonexistentcommand123456789";
	argv[1] = NULL;
	printf("  Command: %s\n", argv[0]);
	printf("  PATH: %s\n", path_list);
	test_in_child(argv, &env);
	printf("  Exit code: %d\n", env.exit_code);
	print_result("command_not_found_returns_127", env.exit_code == 127);
	shenv_free(&env);
	free(path_list);
}

static void	test_found(void)
{
	t_shenv	env;
	char	*path_list;
	char	*argv[3];

	printf("\n%sTesting command found%s\n", YELLOW, RESET);
	path_list = strdup("/bin:/usr/bin:/usr/local/bin");
	shenv_init(&env);
	shenv_var_set(&env, "PATH", path_list);
	argv[0] = "echo";
	argv[1] = "test";
	argv[2] = NULL;
	printf("  Command: %s %s\n", argv[0], argv[1]);
	printf("  PATH: %s\n", path_list);
	test_in_child(argv, &env);
	printf("  Exit code: %d\n", env.exit_code);
	print_result("command_found_executes", env.exit_code == 0);
	shenv_free(&env);
	free(path_list);
}

static void	test_absolute(void)
{
	t_shenv	env;
	char	*path_list;
	char	*argv[3];

	printf("\n%sTesting absolute path%s\n", YELLOW, RESET);
	path_list = strdup("/bin:/usr/bin:/usr/local/bin");
	shenv_init(&env);
	shenv_var_set(&env, "PATH", path_list);
	argv[0] = "/bin/echo";
	argv[1] = "test";
	argv[2] = NULL;
	printf("  Command: %s %s\n", argv[0], argv[1]);
	printf("  PATH: %s\n", path_list);
	test_in_child(argv, &env);
	printf("  Exit code: %d\n", env.exit_code);
	print_result("absolute_path_executes", env.exit_code == 0);
	shenv_free(&env);
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
