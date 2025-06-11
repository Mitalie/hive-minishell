/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shutil.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 21:34:35 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/11 21:02:00 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shutil.h"

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "util.h"

#define SIGINT_MSG "\n"
#define SIGQUIT_MSG "Quit (core dumped)\n"

/*
	Process the child exit status reported by wait().
*/
void	shutil_process_wstatus(int wstatus, t_shenv *env)
{
	int	termsig;

	if (WIFEXITED(wstatus))
		env->exit_code = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
	{
		termsig = WTERMSIG(wstatus);
		env->exit_code = 128 + termsig;
		if (termsig == SIGINT)
			util_write_all(STDOUT_FILENO, SIGINT_MSG, sizeof(SIGINT_MSG) - 1);
		else if (termsig == SIGQUIT)
			util_write_all(STDOUT_FILENO, SIGQUIT_MSG, sizeof(SIGQUIT_MSG) - 1);
	}
}
