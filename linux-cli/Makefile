CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude

sysmon: build/main.o build/shell.o build/cmd_system.o build/cmd_process.o
		$(CC) $(CFLAGS) -o sysmon build/main.o build/shell.o build/cmd_system.o build/cmd_process.o

build:
	mkdir -p build

build/main.o: src/main.c | build
		$(CC) $(CFLAGS) -c src/main.c -o build/main.o

build/shell.o: src/shell.c include/shell.h | build
		$(CC) $(CFLAGS) -c src/shell.c -o build/shell.o

build/cmd_system.o: src/cmd_system.c include/cmd_system.h | build
		$(CC) $(CFLAGS) -c src/cmd_system.c -o build/cmd_system.o

build/cmd_process.o: src/cmd_process.c include/cmd_process.h | build
		$(CC) $(CFLAGS) -c src/cmd_process.c -o build/cmd_process.o

.PHONY: clean
clean:
	rm -f *.o sysmon
	rm -rf build sysmon

