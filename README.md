# ptrace_experiments
Code snippets to test ptrace functionalities


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
