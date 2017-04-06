#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define HMWRK_DEV "/dev/homework"

int
main (int argc, char *argv[])
{
	int fd;

	fd = open (HMWRK_DEV, O_RDWR);

	close (fd);
	exit (0);
}
