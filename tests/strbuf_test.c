#include <stdio.h>
#include "../parser/string_buffer.h"
#include "../parser/exit_funcs.h"

#define PBUF printf("Buffer: %s\n", buf->buffer)
#define PSIZ printf("Filled: %d\n", buf->filled)
#define PCAP printf("Capacity: %d\n", buf->capacity)
#define PALL printf("Buffer: %s\n", buf->buffer); \
             printf("Filled: %d\n", buf->filled); \
             printf("Capacity: %d\n", buf->capacity)


int main(int argc, char **argv)
{
    int i;
    Str_Buf *buf;
    char add[10];

    buf = make_str_buf(10);
    PALL;
    append_to_str_buf(buf, "Hiya.");
    PALL;
    append_to_str_buf(buf, " This should be long enough.");
    PALL;
    reset_str_buf(buf, 5);
    PALL;

    for (i = 0; i < 20; i++) {
        sprintf(add, " %d", i);
        append_to_str_buf(buf, add);
    }
    PALL;

    for (i = 0; i < 20000; i++) {
        reset_str_buf(buf, 5);
        append_to_str_buf(buf, "Yoyoyoyoyoyoyoyoyoyo");
    }

    return 0;
}
