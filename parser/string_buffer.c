#include "string_buffer.h"

Str_Buf *make_str_buf(int initial_size)
{
    Str_Buf *buf;
    if ((buf = malloc(sizeof(Str_Buf))) == NULL)
        err_exit("Allocating string buffer");
    init_str_buf(buf, initial_size);
    return buf;
}

void init_str_buf(Str_Buf *buf, int capacity)
{
    if ((buf->buffer = calloc(capacity, sizeof(char))) == NULL)
        err_exit("Allocating string buffer");
    buf->capacity = capacity;
    buf->filled = 1; /* account for terminating NUL */
}

void dest_str_buf(Str_Buf *buf)
{
    free(buf->buffer);
    free(buf);
}

void append_to_str_buf(Str_Buf *buf, char *str)
{
    char *new_buf;
    int new_cap;
    int len = strlen(str);
    if (buf->filled + len > buf->capacity) {
        new_cap = buf->capacity;
        while (buf->filled + len > new_cap)
            new_cap *= 2;
        new_buf = calloc(new_cap, sizeof(char));
        if (new_buf == NULL)
            err_exit("Increasing string buffer capacity");
        strcpy(new_buf, buf->buffer);
        free(buf->buffer);
        buf->buffer = new_buf;
        buf->capacity = new_cap;
    }
    strcpy(buf->buffer + (buf->filled - 1), str);
    buf->filled += len;
}

void empty_str_buf(Str_Buf *buf)
{
    buf->filled = 1;
    buf->buffer[0] = '\0';
}

void reset_str_buf(Str_Buf *buf, int capacity)
{
    free(buf->buffer);
    init_str_buf(buf, capacity);
}
