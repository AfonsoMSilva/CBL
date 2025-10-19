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

typedef struct {
    size_t count;
    const char *data;
} Cbl_String_View;

Cbl_String_View cbl_sv_from_parts(const char *data, size_t count) {
    Cbl_String_View sv;
    sv.count = count;
    sv.data = data;
    return sv;
}

#define cbl_sb_to_sv(sb) cbl_sv_from_parts((sb).items, (sb).count)

Cbl_String_View cbl_sv_chop_by_delim(Cbl_String_View *sv, char delim)
{
    size_t i = 0;
    while (i < sv->count && sv->data[i] != delim) {
        i += 1;
    }
    
    Cbl_String_View result = cbl_sv_from_parts(sv->data, i);
    
    if(i < sv->count) {
        sv->count -= i + 1;
        sv->data  += i + 1;
    } else {
        sv->count -= i;
        sv->data  += i;
    }
    return result;
}
Cbl_String_View cbl_sv_chop_left(Cbl_String_View *sv, size_t n);
Cbl_String_View cbl_sv_trim(Cbl_String_View sv);
Cbl_String_View nob_sv_trim_left(Cbl_String_View sv);
Cbl_String_View nob_sv_trim_right(Cbl_String_View sv);



#ifndef CBL_STRIP_PREFIX_GUARD_
#define CBL_STRIP_PREFIX_GUARD_
    #ifdef NOB_STRIP_PREFIX
    #define read_entire_file cbl_read_entire_file
    #endif
#endif
//
