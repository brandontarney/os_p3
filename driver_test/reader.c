#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define HMWRK_DEV "/dev/homework"

/*
 * Reads an integer from the current slot
 */
int
main (int argc, char *argv[])
{
	int fd;
	int foo;

	fd = open (HMWRK_DEV, O_RDWR);
	if (fd < 0)
		perror ("open"), exit (-1);

	if (read (fd, &foo, sizeof (foo)) < 0)
	{
		perror ("read");
		exit (1);
	}
	printf ("I read %d\n", foo);
	close (fd);
	exit (0);
}
