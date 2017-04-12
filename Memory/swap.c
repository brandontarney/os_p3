// Prove swapping capabilities in MINIX by using more than memory size to run this program

#include <stdio.h>
#include <stdlib.h>

const int megabyte = 1024 * 1000;

int main(int argc, char *argv[])
{
        int memory_size; 
        int counter = 10;
        while (counter < 10000)
        {
                int memory_size = megabyte * counter;
                int * ptr = (int*)malloc(memory_size);
                if (ptr == NULL)
                {
                        printf("memory allocation of size ~%d MB FAILED (no swapping here), exiting\n", memory_size/1000000);
                        exit(1);
                }
                printf("I just allocated ~%d MB of memory without a segfault!\n", memory_size/1000000);
                free(ptr);
                counter = counter * 10;
        }
        exit(0);
}
