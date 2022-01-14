#include "shell.h"
#include "hevos.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char** argv)
{
    print("Hevos v1.0.0\n");

    while(1) 
    {
        print("> ");
        char buf[1024];
        hevos_terminal_readline(buf, sizeof(buf), true);
        print("\n");
        hevos_system_run(buf);
    }

    return 0;
}