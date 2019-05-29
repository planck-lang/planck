#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char* argv[])
{
    while (true)
    {
        char* buf = readline(">> ");
        if (buf == NULL)
        {
            puts("\nAnnyuonghi Gaseyo.\n");
            break;
        }

        if (strlen(buf) > 0)
        {
            add_history(buf);
            printf("%s\n", buf);
        }

        free(buf);
    }
}
