# Minishell word expansions

## Supported expansions

### Parameter expansion

Recognize `$` followed by a valid parameter name, either unquoted or in double quotes.
A valid parameter name is either a supported special parameter, or a valid name.
Remove the `$` and the parameter name, and substitute them with the parameter value.
Minishell only supports the `?` special parameter, and environment variables.

### Field splitting

Consider characters listed in the IFS variable as delimiters.
Recognize delimiter characters that resulted from unquoted parameter expansion. Characters present in the original word are not delimiters.
Recognized delimiters are removed, and split the word into separate fields.
Delimiters that are not whitespace always terminate a field, even if the resulting field is empty.
Delimiters that are whitespace terminate a field only if followed by a non-delimiter, and if the field is not empty.
This may result in no fields at all if the word consists entirely of whitespace delimiters.

### Pathname expansion

Minishell supports one special pattern character: `*` is a wildcard that matches zero or more characters.
Any other characters and any quoted characters are considered ordinary characters which only match themselves.
If a word (or field) contains at least one special pattern character, it is considered a pattern for pathname expansion.
Test if the pattern matches a path to an existing file or directory.
If any matches are found, replace the pattern field with a separate field for each match.
If no matches are found, or if the field was not a pattern, it remains unchanged.
Minishell only tests against directory entries of the current working directory.

### Quote removal

Any quotes that were present in the original word and were not quoted themselves are removed.

## Modes

A number of different word expansion modes are required depending on the grammatical context.

* Command word: parameter expansion, field splitting, pathname expansion, quote removal
* Redirection: parameter expansion, quote removal
	- POSIX does not mention field splitting, which implies it shall not be done.
	- POSIX also states that pathname expansion may be done by interactive shells only.
		If it produces multiple results, the shell may either report an error or use the word without pathname expansion.
	- Bash without POSIX mode always does both field splitting and pathname expansion, but reports an error unless it yields exactly one field.
* Heredoc delimiter: quote removal
	- POSIX leaves it unspecified whether any other expansions are done, and Bash doesn't seem to do any.
	- Heredoc line processing needs to know whether any quotes were present.
* Heredoc lines: parameter expansion
	- If delimiter had any quotes, no expansion is done on the lines.
	- Expansion should be performed when and if the heredoc redirection is actually evaluated.
	- Expansion should be performed after the delimiter has been found.

We choose to follow Bash's example for redirection expansion. This makes it identical to normal command word expansion.

The timing requirements of heredoc line expansion exist because some expansions in a POSIX shell can have visible side effects that modify the shell environment or consume input from stdin.
We don't support any such expansions, so we can safely perform the supported extensions as soon as each line is read and determined to not match the delimiter.

## Implementation

Heredoc delimiter and heredoc line expansion modes are implemented separately.
This explanation covers command word and redirection expansion modes.

Our implementation deviates from the processing order described in POSIX and in Bash manual, but should achieve correct results.

We implement parameter expansion, field splitting, and quote removal in a single combined stage.
We scan through the word, recognizing parameter expansions and quotes and pass other characters to output.
Quotes that are quoted themselves are also passed to output.
Presence of quotes is noted to prevent field splitting from ignoring an otherwise empty field.

When a parameter expansion is recognized, we immediately scan the substitution value.
If the parameter expansion happens outside quotes, we recognize splitting characters and perform field splitting. Other characters are passed to output.

If special pattern characters are passed to output while within quotes, they are escaped by prepending an escape character.
Pathname expansion stage considers any escaped characters ordinary characters.
Any instances of the escape character itself in the input word are also escaped to prevent misinterpreting them as escaping the following character.

The scan-and-output described above is performed twice.
First pass counts the number of characters in each output field, allocates memory for them, and stores them in a preliminary list.
Second pass stores the characters in the allocated fields.

When a field is completed during the second pass, if it is a pattern it is passed to pathname expansion.
If pathname expansion generates any matches, those are stored in the final list and the pattern is discarded.
If the field is not a pattern or generates no matches, escaping is undone and the field is stored in the final list.

This approach has the following advantages over separate stages:
* Quote recognition is done once, instead of each stage performing its own quote recognition to determine which characters are quoted.
* Parameter expansion can output any quote characters in the substitution value directly instead of needing to quote or escape them to avoid interfering with later quote recognition and removal.
* Parameter expansion doesn't need to store information about which characters are or are not eligible for field splitting.
* While pattern matching in pathname expansion still needs to recognize the escape character, it is much simpler than quote recognition.


## Open questions

TODO: determine answers and remove this section

### Pathname expansion features

* Should hidden directory entries (names beginning with `.`) be handled specially?
If yes, prevent wildcard from matching `.` in the first position only.

* Should multiple `*` wildcards be supported?
If yes, backtracking is required, e.g. recursive call after each attempted `*`.
