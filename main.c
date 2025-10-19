#include <stdio.h>
#include "cbl.h"

int main(){
    const char *path = "test.txt";
    Cbl_String_Builder content = {0};
    if (!cbl_read_entire_file(path, &content)) return 1;
    printf("%s", content.items);
    printf("%zu", content.count);
    
    return 0;
}
