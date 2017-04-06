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
	int slot = 2;

	if (argc > 1)
		slot = atoi (argv[1]);
	if (slot < 0 || slot > 4)
	{
		printf ("Bad slot %d\n", slot);
		exit (1);
	}

	fd = open (HMWRK_DEV, O_RDWR);
	if (fd < 0)
		perror ("open"), exit (-1);

	printf ("CLearing slot %d\n", slot);

	if (ioctl (fd, HIOCCLEARSLOT, &slot) < 0)
	{
		perror ("ioctl");
	}

	close (fd);
	exit (0);
}
