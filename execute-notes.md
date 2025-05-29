execute flow modes:
- empty cmd
	1. open redirs
	2. close redirs
- builtin
	1. open redirs
	2. exec builtin
	3. close redirs
- external, pipelined
	1. open redirs
	2. move redirs to stdin/stdout
	3. pathsearch-execve
- external, not pipelined
	1. fork
		1. open redirs
		2. move redirs to stdin/stdout
		3. pathsearch-execve
		4. update exit_code and force exit
	2. wait for child

execute flow overall:
1. expand args
2. if external and not already child, fork
3. if forked parent, wait for child and return
4. open redirs
5. if external, move redirs to stdin/stdout
6. if external, pathsearch-execve
7. if builtin, exec builtin
8. if builtin or empty, close redirs
9. if child, update exit_code and force exit
