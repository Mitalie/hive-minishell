# fixes

* recursion limit for wildcard
* recursion limit for list/group parsing

# checks

* syscall errors
* malloc errors

# important cleanups

* delete tests
* delete design notes?
* remove todo comments

# prio cleanups and improvements

* use t_status error handling in execute/path

# optional cleanups and improvements

* make sure all func names and global names have module prefix
* split status_err with different params
* wrap malloc and other allocating functions (ft_strdup) with t_status funcs
* move tokenizer into parser? separation of concerns vs could share state struct
* include-what-you-use (comment for readline/stdio? or rely on debian/ubuntu and don't include it?)
* pass flags to libft build properly
* cflow diagram in Makefile

# nice-to-haves, probably not doing

* informative syntax errors
* check for caught SIGINT in child, go to clean exit and then raise the die with the signal
* don't abort exec if child caught SIGINT
* report all deaths to signals (implement strsignal with hardcoded table)
