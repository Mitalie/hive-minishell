/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:20:42 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/13 14:49:38 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "env.h"

#include "shell.h"

t_shell	g_shell;

static void	print_env_variable(t_env *env, const char *var_name)
{
	char	*value;

	value = env_get(env, var_name);
	if (value)
		printf("%s=%s\n", var_name, value);
	else
		printf("%s=(null)\n", var_name);
}

static void	test_env_get(t_env *env)
{
	printf("\n1. Testing env_get for HOME:\n");
	print_env_variable(env, "HOME");
}

static void	test_env_set_new(t_env *env)
{
	int	result;

	printf("\n2. Testing env_set for new variable TEST_VAR:\n");
	result = env_set(env, "TEST_VAR", "Hello World");
	if (result == 0)
		printf("Successfully set TEST_VAR\n");
	else
		printf("Failed to set TEST_VAR\n");
	print_env_variable(env, "TEST_VAR");
}

static void	test_env_set_existing(t_env *env)
{
	int		result;

	printf("\n3. Testing env_set to modify existing variable HOME:\n");
	printf("Before: ");
	print_env_variable(env, "HOME");
	result = env_set(env, "HOME", "/temp/home");
	if (result == 0)
		printf("Successfully modified HOME\n");
	else
		printf("Failed to modify HOME\n");
	printf("After: ");
	print_env_variable(env, "HOME");
}

static void	test_env_unset(t_env *env)
{
	int	result;

	printf("\n4. Testing env_unset for TEST_VAR:\n");
	printf("Before: ");
	print_env_variable(env, "TEST_VAR");
	result = env_unset(env, "TEST_VAR");
	if (result == 0)
		printf("Successfully unset TEST_VAR\n");
	else
		printf("Failed to unset TEST_VAR\n");
	printf("After: ");
	print_env_variable(env, "TEST_VAR");
}

static void	print_array_items(char **array, int count)
{
	int	i;

	i = 0;
	while (i < count && array[i])
	{
		printf("%s\n", array[i]);
		i++;
	}
}

static void	test_env_array(t_env *env)
{
	char	**env_array;

	printf("\n5. Testing env_get_array (showing first 10 items):\n");
	env_array = env_get_array(env);
	print_array_items(env_array, 10);
	printf("... (and more)\n");
}

int	main(void)
{
	t_env	*env;

	printf("--- Testing Environment Functions ---\n\n");
	printf("Initializing environment...\n");
	env = env_init();
	if (!env)
	{
		printf("Failed to initialize environment\n");
		return (1);
	}
	test_env_get(env);
	test_env_set_new(env);
	test_env_set_existing(env);
	test_env_unset(env);
	test_env_array(env);
	printf("\nCleaning up environment...\n");
	env_free(env);
	printf("Test completed.\n");
	return (0);
}
