/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:50:28 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/03 14:57:24 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUS_H
# define STATUS_H

/*
	Functions that might fail should return one of these status codes. Errors
	should also be reported to user using `status_err` or `status_warn` helpers.
	Functions that receive codes other than `S_OK` should clean up any resources
	its callers can't access and then return the code further, unless it instead
	handles a particular class of errors and continues.

	- `S_OK` indicates normal operation of the shell shall continue.
	- `S_EXIT_OK` indicates a clean exit, such as exit command, end-of-input, or
	a forked child process having completed its task. The shell shall exit
	without changing the current exit status.
	- `S_EXIT_ERR` indicates a fatal error. The shell shall set an appropriate
	exit status and then exit.
	- `S_RESET_ERR`, `S_RESET_SYNTAX`, and `S_RESET_SIGINT` indicate the current
	input shall be aborted due to an internal error, invalid input, or interrupt
	signal, respectively. The shell shall set an appropriate exit status, and
	then request more input if interactive and exit if non-interactive.
	- `S_COMM_ERR` indicates the current command can not be executed due to an
	error, but processing of the current input shall continue.
	- `S_BUILTIN_ERR` and `S_BUILTIN_ARG` are used to abort the execution of a
	built-in command without considering it an error of the shell. The shell
	shall set exit status of 1 for errors and 2 for invalid arguments.
*/
typedef enum e_status
{
	S_OK,
	S_EXIT_OK,
	S_EXIT_ERR,
	S_RESET_ERR,
	S_RESET_SYNTAX,
	S_RESET_SIGINT,
	S_COMM_ERR,
	S_BUILTIN_ERR,
	S_BUILTIN_ARG,
}	t_status;

/*
	Predefined error messages that are used in many places in the codebase.
*/

# define ERRMSG_MALLOC "memory allocation failed"

/*
	Print an error message to stderr and return a status code. Useful for
	printing a message for the user and assigning or returning a status code in
	a single statement.
	- `msg` is required and always printed
	- If `extra` is not NULL, `": "` and `extra` are printed
	- If `errnum` is not 0, `": "` and `strerror(errnum)` are printed
*/
t_status	status_err(t_status status,
				const char *msg, const char *extra, int errnum);

/*
	Print a warning message to stderr.
	- `msg` is required and always printed
	- If `extra` is not NULL, `": "` and `extra` are printed
	- If `errnum` is not 0, `": "` and `strerror(errnum)` are printed
*/
void		status_warn(const char *msg, const char *extra, int errnum);

/*
	Set the appropriate exit code if the status implies one.
*/
void		status_set_exit_code(t_status status, int *exit_code);

/*
	Set the appropriate exit code if the status implies one, and replace the
	status with S_EXIT_OK.
*/
t_status	status_force_exit(t_status status, int *exit_code);

#endif
