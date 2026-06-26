// handler definition: input: int argc, char *argv[] ; output: int
typedef int (*cmd_handler_t)(int argc, char *argv[]);

// command struct
typedef struct {
    const char *name;  // name
    const char *desc;  // description
    cmd_handler_t handler; // handler
} shell_cmd_t;

// Initialization 
void shell_init(void);

// register a command
int shell_register(const shell_cmd_t *cmd);

// get command
const shell_cmd_t *shell_get_cmds(int *count);

// main function
void shell_run(void);
