
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
* (bonus) Simple filename wildcard (`*`)

Special characters generally lose their meaning within quotes.
However variable expansion still happens within double quotes, and expansion could produce one or multiple fields depending on whether it happens in quotes or not.
Shell Command Language described in POSIX specifies that quotes are preserved within words until syntax analysis, expansions, and field splitting have been performed.

> ~~Instead of preserving quotes, it might be simpler to recognize and remove all special symbols in the tokenizer.
> The tokenizer would to output word fragment and expansion tokens which can be merged after performing expansion.
> Expansion tokens would have a quoted and unquoted variant to control field splitting.
> Separator token or flag would be required to control whether word fragment and expanded fragment or two expanded fragments next to each other should be merged.~~
>
> POSIX shell (and Bash) process filename wildcards resulting from expansion, so we'd still need to perform at least some special character processing after initial tokenization.

## Processing steps

1. Tokenizer: break the input into words and operators.
2. Syntax analyzer: recognize logical meaning of tokens and arrange them into a tree
3. Variable expansion
4. Word splitting
5. Quote removal
6. (bonus) Filename generation
7. Execution according to syntax tree (including redirections)
