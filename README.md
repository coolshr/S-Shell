# Assignment 2 Shell
### To run: 
```
$ make
$./a.out
```
- promptandinput() function prompts the username, hostname and takes the input.
- parse parses the input and calls the appropriate function
- RunBg runs the background process
- handle_sigquit is called when the child exits of a background process
- ex() is used to exectue shell commands using execvp()
- enableRawMode() and disableRawMode() are used to enable and disable the raw mode so that arrow key function can be used, but it is not implemented.
- 