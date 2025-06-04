/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:20:42 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/04 20:14:39 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "shenv.h"
#include "status.h"

static void	print_env_variable(t_shenv *env, const char *var_name)
{
	char	*value;

	value = shenv_var_get(env, var_name);
	if (value)
		printf("%s=%s\n", var_name, value);
	else
		printf("%s=(null)\n", var_name);
}

static void	test_env_get(t_shenv *env)
{
	printf("\n1. Testing shenv_var_get for HOME:\n");
	print_env_variable(env, "HOME");
}

static void	test_env_set_new(t_shenv *env)
{
	t_status	result;

	printf("\n2. Testing shenv_var_set for new variable TEST_VAR:\n");
	result = shenv_var_set(env, "TEST_VAR", "Hello World");
	if (result == S_OK)
		printf("Successfully set TEST_VAR\n");
	else
		printf("Failed to set TEST_VAR\n");
	print_env_variable(env, "TEST_VAR");
}

static void	test_env_set_existing(t_shenv *env)
{
	t_status	result;

	printf("\n3. Testing shenv_var_set to modify existing variable HOME:\n");
	printf("Before: ");
	print_env_variable(env, "HOME");
	result = shenv_var_set(env, "HOME", "/temp/home");
	if (result == S_OK)
		printf("Successfully modified HOME\n");
	else
		printf("Failed to modify HOME\n");
	printf("After: ");
	print_env_variable(env, "HOME");
}

static void	test_env_unset(t_shenv *env)
{
	t_status	result;

	printf("\n4. Testing shenv_var_unset for TEST_VAR:\n");
	printf("Before: ");
	print_env_variable(env, "TEST_VAR");
	result = shenv_var_unset(env, "TEST_VAR");
	if (result == S_OK)
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

static void	test_env_array(t_shenv *env)
{
	char	**var_array;

	printf("\n5. Testing env array (showing first 10 items):\n");
	var_array = env->var_array;
	print_array_items(var_array, 10);
	printf("... (and more)\n");
}

int	main(void)
{
	t_shenv		env;
	t_status	status;

	printf("--- Testing Environment Functions ---\n\n");
	printf("Initializing environment...\n");
	status = shenv_init(&env);
	if (status != S_OK)
	{
		printf("Failed to initialize environment\n");
		return (1);
	}
	test_env_get(&env);
	test_env_set_new(&env);
	test_env_set_existing(&env);
	test_env_unset(&env);
	test_env_array(&env);
	printf("\nCleaning up environment...\n");
	shenv_free(&env);
	printf("Test completed.\n");
	return (0);
}
