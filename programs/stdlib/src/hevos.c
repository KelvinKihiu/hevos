#include "hevos.h"
#include "string.h"

struct command_argument* hevos_parse_command(const char* command, int max)
{
    struct command_argument* root_command = 0;
    char scommand[1025];
    if (max >= (int) sizeof(scommand))
    {
        return 0;
    }
    
    strncpy(scommand, command, sizeof(scommand));
    char* token = strtok(scommand, " ");
    if (!token)
    {
        goto out;
    }

    root_command = hevos_malloc(sizeof(struct command_argument));
    if (!root_command)
    {
        goto out;
    }
    
    strncpy(root_command->argument, token, sizeof(root_command->argument));
    root_command->next = 0;

    struct command_argument* current = root_command;
    token = strtok(NULL, " ");
    while (token != 0)
    {
        struct command_argument* new_command = hevos_malloc(sizeof(struct command_argument));
        if (!new_command)
        {
            break;
        }
        
        strncpy(new_command->argument, token, sizeof(new_command->argument));
        new_command->next = 0x00;
        current->next = new_command;
        current = new_command;
        token = strtok(NULL, " ");
    }
    
out:
    return root_command;
}

int hevos_getkeyblock()
{
    int val = hevos_getkey();
    do
    {
        val = hevos_getkey();
    } while (val == 0);
}


void hevos_terminal_readline(char* out, int max, bool output_while_typing)
{
    int i = 0;
    for (i = 0; i < max; i++)
    {
        char key = hevos_getkeyblock();

        if (key == 13)
        {
            break;
        }

        if (key == 0x08 && i == 0)
        {
            i = -1;
            continue;
        }
        
        if (output_while_typing)
        {
            hevos_putchar(key);
        }
        
        if (key == 0x08 && i >= 1)
        {
            out[i-1] == 0x00;
            i -= 2;
            continue;
        }
        
        out[i] = key;
    }
    
    out[i] = 0x00;
}

int hevos_system_run(const char* command)
{
    char buf[1024];
    strncpy(buf, command, sizeof(buf));
    struct command_argument* root_command_argument = hevos_parse_command(buf, sizeof(buf));
    if (!root_command_argument)
    {
        return -1;
    }

    return hevos_system(root_command_argument);
}