# Ptrace experiments
This project contains some code snippets to test ptrace functionalities.
Feel free to use the code as you wish and to contribute to the project.
A list of available experiments is provided below as well as their purpose and how to run them.

### Disclaimer
Note that the code is not meant to be used in production and it is provided as is.
Also, the code is not optimised and it is not meant to be used as a reference for best practices.


### Requirements
- Linux-based OS with ptrace support
- C compiler
- make

## Available experiments

### control_fd
In this experiment the tracee has three fds open: fd1, fd2 and fd3.
fd3 is a copy of fd2.
The tracer overrides fd3 with fd1 by using the ptrace syscall PTRACE_POKEDATA.

#### How to run
```
cd control_fd
make
./control_fd
```