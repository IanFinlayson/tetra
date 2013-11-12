#include "string_buffer.h"
#include <string.h>

Str_Buf *make_string_buffer(int initial_size)
{
    Str_Buf *buf;
    if ((buf = malloc(sizeof(Str_Buf))) == NULL)
        err_exit("Allocating string buffer");
    if ((buf->buffer = calloc(initial_size, sizeof(char))) = NULL)
        err_exit("Allocating string buffer");
    buf->capacity = initial_size;
    buf->filled = 1; /* account for terminating null-character */
}

void dest_string_buffer(Str_Buf *buf)
{
    free(buf->buffer);
    free(buf);
}

void add_str(Str_Buf *buf, char *str)
{
    char *new_buf;
    int len = strlen(str);
    //int old_end = strlen(buf->buffer);
    int growth = len + buf->capacity;
    if (buf->filled + len > buf->capacity) {
        new_buf = calloc(growth + buf->capacity, sizeof(char));
        if (new_buf == NULL)
            err_exit("Increasing string buffer capacity");
        strcpy(new_buf, buf->buffer);
        free(buf->buffer);
        buf->buffer = new_buf;
        buf->capacity += growth;
    }
    strcpy(buf->buffer + (buf->filled - 1), str);
    buf->filled += len;
}

void empty_buffer(Str_Buf *buf)
{
    buf->filled = 1;
    buf->buffer[0] = '\0';
}

void reset_buffer(Str_Buf *buf)
{

}
