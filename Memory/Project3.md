# Project 3 Questions
---
- Brandon Tarney
- 4/9/2017

## 1.A. Explain how these addresses demonstrate the use of virtual addresses in Minix...

We can see from the addresses here [](VirtualMemoryAddresses.png) they appear to be contiguous with the text data followed by global data data followed by heap data and then stack data at the other end of the process memory allocation growing towards the heap. We know that the actual memory used is unlikely to be contiguous, which shows us the power of Virtual memory: making parts of memory which are not actually close appear-so. 


## 1.B. Does Minix 3.3.0 support a persistent storage "swap area" on disk that allows Minix to either run a process that requires more than the amount of available physica memory, or to run more processes than can fit into memory? Explain your answer by creating a process or set of processes that use more than the available physical memory.

Yes, Minix supports swap areas. We can see this by running a program that allocates > 1024MB of memory which would normally be impossible given only 1024mb of base memory was allocated for my Minix VM but because 12GB of storage were allocated to the virtual storage device, this is possible proving that Minix has swap capability. 


## 1.C. Show where in the Minix source where the virtual memory mapping is performed, and explain. 

It is performed in /usr/src/minix/servers/vm/pagetable.c
