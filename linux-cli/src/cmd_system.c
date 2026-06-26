#include "cmd_system.h"
#include "shell.h"
#include <stdio.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>


static int cmd_help(int argc, char *argv[]) {
    (void)argc; (void)argv;
    int count;
    const shell_cmd_t *list = shell_get_cmds(&count);
    for(int i = 0 ; i < count; i++) {
        printf("%s      %s\n", list[i].name, list[i].desc);
    }
    return 0;
}

static int cmd_uptime(int argc, char *argv[]) {
    (void)argc; (void)argv;
    struct sysinfo info;
    sysinfo(&info);
    int hours, minutes, seconds;
    long uptime = info.uptime;
    hours   = uptime / 3600;
    minutes = (uptime % 3600) / 60;
    seconds = uptime % 60;
    printf("Uptime: %dh %dm %ds\n", hours, minutes, seconds);
    return 0;
}

static int cmd_mem(int argc, char *argv[]) {
    (void)argc; (void)argv;
    struct sysinfo info;
    sysinfo(&info);
    
    long total_mem = info.totalram * info.mem_unit / (1024 * 1024);
    long free_mem = info.freeram * info.mem_unit / (1024 * 1024);
    long used_mem = total_mem - free_mem;
    
    printf("Memory: used %ld MB / total %ld MB\n", used_mem, total_mem);
    return 0;
}

static int cmd_disk(int argc, char *argv[]) {
    (void)argc; (void)argv;
    struct statvfs stat;
    statvfs("/", &stat);

    long total_bytes = stat.f_blocks * stat.f_frsize / (1024 * 1024);
    long free_bytes = stat.f_bfree * stat.f_frsize / (1024 * 1024);
    long used_bytes  = total_bytes - free_bytes;
    
    printf("Disk: used %ld MB / total %ld MB\n", used_bytes, total_bytes);
    return 0;
}

void cmd_system_register_all(void) {
    static const shell_cmd_t cmds[] = {
        {"help", "Show available commands", cmd_help},
        {"uptime", "Show system uptime",    cmd_uptime},
        {"mem", "Show memory usage", cmd_mem},
        {"disk", "Show dist usage", cmd_disk},
    };
    for (int i = 0; i < (int)(sizeof(cmds)/sizeof(cmds[0])); i++) {
        shell_register(&cmds[i]);
    }
}
