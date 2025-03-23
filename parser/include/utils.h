/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:24:15 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/23 14:45:27 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"

/* Error Handling */
void print_syntax_error(const char *message);

/* String Utilities */
char *trim_quotes(char *str);
char *join_string(const char *s1, const char *s2);

/* Memory Utilities */
void *safe_malloc(size_t size);
void free_array(char **array);

/* Debugging Utilities */
void print_tokens(struct s_token *tokens);
void print_ast(struct s_ast_list_entry *root, int indent_level);
void print_simple_command(struct s_ast_simple_command *cmd, int indent_level);
void free_tokens(struct s_token *tokens);

#endif /* UTILS_H */
