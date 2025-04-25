/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:40:48 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/25 18:57:00 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_INTERNAL_H
# define HEREDOC_INTERNAL_H

# include "parser.h"
# include "ast.h"

# define HEREDOC_TMP_PREFIX "/tmp/minishell_heredoc_"

enum e_parser_status	read_heredoc(struct s_ast_redirect *redirect);
char					*generate_temp_filename(void);
int						handle_heredoc_line(int fd, char *line,
							char *delimiter);
int						create_heredoc_file(char **tmp_filename);
int						reopen_heredoc_file(char *tmp_filename);

#endif
