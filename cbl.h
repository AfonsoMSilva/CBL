/* 
 */

#ifndef CBL_H_
#define CBL_H_
#endif

#ifndef CBL_ASSERT
#include <assert.h>
#define CBL_ASSERT assert
#endif /* CBL_ASSERT */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>

#ifndef CBL_REALLOC
#include <stdlib.h>
#endif

#define cbl_return_defer(value) do { result = (value); goto defer; } while(0)
#define CBL_DECLTYPE_CAST(T)


typedef struct {
    char *items;
    size_t count;
    size_t capacity;
} Cbl_String_Builder;

bool cbl_read_entire_file(const char *path, Cbl_String_Builder *sb)
{
    bool result = true;

    FILE *f = fopen(path, "rb");
    size_t new_count = 0;
    long long m = 0;
    if (f == NULL)                  cbl_return_defer(false);
    if (fseek(f, 0, SEEK_END) < 0)  cbl_return_defer(false);
    m = ftell(f);
    if (m < 0)                      cbl_return_defer(false);
    if (fseek(f, 0, SEEK_SET) < 0)  cbl_return_defer(false);
    
    new_count = sb->count + m;
    if (new_count > sb->capacity) {
        sb->items = CBL_DECLTYPE_CAST(sb->items)realloc(sb->items, new_count);
        CBL_ASSERT(sb->items != NULL && "TOO BIG DOESNT FIT");
        sb->capacity = new_count;
    }

    fread(sb->items + sb->count, m, 1, f);
    if (ferror(f)) {
        cbl_return_defer(false);
    }
    sb->count = new_count;

defer:
    if (!result) return 69; //TODO
    if (f) fclose(f);
    return result;
}

#ifndef CBL_STRIP_PREFIX_GUARD_
#define CBL_STRIP_PREFIX_GUARD_
    #ifdef NOB_STRIP_PREFIX
    #define read_entire_file cbl_read_entire_file
    #endif
#endif
//
