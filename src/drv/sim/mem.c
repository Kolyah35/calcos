#include <mem.h>
#include <stdint.h>
#include <malloc.h>

unsigned int get_used_mem() {
#ifdef __linux__
    struct mallinfo2 mi = mallinfo2();

    return mi.uordblks;
#endif
}