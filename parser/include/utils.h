/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:24:15 by josmanov          #+#    #+#             */
/*   Updated: 2025/02/27 19:19:53 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"

// Error Handling
void print_syntax_error(const char *message);

// String Utilities
char *trim_quotes(char *str);
char *join_strings(const char *s1, const char *s2);

// Memory Utilities
void *safe_malloc(size_t size);
void free_array(char **array);

// Debugging Utilities
void print_tokens(struct s_token *tokens);

#endif
