
#include <stdio.h>
#include <stdlib.h>

__attribute__((export_name("allocatemem")))
char* allocatemem(int size) {

    char* ptr = (char*) malloc(size * sizeof(char));
    return ptr;
}


__attribute__((export_name("freemem")))
int freemem(char* pointer) {
    
    free (pointer);
    return 0;
}
