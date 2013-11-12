#include "exit_funcs.h"
#include <stdio.h>
#include <stdlib.h>

void err_exit(char *msg)
{
    fprintf(stderr, "%s.\n", msg);
    exit(1);
}

void ferr_exit(char *msg, FILE *file)
{
    if (feof(file))
        fprintf(stderr, "Unexpected EOF reached.\n");
    else if (ferror(file))
        perror(msg);
    else
        fprintf(stderr, "%s.\n", msg);
    exit(1);
}
