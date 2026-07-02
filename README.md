# Embedded UART Command Shell on ESP32

A modular UART command shell running on FreeRTOS, built with ESP-IDF + Arduino framework (PlatformIO). Designed to demonstrate embedded systems concepts relevant to Linux SDK and driver development.

## Architecture

```
┌──────────────────────────────────────────────┐
│                  main.cpp                    │
│  setup(): init modules, create FreeRTOS tasks│
└───────┬─────────────────────┬────────────────┘
        │                     │
        ▼                     ▼
┌──────────────┐     ┌─────────────────┐
│  shell_task  │     │  monitor_task   │
│  (FreeRTOS)  │     │  (FreeRTOS)     │
│              │     │  - Heartbeat LED│
│  uart_io     │     │  - Heap watchdog│
│  ┌─────────┐ │     └─────────────────┘
│  │ getchar │ │
│  │ putchar │ │
│  │ puts    │ │
│  │ printf  │ │
│  └─────────┘ │
│              │
│  Command Table (function pointer)
│  ┌─────────────────────────────┐
│  │ help  → cmd_help()          │
│  │ free  → cmd_free()          │
│  │ uptime→ cmd_uptime()        │
│  │ ps    → cmd_ps()            │
│  │ reboot→ cmd_reboot()        │
│  │ gpio  → cmd_gpio()          │
│  └─────────────────────────────┘
│              │
│         HAL Layer
│  ┌─────────────────────────────┐
│  │ hal_gpio_driver_t           │
│  │  .set_mode → esp32 driver   │
│  │  .write    → esp32 driver   │
│  │  .read     → esp32 driver   │
│  └─────────────────────────────┘
└──────────────┘
```

## Module Overview

| Module | Files | Description |
|--------|-------|-------------|
| UART I/O | `uart_io.c/h` | ESP-IDF UART driver wrapper (8N1, init/getchar/puts/printf) |
| Shell | `shell.c/h` | FreeRTOS task, line editor, command dispatch via function pointer table |
| System Commands | `cmd_system.c/h` | `help`, `free`, `uptime`, `ps`, `reboot` |
| HAL GPIO | `hal_gpio.c/h` | Driver abstraction with struct + function pointers (Linux `file_operations` pattern) |
| GPIO Commands | `cmd_gpio.c/h` | `gpio mode/set/get` shell commands |
| Task Monitor | `task_monitor.c/h` | Background task: heartbeat LED blink + heap low-watermark warning |

## Build & Flash

**Requirements:** [PlatformIO](https://platformio.org/) CLI or VSCode extension

```bash
# Build
pio run

# Flash
pio run -t upload

# Open serial monitor (115200 baud)
pio device monitor
```

## Usage

Connect via serial terminal at **115200 baud**. Press **EN (Reset)** after flashing.

```
=== RTOS Shell ===
rtos> help
help    Show available commands
free    Show heap usage
uptime  Show system uptime
ps      List background processes
reboot  Restart
gpio    Control GPIO pins

rtos> free
Heap free: 236452 bytes
Min free:  224312 bytes (all-time low)

rtos> uptime
Uptime: 0h 1m 23s

rtos> ps
Tasks: 6
Current: shell

rtos> gpio mode 2 out
rtos> gpio set 2 1
rtos> gpio set 2 0
rtos> gpio get 2
pin 2 = 0

rtos> reboot
Rebooting...
```

## Design Highlights

### Function Pointer Command Table
Commands are registered at runtime via `shell_register()`, keeping the shell core decoupled from command implementations. Adding a new command requires no changes to `shell.c`.

```c
typedef int (*cmd_handler_t)(int argc, char *argv[]);

typedef struct {
    const char *name;
    const char *desc;
    cmd_handler_t handler;
} shell_cmd_t;
```

This pattern mirrors Linux kernel driver registration (`register_chrdev`, `file_operations`).

### HAL Driver Abstraction
GPIO is accessed through a driver interface struct, not called directly. This makes the hardware swappable without changing higher-level code.

```c
typedef struct {
    int (*set_mode)(int pin, int mode);
    int (*write)(int pin, int value);
    int (*read)(int pin);
} hal_gpio_driver_t;
```

### FreeRTOS Multi-task
Two independent tasks run concurrently:
- `shell_task` (priority 2): handles UART input/output and command execution
- `monitor_task` (priority 1): blinks LED every 500ms and prints heap warnings if free heap drops below 10KB

### UART Character-by-Character Input
Unlike Linux `fgets`, the ESP32 has no terminal driver. `shell_task` reads one byte at a time with timeout, manually handles echo and backspace, and processes a line only when `\r` is received.

## Project Structure

```
.
├── include/
│   ├── uart_io.h
│   ├── shell.h
│   ├── cmd_system.h
│   ├── hal_gpio.h
│   ├── cmd_gpio.h
│   └── task_monitor.h
├── src/
│   ├── main.cpp
│   ├── uart_io.c
│   ├── shell.c
│   ├── cmd_system.c
│   ├── hal_gpio.c
│   ├── cmd_gpio.c
│   └── task_monitor.c
├── linux-cli/          # Companion Linux CLI project
└── platformio.ini
```

## Companion Project

[`linux-cli/`](linux-cli/) contains a parallel implementation of the same shell architecture targeting Linux (x86), using POSIX APIs (`fork`, `execvp`, `sigaction`, `sysinfo`, `statvfs`). Demonstrates the same command dispatch pattern adapted for a full OS environment.
