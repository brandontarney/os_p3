#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ioc_homework.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define HMWRK_DEV "/dev/homework"

/*
 * Returns the current slot in-use
 * @Author Brandon Tarney
 * @Date 3/14/2017
 * @params 
 * @return slot in use
 */

int
main (int argc, char *argv[])
{
	int fd;
	int foo;
	int slot = 2;
	int nslot = -1;

	fd = open (HMWRK_DEV, O_RDWR);
	if (fd < 0)
		perror ("open"), exit (-1);


	if (ioctl (fd, HIOCGETSLOT, &nslot) < 0)
	{
		perror ("ioctl");
	}

	printf ("Slot in use: %d\n", nslot);

	close (fd);
	exit (0);
}
