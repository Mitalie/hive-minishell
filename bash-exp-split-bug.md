While designing our word expansion and splitting, we ran into a bug in Bash.
Consider the following example:
```sh
var="a   b"
echo $var
echo $var-
echo $var$
```

Shell should expand and split `$var` to two fields `a` and `b` which `echo` then prints separated with a single space.
Additional characters that aren't part of the variable name should be appended to last split field,
so `$var-` should produce `a` and `b-` and similarly `$var$` should produce `a` and `b$`.

In `dash`, in `busybox sh` and in `zsh -o sh_word_split`, we get separate `a` and `b$` as expected.
In `bash`, we get `a   b$` as a single field instead, with three consecutive spaces between the visible characters.
In `bash`, `$var$` does not undergo word splitting, because the trailing dollar sign is interpreted as a zero-length variable name which [sets expanded_something to 0](https://github.com/bminor/bash/blob/6794b5478f660256a1023712b5fc169196ed0a22/subst.c#L10676-10686), and caller further up the stack [assumes there is nothing to split](https://github.com/bminor/bash/blob/6794b5478f660256a1023712b5fc169196ed0a22/subst.c#L12842-L12855).

This also affects other forms of expansion that should be split into fields, e.g. command substitution:
```sh
echo $(echo a; echo; echo b)
echo $(echo a; echo; echo b)$
```

This also happens if the dollar sign is followed by a character that can't be interpreted as part of a parameter name nor as special parameters, e.g. `+` or `/`.
The bug is circumvented if there is another expansion afterwards that sets expanded_something to a non-zero value.
