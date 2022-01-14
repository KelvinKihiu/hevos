#ifndef HEVOS_H
#define HEVOS_H

#include <stddef.h>
#include <stdbool.h>


struct command_argument
{
    char argument[512];
    struct command_argument* next;
};

struct process_arguments
{
    int argc;
    char** argv;
};


void hevos_exit(int err_code);
void print(const char* message);
int hevos_getkey();
void* hevos_malloc(size_t size);
void hevos_free(void* size);
void hevos_putchar(char c);
int hevos_getkeyblock();
void hevos_terminal_readline(char* out, int max, bool output_while_typing);
void hevos_process_load_start(const char* filename);
struct command_argument* hevos_parse_command(const char* command, int max);
void hevos_process_get_arguments(struct process_arguments* arguments);
int hevos_system(struct command_argument* argument);
int hevos_system_run(const char* command);

#endif