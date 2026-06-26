#include "shell.h"
#include "cmd_system.h"
#include "cmd_process.h"
#include <stdio.h>

int main(void) {
    printf("sysmon starting...\n");
    shell_init();
    cmd_process_init();
    cmd_system_register_all();
    cmd_process_register_all();
    shell_run();
    return 0;
}