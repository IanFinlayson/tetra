#ifndef H_STRING_BUFFER
#define H_STRING_BUFFER

#include "tetra_hdr.h"
#include <string.h>
#include <stdlib.h>

typedef struct str_buf {
    char *buffer;
    int capacity;
    int filled;
} Str_Buf;

Str_Buf *make_str_buf(int initial_size);

void init_str_buf(Str_Buf *buf, int capacity);

void dest_str_buf(Str_Buf *buf);

void append_to_str_buf(Str_Buf *buf, char *str);

void empty_str_buf(Str_Buf *buf);

void reset_str_buf(Str_Buf *buf, int capacity);

#endif /* H_STRING_BUFFER */
