#include "hevos.h"
#include "stdio.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        print("Error: Invalid arguments.\n");
        print("Usage: echo args\n");

        return 0;
    }

    for (int i = 1; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }

    print("\n");   

    return 0;
}