/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_search.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:38:33 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/25 14:35:27 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "path.h"
#include "env.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

static void	print_test_result(const char *name, bool passed)
{
	if (passed)
	{
		printf("[%sPASS%s] %s%s%s\n", GREEN, RESET, CYAN, name, RESET);
	}
	else
	{
		printf("[%sFAIL%s] %s%s%s\n", RED, RESET, CYAN, name, RESET);
	}
}

static bool	check_result(char *result, const char *expected)
{
	if (result != NULL && strcmp(result, expected) == 0)
	{
		return (true);
	}
	return (false);
}

static void	test_absolute_path(t_env *env)
{
	char	*result;
	bool	success;

	printf("\n%sTesting absolute path handling%s\n", YELLOW, RESET);
	result = path_search("/bin/ls", env);
	success = check_result(result, "/bin/ls");
	printf("  Input: /bin/ls\n");
	if (result != NULL)
	{
		printf("  Result: %s\n", result);
	}
	else
	{
		printf("  Result: NULL\n");
	}
	print_test_result("absolute_path_preserved", success);
	free(result);
}

static void	test_relative_path(t_env *env)
{
	char	*result;
	bool	success;

	printf("\n%sTesting relative path handling%s\n", YELLOW, RESET);
	result = path_search("./test", env);
	success = check_result(result, "./test");
	printf("  Input: ./test\n");
	if (result != NULL)
	{
		printf("  Result: %s\n", result);
	}
	else
	{
		printf("  Result: NULL\n");
	}
	print_test_result("relative_path_preserved", success);
	free(result);
}

static void	display_search_result(char *input, char *result)
{
	printf("  Input: %s\n", input);
	if (result != NULL)
	{
		printf("  Result: %s\n", result);
	}
	else
	{
		printf("  Result: NULL\n");
	}
}

static void	test_path_search(t_env *env)
{
	char	*result;

	printf("\n%sTesting PATH search%s\n", YELLOW, RESET);
	result = path_search("ls", env);
	display_search_result("ls", result);
	print_test_result("common_utility_found", result != NULL);
	free(result);
	result = path_search("nonexistentcommand123456789", env);
	display_search_result("nonexistentcommand123456789", result);
	print_test_result("nonexistent_command_returns_null", result == NULL);
	free(result);
}

static void	test_empty_path(t_env *env)
{
	char	*result;
	char	*old_path;

	printf("\n%sTesting empty PATH variable%s\n", YELLOW, RESET);
	old_path = env_get(env, "PATH");
	if (old_path != NULL)
	{
		old_path = strdup(old_path);
	}
	env_set(env, "PATH", "");
	result = path_search("ls", env);
	display_search_result("ls with empty PATH", result);
	print_test_result("empty_path_returns_null", result == NULL);
	free(result);
	if (old_path != NULL)
	{
		env_set(env, "PATH", old_path);
		free(old_path);
	}
}

int	main(void)
{
	t_env	*env;

	printf("%s\n=== PATH SEARCH TEST SUITE ===%s\n\n", YELLOW, RESET);
	env = env_init();
	if (env == NULL)
	{
		printf("Failed to initialize environment\n");
		return (1);
	}
	test_absolute_path(env);
	test_relative_path(env);
	test_path_search(env);
	test_empty_path(env);
	env_free(env);
	printf("\n%sTests completed%s\n", GREEN, RESET);
	return (0);
}
