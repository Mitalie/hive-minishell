/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_pattern.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:26:57 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/12 16:45:44 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

#include <stdbool.h>
#include <stddef.h>

#include "libft.h"
#include "signals.h"
#include "status.h"

/*
	The inner matching functions don't use the pattern struct because character
	matching advances position in pattern and candidate, while wildcard matching
	needs to be able backtrack and retry from a previous position. Using
	recursion and keeping the pointers in parameters is an easy way to do that.
*/

static t_status	word_pattern_match_characters(char *pattern, char *candidate,
					size_t max_wildcard_len, bool *match);
static t_status	word_pattern_match_wildcards(char *pattern, char *candidate,
					size_t max_wildcard_len, bool *match);

/*
	Helper function for initialization, see below.
*/
static void		word_pattern_init_skip_prefix(struct s_word_pattern *pattern);

#define ERRMSG_NUMWILDCARD "warning: too many wildcards, not processing pattern"

/*
	Initialize a pattern for matching against the given pattern string `str`.

	The matching algorithm needs to know the minimum length of the pattern to
	avoid testing excessively long wildcard matches that wouldn't allow the
	rest of the pattern to match. Minimum length is the number of literal
	characters, with a zero length match for each wildcard.

	We also apply special rules for pathname expansion:
	- Minishell doesn't traverse directories for pathname expansion, so patterns
	with slashes will never match and can be rejected immediately.
	- If the pattern begins with dot-slash elements, possibly with repeated
	slashes, they refer to current directory and can be skipped to allow
	matching to happen normally. The prefix is prepended to matches afterwards.

	We also apply a limit on the number of wildcards to prevent stack overflow
	due to execessive recursion. While we could handle very large patterns, we
	put the limit at "only" 100 wildcards as the execution time for checking
	potential matches gets impractical anyway.
*/
bool	word_pattern_init_filename(struct s_word_pattern *pattern, char *str)
{
	size_t	min_len;
	size_t	max_wildcards;

	pattern->pattern = str;
	word_pattern_init_skip_prefix(pattern);
	min_len = 0;
	max_wildcards = 100;
	while (str[min_len])
	{
		if (str[min_len] == '*' && !max_wildcards--)
		{
			status_warn(ERRMSG_NUMWILDCARD, pattern->pattern, 0);
			return (false);
		}
		while (str[min_len] == '*')
			str++;
		if (str[min_len] == INTERNAL_ESCAPE)
			str++;
		if (str[min_len] == '/')
			return (false);
		if (str[min_len])
			min_len++;
	}
	pattern->min_len = min_len;
	return (true);
}

static void	word_pattern_init_skip_prefix(struct s_word_pattern *pattern)
{
	char	*str;

	str = pattern->pattern;
	pattern->prefix = str;
	while (str[0] == '.' && str[1] == '/')
	{
		str += 2;
		while (str[0] == '/')
			str++;
	}
	pattern->prefix_len = str - pattern->prefix;
	pattern->pattern = str;
}

/*
	Test if the candidate `str` matches the pattern.

	We apply a special rule for pathname expansion:
	- Filenames beginning with a period `.` are considered hidden. A wildcard at
	the start of a pattern will not match the initial period.
*/
t_status	word_pattern_test_filename(
	struct s_word_pattern *pattern, char *str, bool *match)
{
	size_t	candidate_len;
	size_t	excess_len;

	*match = false;
	if (str[0] == '.' && pattern->pattern[0] == '*')
		return (S_OK);
	candidate_len = ft_strlen(str);
	if (candidate_len < pattern->min_len)
		return (S_OK);
	excess_len = candidate_len - pattern->min_len;
	return (word_pattern_match_characters(
			pattern->pattern, str, excess_len, match));
}

#define ERRMSG_PMATCH_INT "interrupted pattern matching"

/*
	Attempt to match literal characters in candidate to the pattern. Wildcard
	characters in pattern are skipped and a helper function tries different
	wildcard lengths. If literal characters don't match, reject the candidate.
	If end of the candidate and the pattern is reached, accept the candidate.

	Check for SIGINT here as the matching can take a long time with patterns
	with many wildcards if they can't be immediately rejected.
*/
static t_status	word_pattern_match_characters(char *pattern, char *candidate,
	size_t max_wildcard_len, bool *match)
{
	if (signals_check_sigint(false))
		return (status_err(S_RESET_SIGINT, ERRMSG_PMATCH_INT, NULL, 0));
	while (true)
	{
		if (*pattern == '*')
			return (word_pattern_match_wildcards(pattern + 1, candidate,
					max_wildcard_len, match));
		if (*pattern == INTERNAL_ESCAPE)
			pattern++;
		if (*pattern != *candidate)
			return (S_OK);
		if (!*pattern)
		{
			*match = true;
			return (S_OK);
		}
		pattern++;
		candidate++;
	}
}

/*
	Attempt the longest possible match for a wildcard and check if the rest of
	the pattern matches. If it doesn't, repeat with a shorter matches for the
	wildcard down to a zero-length match. If even a zero-length match doesn't
	allow the rest of the pattern to match, reject the candidate.

	Consecutive wildcards are combined into this one as their individual lenghts
	don't affect the result and processing them individually would exponentially
	ncrease the number of attempts before rejection.
*/
static t_status	word_pattern_match_wildcards(char *pattern, char *candidate,
	size_t max_wildcard_len, bool *match)
{
	t_status	status;
	size_t		wildcard_len;

	while (*pattern == '*')
		pattern++;
	wildcard_len = max_wildcard_len + 1;
	while (wildcard_len--)
	{
		status = word_pattern_match_characters(pattern,
				candidate + wildcard_len,
				max_wildcard_len - wildcard_len,
				match);
		if (status != S_OK)
			return (status);
		if (*match)
			return (S_OK);
	}
	return (S_OK);
}
