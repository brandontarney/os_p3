#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ioc_homework.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define HMWRK_DEV "/dev/homework"

/*
 * Clears a given slot
 * @Author Brandon Tarney
 * @Date 3/14/2017
 *
 * @params [SLOT]
 * @return 
 */

int
main (int argc, char *argv[])
{
	int fd;
	int foo;

	fd = open (HMWRK_DEV, O_RDWR);
	if (fd < 0)
		perror ("open"), exit (-1);

	printf ("CLearing current slot");

	if (ioctl (fd, HIOCCLEARSLOT, NULL) < 0)
	{
		perror ("ioctl");
	}

	close (fd);
	exit (0);
}
