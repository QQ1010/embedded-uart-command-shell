#ifndef SHELL_H
#define SHELL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*cmd_handler_t)(int argc, char *argv[]);

typedef struct {
    const char *name;
    const char *desc;
    cmd_handler_t handler;
} shell_cmd_t;

void shell_init(void);
int shell_register(const shell_cmd_t *cmd);
const shell_cmd_t *shell_get_cmds(int *count);
void shell_task(void *param);


#ifdef __cplusplus
}
#endif

#endif