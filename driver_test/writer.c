#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define HMWRK_DEV "/dev/homework"

/*
 * Writes an integer from the current slot
 * The value can be optionally provided on the command line.
 */
int
main (int argc, char *argv[])
{
	int fd;
	int foo;

	fd = open (HMWRK_DEV, O_RDWR);
	if (fd < 0)
		perror ("open"), exit (-1);

	foo = 30;
	if (argc > 1)
	{
		foo = atoi (argv[1]);
	}
	printf ("I will write %d\n", foo);

	if (write (fd, &foo, sizeof (foo)) < 0)
	{
		perror ("write");
		exit (1);
	}
	close (fd);
	exit (0);
}
