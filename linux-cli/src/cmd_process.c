#define _POSIX_C_SOURCE 200809L

#include "cmd_process.h"
#include "shell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>

#define MAX_CHILDREN 16
static pid_t children[MAX_CHILDREN];
static int child_count = 0;


static void sigchld_handler(int sig) {
    (void)sig;
    // 用 waitpid 回收所有已結束的子 process
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}

void cmd_process_init(void) {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);
}

static int cmd_exec(int argc, char *argv[]) {
    if(argc < 2) {
        return -1;
    }

    pid_t pid = fork();

    if(pid == 0) {
        // Child
        execvp(argv[1], &argv[1]);

        perror("execvp");
        _exit(1);
    }
    else if(pid > 0) {
        // Parent
        int status;
        waitpid(pid, &status, 0);
        return 0;
    }
    else {
        // fork fail
        return -1;
    }
}

static int cmd_run(int argc, char *argv[]) {
    if(argc < 2) {
        return -1;
    }

    pid_t pid = fork();

    if(pid == 0) {
        // Child
        execvp(argv[1], &argv[1]);

        perror("execvp");
        _exit(1);
    }
    else if(pid > 0) {
        // Parent
        children[child_count++] = pid;
        printf("Started [PID %d]\n", pid);
        return 0;
    }
    else {
        // fork fail
        return -1;
    }
}

static int cmd_ps(int argc, char *argv[]) {
    (void)argc; (void)argv;
    for(int i = 0 ; i < child_count; i++) {
        if(kill(children[i], 0) == 0) {
            printf("[PID %d] alive\n", children[i]);
        }
        else {
            printf("[PID %d] exited\n", children[i]);
        }
    }
    return 0;
}

void cmd_process_register_all(void) {
    static const shell_cmd_t cmds[] = {
        {"exec", "Execute a program", cmd_exec},
        {"run", "Run a program in background", cmd_run},
        {"ps", "List background processes", cmd_ps},
    };
    for(int i = 0 ; i < (int)(sizeof(cmds) / sizeof(cmds[0])); i++) {
        shell_register(&cmds[i]);
    }
}