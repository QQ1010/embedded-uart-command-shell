#include "shell.h"
#include "cmd_system.h"
#include <stdio.h>

int main(void) {
    printf("sysmon starting...\n");
    shell_init();
    cmd_system_register_all();
    shell_run();
    return 0;
}