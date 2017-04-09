/*  Program to test and interact with Minix virtual memory
 *    @Author Brandon Tarney
 *    @Date   4/9/2016
 */

#include <stdio.h> 
#include <stdlib.h> 

int globalData;


/*
 * Dummy Function to add something to the stack for getting the address
 */
void * stackFcn()
{
        int stackData = 42;
        return &stackData;
}

/*
 * Prints out the (approximate) addresses of the text area, the global data area, the heap, and the stack
 */
int main (int argc, char *argv[]) 
{
        void *stackAdrs, *dataAdrs, *heapAdrs;
        void *(*txtAdrs)();

        //const int txtData = 42;
        txtAdrs = &stackFcn;
        dataAdrs = &globalData;
        stackAdrs = stackFcn();
        heapAdrs = malloc(sizeof(int));


        printf("Address of text (%p)\nAddress of global data (%p)\nAddress of heap (%p)\nAddress of stack (%p)\n",
                        txtAdrs, dataAdrs, heapAdrs, stackAdrs);

        free(heapAdrs);

        exit(0);
}
