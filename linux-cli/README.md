# sysmon — Linux System Monitor CLI

A lightweight system monitoring tool written in pure C, featuring an interactive command shell with modular command registration, process management via `fork/exec/wait`, and signal-based zombie process cleanup.

## Features

- **Command shell framework** — function pointer table for command dispatch, similar to the Linux kernel driver register/unregister pattern
- **System monitoring** — query uptime, memory usage, and disk usage via Linux system calls
- **Process management** — launch foreground and background processes using `fork()` / `execvp()` / `waitpid()`
- **Background process tracking** — track child processes and check their status
- **Signal handling** — `SIGCHLD` handler with `sigaction()` for automatic zombie process cleanup

## Architecture

```
include/
├── shell.h           — Shell framework interface (command registration, dispatch)
├── cmd_system.h      — System monitoring commands
└── cmd_process.h     — Process management commands

src/
├── main.c            — Entry point, initializes modules and starts the shell
├── shell.c           — Command line parser, tokenizer, and command dispatch loop
├── cmd_system.c      — System commands: help, uptime, mem, disk
└── cmd_process.c     — Process commands: exec, run, ps + SIGCHLD handler
```

Commands are registered via `shell_register()` with a function pointer table. The shell reads user input, tokenizes it into `argc/argv`, looks up the command in the table, and calls the corresponding handler. Adding a new command requires only writing a handler function and registering it — the shell core does not need to be modified.

## Build & Run

### Prerequisites

- GCC
- Make
- Linux environment (or Docker)

### Build

```bash
cd linux-cli
make
./sysmon
```

### Using Docker

```bash
docker run -it -v "$(pwd)/linux-cli:/work" -w /work ubuntu:22.04 bash
apt update && apt install -y gcc make procps
make clean && make
./sysmon
```

## Usage

```
$ ./sysmon
sysmon starting...
sysmon> help
help      Show available commands
uptime    Show system uptime
mem       Show memory usage
disk      Show disk usage
exec      Execute a program
run       Run a program in background
ps        List background processes

sysmon> uptime
Uptime: 0h 15m 42s

sysmon> mem
Memory: used 312 MB / total 7974 MB

sysmon> disk
Disk: used 15234 MB / total 61385 MB

sysmon> exec ls -la
total 28
drwxr-xr-x 4 root root  128 Jun 26 06:44 .
-rw-r--r-- 1 root root  749 Jun 26 06:31 Makefile
drwxr-xr-x 6 root root  192 Jun 26 06:18 src
...

sysmon> run sleep 5
Started [PID 1234]

sysmon> ps
[PID 1234] alive

sysmon> ps
[PID 1234] exited

sysmon> exit
```

## Technical Highlights

| Topic | API / Technique |
|-------|----------------|
| System info | `sysinfo()` — uptime, total/free RAM |
| Filesystem | `statvfs()` — disk usage query |
| Process creation | `fork()` + `execvp()` — spawn child processes |
| Process synchronization | `waitpid()` — wait for child, reap zombies |
| Signal handling | `sigaction()` + `SIGCHLD` — async zombie cleanup |
| Command dispatch | Function pointer table — extensible command registration |
