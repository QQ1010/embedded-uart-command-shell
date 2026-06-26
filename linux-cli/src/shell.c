#include "shell.h"
#include <stdio.h>
#include <string.h>

#define MAX_ARGS 16
#define MAX_CMDS 16
#define MAX_LINE 256
static shell_cmd_t cmds[MAX_CMDS];
static int cmd_count = 0;


// Initialization 
void shell_init(void) {
    cmd_count = 0;
    memset(cmds, 0, sizeof(cmds));
}

// register a command
int shell_register(const shell_cmd_t *cmd) {
    if(cmd == NULL || cmd->name == NULL || cmd->handler == NULL) {
        return -1;
    }

    if(cmd_count >= MAX_CMDS) {
        return -1;
    }
    
    cmds[cmd_count++] = *cmd;
    return 0;
}

// get command
const shell_cmd_t *shell_get_cmds(int *count) {
    if(count != NULL) {
        *count = cmd_count;
    }
    return cmds;
}

// main function
void shell_run(void) {
    while(1) {
        printf("sysmon> ");
        char line[MAX_LINE];
        if(fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        // remove \n
        line[strcspn(line, "\n")] = '\0';
        // no input
        if(strlen(line) == 0) {
            continue;
        }
        // exit
        if(strcmp(line, "exit") == 0) {
            break;
        }
        // tokenize
        char *argv[MAX_ARGS];
        int argc = 0;

        char *token = strtok(line, " ");

        while(token != NULL && argc < MAX_ARGS) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        
        argv[argc] = NULL;
        
        if(argc == 0) {
            continue;
        }

        // find command in table
        int found = 0;
        for(int i = 0 ; i < cmd_count; i++) {
            if(strcmp(argv[0], cmds[i].name) == 0) {
                int ret = cmds[i].handler(argc, argv);

                if(ret != 0) {
                    printf("command failed: %s, ret=%d\n", argv[0], ret);
                }

                found = 1;
                break;
            }
        }
        if(!found) {
            printf("unknown command: %s\n", argv[0]);
        }
    }
}