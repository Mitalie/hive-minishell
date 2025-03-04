# Minishell word expansions

## Modes

A number of different word expansion modes are required depending on the grammatical context.

* command_word: variable expansion, field splitting, filename generation, quote removal
* redirect_word: variable expansion, quote removal
	- POSIX does not mention field splitting, which implies it shall not be done.
	It also states that filename generation shall not be done if the shell is noninteractive, and filename generation may be done if the shell is interactive, but only if it yields exactly one field.
	- Bash always does field splitting and filename generation, but gives error unless it yields exactly one field.
	This makes redirect_word expansion identical to command_word expansion, but requires checking the field count in redirect code.
* heredoc_word: quote removal
	- POSIX leaves it unspecified whether any other expansions are done, and Bash doesn't seem to do any. The shell needs to know whether any quotes were present, though.

## Challenges or questions

### Handling of quotes in expanded values

Variables may contain quotes in them, and those quotes must be neutralized so that they don't prevent field splitting and filename generation, and don't get removed by quote removal.
Wrap them in pair of opposite quotes? Support backslash or another escape char for this? Pair the string with flags for each character? Split the string into flagged fragments?

### Filename generation features

Should hidden directory entries (names beginning with `.`) be handled specially?
If yes, prevent wildcard from matching `.` in the first position only.

Should multiple `*` wildcards be supported?
If yes, backtracking is required, e.g. recursive call after each attempted `*`.

## Details for separate steps

### Variable expansion

Scan over the string and count its length, recognizing single quotes `'` and expansion characters `$`.
On single quote, scan until next single quote without recognizing expansion characters.
On expansion character, read variable name without counting it, then count the corresponding value instead.
Minishell doesn't support shell parameters, only environment variables and last exit status `$?` are supported.
POSIX leaves the result unspecified if a `$` doesn't introduce a valid expansion; we mimic Bash and output the `$` character.
When the end of the word is reached, allocate memory for the expanded word.
Repeat the scan from beginning, but this time copying characters instead of counting them.

### Field splitting

Scan over the string, counting length and recognizing single quotes `'`, double quotes `"` and blanks.
On quote, scan until next matching quote without recognizing blanks.
On blank, allocate memory for the split field.
Copy characters from starting position to current position into the split field.
Skip the blank(s) and resume scanning with current position as new starting position.
If entire string is scanned without finding blanks, yield the original string.
Otherwise free the original string and yield the created fields.

### Filename generation

If input contains no wildcards, yield the input directly after quote removal.
The project assignment only requires current directory, so no need to split the pattern on `/`.
Read directory entries with `opendir`/`readdir`/`closedir`.
Compare each directory entry with the pattern and yield any matches as separate fields, without quote removal.
If no matches are found, yield the input directly after quote removal.
If matches are found, free the input string.

### Quote removal

Simple pass over string with read and write pointers.
Quote characters are ignored without incrementing write pointer.
After a quote character, other quote characters are not recognized until the matching end of quotation is found.
Other characters are moved towards the start of the string, overwriting any recognized quote characters.
New terminator must also be written.


## Integrated variable expansion and splitting

Integrating the two stages avoids the double scanning and allocation done by separate stages, but is slightly more complex.
The same two-pass approach as with separate stages is be used, but there are more states and more special character rules.
Memory for field is allocated during the first pass whenever a split is detected, in addition to end of the word.

We also recognize wildcard characters during the scan to see whether filename generation needs to be attempted for each field.


Scan modes and special character rules:
* Normal
	* `'`: Switch to `Single quoted`
	* `"`: Switch to `Double quoted`
	* `$`: Skip character, read and skip variable name, scan variable value with `Replacement`
	* `*`: Enable filename generation
* Single quoted
	* `'`: Return to `Normal`
* Double quoted
	* `"`: Return to `Normal`
	* `$`: Skip character, read and skip variable name, scan variable value with `Quoted replacement`
* Replacement
	* `*`: Enable filename generation
	* blank: End current field and move onto next, skip until next non-blank
* Quoted replacement
	* (no special characters recognized)

