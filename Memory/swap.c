// Prove swapping capabilities in MINIX by using more than memory size to run this program

#include <stdio.h>
#include <stdlib.h>

const int available_bytes = 1024 * 1000000;

int main(int argc, char *argv[])
{
        void * ptr = malloc(available_bytes + 1);
        printf("I just allocated 1025 MB of memory without a segfault!\n");
        free(ptr);
        exit(0);
}
