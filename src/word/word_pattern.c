/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_pattern.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:26:57 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/19 19:37:25 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

#include <stdbool.h>
#include <stddef.h>

#include "libft.h"

/*
	The inner matching functions don't use the pattern struct because character
	matching advances position in pattern and candidate, while wildcard matching
	needs to be able backtrack and retry from a previous position. Using
	recursion and keeping the pointers in parameters is an easy way to do that.

	TODO: Limit recursion to avoid stack overflow, or switch to explicit stack.
*/

static bool	word_pattern_match_characters(char *pattern, char *candidate,
				size_t max_wildcard_len);
static bool	word_pattern_match_wildcards(char *pattern, char *candidate,
				size_t max_wildcard_len);

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
*/
bool	word_pattern_init_filename(struct s_word_pattern *pattern, char *str)
{
	size_t	min_len;

	pattern->prefix = str;
	while (str[0] == '.' && str[1] == '/')
	{
		str += 2;
		while (str[0] == '/')
			str++;
	}
	pattern->prefix_len = str - pattern->prefix;
	pattern->pattern = str;
	min_len = 0;
	while (str[min_len])
	{
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

/*
	Test if the candidate `str` matches the pattern.

	We apply a special rule for pathname expansion:
	- Filenames beginning with a period `.` are considered hidden. A wildcard at
	the start of a pattern will not match the initial period.
*/
bool	word_pattern_test_filename(struct s_word_pattern *pattern, char *str)
{
	size_t	candidate_len;
	size_t	excess_len;

	if (str[0] == '.' && pattern->pattern[0] == '*')
		return (false);
	candidate_len = ft_strlen(str);
	if (candidate_len < pattern->min_len)
		return (false);
	excess_len = candidate_len - pattern->min_len;
	return (word_pattern_match_characters(pattern->pattern, str, excess_len));
}

/*
	Attempt to match literal characters in candidate to the pattern. Wildcard
	characters in pattern are skipped and a helper function tries different
	wildcard lengths. If literal characters don't match, reject the candidate.
	If end of the candidate and the pattern is reached, accept the candidate.
*/
static bool	word_pattern_match_characters(char *pattern, char *candidate,
	size_t max_wildcard_len)
{
	while (true)
	{
		if (*pattern == '*')
			return (word_pattern_match_wildcards(pattern + 1, candidate,
					max_wildcard_len));
		if (*pattern == INTERNAL_ESCAPE)
			pattern++;
		if (*pattern != *candidate)
			return (false);
		if (!*pattern)
			return (true);
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
static bool	word_pattern_match_wildcards(char *pattern, char *candidate,
	size_t max_wildcard_len)
{
	size_t	wildcard_len;

	while (*pattern == '*')
		pattern++;
	wildcard_len = max_wildcard_len + 1;
	while (wildcard_len--)
		if (word_pattern_match_characters(pattern, candidate + wildcard_len,
				max_wildcard_len - wildcard_len))
			return (true);
	return (false);
}
