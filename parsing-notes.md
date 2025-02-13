
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
Shell Command Language described in POSIX specifies that quotes and many special characters are preserved within words and processed later, after syntax analysis.

We thought it might be simpler to recognize all special characters in the tokenizer and produce corresponding tokens, instead of preserving them as characters within words until later steps.
It turns out interactions between various features, particularly expansion and filename wildcards, require additional complexity to handle with this approach and defeat the simplicity benefit, so we go with the approach described in POSIX.

<details>
<summary>Analysis of the alternative approach</summary>

Instead of preserving quotes, it might be simpler to recognize and remove all special symbols in the tokenizer.
The tokenizer would output plain fragment and expansion tokens which can be processed in a single pass.
Merge flag controls whether each fragment should be appended to the preceding field or start a new one.
Expansion tokens have a quoted and unquoted variant to disable or enable splitting into multiple plain fragments.
Filename wildcards are also put in their own token type to trigger filename generation.

Most special characters are not parsed in heredoc delimiter, but quotes are removed and the delimiter is flagged either quoted or unquoted.
Either the tokenizer needs to recognize the delimiter and output it as a parameter on the HEREDOC token, or we need to later reconstruct the original input (including special characters) from the various tokens that were produced.

POSIX shell (and Bash) process filename wildcards resulting from expansion, so with this approach we'll need to perform another simplified tokenization round on results of unquoted expansion.
This expansion can only produce plain fragments or filename wildcards - quotes or dollar signs inside variables don't trigger recursive processing.

Fields resulting from variable expansion can be merged into filename wildcards.
Therefore we can't assume all filename tokens are fully formed from tokenizer.
A later filename token can absorb preceding text tokens with inputs like `$var*`, and a filename token can't be processed right away if following tokens can merge with it, such as with `*$var`.

Merged fragments might also include literal `*` that should NOT be interpreted as special characters for the wildcard pattern.
Filename generator needs to know each special character's status as literal or wildcard.
Because we no longer have quotes at this stage, we need to keep active wildcard characters in their own tokens instead of merging with text, and pass token sequences to filename generator.
Without merging we still need to keep track where each pattern ends, and we'll have to scan ahead to see if there are wildcards or not, or pass every token through filename generator.

The resulting complexity from these caveats makes the original POSIX approach seem more attractive.
</details>

## Processing steps

1. Tokenizer: break the input into words and operators.
2. Syntax analyzer: recognize logical meaning of tokens and build a syntax tree
3. Word processing: for each word, perform following steps (if applicable in that word's context)
    1. Variable expansion
    2. Field splitting
    3. (bonus) Filename generation
    4. Quote removal
4. Execution according to syntax tree (including redirections)

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
