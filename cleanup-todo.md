# nice-to-haves, probably not doing

* cflow diagram in Makefile
* split status_err with different params
* informative syntax errors
* check for caught SIGINT in child, go to clean exit and then raise the die with the signal
* don't abort exec if child caught SIGINT
* report all deaths to signals (implement strsignal with hardcoded table)
