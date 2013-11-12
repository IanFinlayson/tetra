#ifndef H_STRING_BUFFER
#define H_STRING_BUFFER

#include "tetra_hdr.h"

typedef struct string_buffer {
    char *buffer;
    int capacity;
    int filled;
} Str_Buf;

Str_Buf *make_string_buffer(int initial_size);

void dest_string_buffer(Str_Buf *buf);

void add_str(Str_Buf *buf, char *str);

void empty_buffer(Str_Buf *buf);

void reset_buffer(Str_Buf *buf);

#endif /* H_STRING_BUFFER */
