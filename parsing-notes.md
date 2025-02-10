
# Minishell parsing

## Required syntax features / special characters:

* Single quoted strings (`'`)
* Double quoted strings (`"`)
  * Allowing variable expansion within
* Variable expansion (`$`)
  * Environment variables (`$name`)
  * Last exit status (`$?`)
* Basic redirections (`<` `>` `>>`)
* Heredoc (`<<`)
* Pipeline (`|`)
* (bonus) Command list operators (`&&` `||`)
* (bonus) Grouping operators (`(` `)`)
* (bonus) Simple filename wildcard (`*`)

Special characters generally lose their meaning within quotes.
However variable expansion still happens within double quotes, and expansion could produce one or multiple fields depending on whether it happens in quotes or not.
Shell Command Language described in POSIX specifies that quotes are preserved within words until syntax analysis, expansions, and field splitting have been performed.

Instead of preserving quotes, it might be simpler to recognize and remove all special symbols in the tokenizer.
The tokenizer would output plain fragment and expansion tokens which can be merged after performing expansion.
Expansion tokens would have a quoted and unquoted variant to control splitting.
Merge flag controls whether word fragment or expanded fragment should be merged into the preceding fragment or not.
Filename wildcards are also put in their own token type to trigger filename generation.

POSIX shell (and Bash) process filename wildcards resulting from expansion, so with this approach we'll need to perform a simplified tokenization on results of unquoted expansion.
This expansion can only produce plain fragments or filename wildcards - quotes or dollar signs inside variables don't trigger recursive processing.

## Processing steps

### POSIX shell

1. Tokenizer: break the input into words and operators.
2. Syntax analyzer: recognize logical meaning of tokens and build a syntax tree
3. Word processing: for each word, perform following steps (if applicable in that word's context)
    1. Variable expansion
    2. Field splitting
    3. (bonus) Filename generation
    4. Quote removal
4. Execution according to syntax tree (including redirections)

### Our alternative approach

1. Tokenizer: break the input into fragments and operators.
2. Syntax analyzer: recognize logical meaning of tokens and build a syntax tree
3. Fragment processing: for each fragment, perform one action depending on fragment type
    * Variable expansion (produces new plain or filename fragments)
    * Filename generation (produces new plain fragments)
    * Fragment merging
4. Execution according to syntax tree (including redirections)

### (temp) problems

* Heredoc delimiter is not expanded, but quotes are removed and the delimiter is flagged either quoted or unquoted.
  We could defend not having quoted mode and take the delimiter entirely unexpanded, but it still needs either parameterizing the HEREDOC token rather than emitting operator + word, or reconstructing the original input in case the delimiter got parsed into multiple tokens.
* Fields resulting from variable expansion can be merged into filename wildcards.
  Therefore we can't assume all filename tokens are fully formed from tokenizer.
  Filename fragments also need a merge flag to support `$var*`, and a merge-flagged variable token after a filename token needs to be expanded before filename generation to support `*$var`.
  Merged fragments might also include literal `*` that should NOT be interpreted as special characters for the wildcard pattern.
  * We could flag unfinished word fragments instead of appending fragments.
    Then we'd need to buffer text while merging as it could end up merging into a wildcard, but this might not be a big hurdle.
  * Filename generator needs to know each special character's quote status.
    Maybe the generator can take in a sequence of tokens instead of a pattern as string, and special characters would have individual tokens like operators instepipelinead of text tokens flagged for filename generation.

## Tokens

```
WORD        (content)
REDIR_IN
REDIR_OUT
REDIR_APP
HEREDOC
PIPE
LIST_AND
LIST_OR
GROUP_START
GROUP_END
```

## Grammar

```
list = pipeline | group | and | or
group = GROUP_START list GROUP_END
and = list LIST_AND list_entry
or = list LIST_OR list_entry
list_entry = pipeline | group
pipeline = command pipeline_tail*
pipeline_tail = PIPE command
simple_command = command_word+
simple_command_word = redirect | word
redirect = redirect_op word
```
