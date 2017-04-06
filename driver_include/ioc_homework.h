/*  Homework ioctl() command codes
    * Original author = JOhn Noble @ 23 Oct 2010
    */

#ifndef _S_I_HOMEWORK_H
#define _S_I_HOMEWORK_H

#include <minix/ioctl.h>

#define HIOCSLOT       _IOW('h', 3, u32_t)
#define HIOCCLEARSLOT   _IOW('h', 4, u32_t)
#define HIOCGETSLOT     _IOR('h', 5, u32_t)

#endif /* _S_I_HOMEWORK_H */
