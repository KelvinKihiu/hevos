#include "hevos.h"

extern int main(int argc, char** argv);

int c_start()
{
    struct process_arguments arguments;
    hevos_process_get_arguments(&arguments);

    return main(arguments.argc, arguments.argv);
}