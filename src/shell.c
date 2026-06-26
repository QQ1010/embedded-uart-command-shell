#include "shell.h"
#include "uart_io.h"
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

static int tokenize(char *line, char *argv[], int max_args)
{
    int argc = 0;
    char *token = strtok(line, " ");

    while (token != NULL && argc < max_args) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }
    argv[argc] = NULL;
    return argc;
}

static void process_line(char *line)
{
    char *argv[MAX_ARGS];
    int argc = tokenize(line, argv, MAX_ARGS);

    if (argc == 0)
        return;

    for (int i = 0; i < cmd_count; i++) {
        if (strcmp(argv[0], cmds[i].name) == 0) {
            int ret = cmds[i].handler(argc, argv);
            if (ret != 0)
                uart_io_printf("command failed: %s, ret=%d\r\n", argv[0], ret);
            return;
        }
    }
    uart_io_printf("unknown command: %s\r\n", argv[0]);
}

void shell_task(void *param) {
    (void) param;
    char line[MAX_LINE];
    int pos = 0;

    uart_io_puts("rtos> ");

    for(;;) {
        int c = uart_io_getchar(50);
        if(c < 0)
            continue;
        if(c == '\n')
            continue;
        if(c == '\r') {
            // Enter
            uart_io_puts("\r\n");
            line[pos] = '\0';
            if(pos > 0) {
                process_line(line);
            }
            pos = 0;
            uart_io_puts("rtos> ");
        }
        else if(c == '\b' || c == 0x7F) {
            // Backspace
            if(pos > 0) {
                pos --;
                uart_io_puts("\b \b");
            }
        }
        else if(pos < MAX_LINE - 1) {
            // general character => put into buffer
            line[pos++] = (char)c;
            uart_io_putchar((char)c);
        }
    }
}