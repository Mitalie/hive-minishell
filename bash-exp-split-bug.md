While designing our word expansion and splitting, we ran into a bug in Bash.
Consider the following example:
```sh
var="a   b"
echo $var
echo $var-
echo $var$
```

Shell should expand and split `$var` to two fields `a` and `b` which `echo` then prints separated with a single space.
Since the variable value doesn't end with a splitting character, additional characters after the variable name (or their expansions) should be appended to last split field.
A dollar sign at the end of the word should be treated as a literal character and not an expansion.
Therefore `$var-` should (and does) produce `a` and `b-`, and `$var$` should produce `a` and `b$`.

In `dash`, in `busybox sh` and in `zsh -o sh_word_split`, we get separate `a` and `b$` as expected.
In `bash`, we get `a   b$` as a single field instead, with three consecutive spaces between the visible characters.
In `bash`, `$var$` does not undergo word splitting, because the code that identifies a non-expansion dollar sign [unsets the expanded_something flag](https://github.com/bminor/bash/blob/6794b5478f660256a1023712b5fc169196ed0a22/subst.c#L10676-10686), and caller further up the stack [assumes there is nothing to split](https://github.com/bminor/bash/blob/6794b5478f660256a1023712b5fc169196ed0a22/subst.c#L12842-L12855).

This also affects other forms of expansion that should be split into fields, e.g. command substitution:
```sh
echo $(echo a; echo; echo b)
echo $(echo a; echo; echo b)$
```

This also happens if the dollar sign is followed by a character that doesn't form a valid expansion, e.g. `+` or `/`.
POSIX leaves the result unspecified in this case, but Bash interprets the dollar sign as a literal character.
There is no reason to expect the splitting of the preceding expansion to change.

The bug is circumvented if there is another expansion afterwards within the same word that sets expanded_something flag again.
