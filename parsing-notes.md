
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
AND
OR
GROUP_START
GROUP_END
END
```

## Grammar

```
command = list END
list = list_entry list_cont*
list_entry = group | pipeline
group = GROUP_START list GROUP_END
list_cont = list_op list_entry
list_op = AND | OR
pipeline = simple_command pipeline_cont*
pipeline_cont = PIPE simple_command
simple_command = command_word+
command_word = redirect | command_arg
redirect = redirect_op WORD
redirect_op = REDIR_IN | REDIR_OUT | REDIR_APP | HEREDOC
command_arg = WORD
```

This grammar describes all valid minishell command lines, and can be parsed with a recursive descent parser.
Left-side of each `=` names a "non-terminal symbol" and right side describes what other symbols that symbol can be composed of.
`|` means alternative options, `*` means zero or more repetitions, `+` means one or more repetitions, and capitalized words are "terminal symbols" which are not composed of anything else and correspond to tokens produced by the tokenizer.
The variable repetitions can be considered special alternatives - the repetition either continues or terminates.

### Top-down parsing

A top-down parser starts with just the initial symbol (here `command`) and replaces it with the symbols it must be composed of according to the grammar.
Any terminal symbols produced must match the input tokens, while any non-terminal symbols produced are processed recursively.
If terminal symbols don't match the input or there is more input remaining, the input is rejected.
Any alternatives in the grammar produce ambiguity.
The parser must choose one of the alternatives to proceed, but rejecting one alternative doesn't necessarily mean the input is incorrect.
The parser must instead backtrack and try other alternatives.

Backtracking requires storing information about previous choices made so that other alternatives can be attempted.
For complex input, the parser can do a lot of unnecessary work exploring an alternative that will be ultimately rejected.
However this particular grammar allows unambiguosly determining the correct alternative by looking at just the next token:
* For `list`, `list_cont` begins with `list_op` where only `AND` and `OR` are valid. Otherwise repetition ends.
* For `list_entry`, `GROUP_START` selects `group`, otherwise assume `pipeline`.
  * Further, `pipeline` starts with a `simple_command`, which must eventually start with either a redirection operator or `WORD`. Any other token could trigger an error here.
* For `pipeline`, `pipeline_cont` alway begins with `PIPE`, otherwiser repetition ends.
* For `simple_command`, `command_word` must be either `redirect` which begins with one of the redirection operators, or `param` which only accepts `WORD`.

Therefore there is no need to do actual backtracking - if there is no choice for the current position or the token doesn't match any of the alternatives, the parser can reject the input and report an error immediately.

## Abstract syntax tree

The parser must build an abstract syntax tree, which stores all the necessary information for execution.
Not all of the grammar symbols need to produce distinct tree nodes if they contain no meaningful information or their information can be stored otherwise, e.g. in the parent node.
Variable repetitions can be stored as linked lists. We can avoid allocating separate `t_list` nodes by including a `next` pointer in the repeatable nodes.

```c
struct  s_ast_command_arg
{
    char                        *word;
    struct s_ast_command_arg    *next;
}

// no node for redirect_op, store the variant in redirect node
enum    e_ast_redirect_op
{
    AST_REDIR_IN,
    AST_REDIR_OUT,
    AST_REDIR_APP,
    AST_HEREDOC,
}

struct  s_ast_redirect
{
    enum e_ast_redirect_op  op;
    char                    *word;
    struct s_ast_redirect   *next;
}

// no node for command_word, simple_command separates it into arg and redir
//   which each link to next of the type

struct  s_ast_simple_command
{
    struct s_ast_command_arg    *args;
    struct s_ast_redirect       *redirs;
    struct s_ast_simple_command *next;
}

// no node for pipeline_cont, simple_command links to next
// no node for pipeline, link directly to simple_commmand

// no node for list_op, store the variant in preceding list_entry node
enum    e_ast_list_op
{
    AST_LIST_AND,
    AST_LIST_OR,
}

// no node for list_cont, list_entry links to next and stores the op
// no node for group, link directly to inner list

// list_entry must distinguish inner variant
enum    e_ast_list_entry_type
{
    AST_LIST_GROUP,
    AST_LIST_PIPELINE,
}

struct  s_ast_list_entry
{
    enum e_ast_list_entry_type      type;
    union
    {
        struct s_ast_list_entry     *group;
        struct s_ast_simple_command *pipeline;
    };
    enum e_ast_list_op              next_op;
    struct s_ast_list_entry         *next;
}

// no node for command, link directly to top-level list
```

### Recursive descent parser

Top-down parsing can be implemented as a recursive descent parser: a function is created for each non-terminal symbol.
The function adds its symbol to the tree, peeks at the next token if it needs to determine correct alternative, consumes tokens for any terminal children, and calls the corresponding function for any non-terminal children.
