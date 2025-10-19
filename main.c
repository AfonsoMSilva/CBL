#include <stdio.h>
#include "cbl.h"

int main(){
    const char *path = "test.txt";
    Cbl_String_Builder buff = {0};
    if (!cbl_read_entire_file(path, &buff)) return 1;
    printf("%s", buff.items);
    printf("%zu\n", buff.count);

    Cbl_String_View content = { 
        .data = buff.items,
        .count = buff.count,
    };

    for(size_t i = 0; i < 10 && content.count > 0; i++) {
        Cbl_String_View token = cbl_sv_chop_by_delim(&content, ' ');
        printf("%s\n", token.data);
    }
    return 0;
}
