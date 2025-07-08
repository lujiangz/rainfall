#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	o(void)
{
	system("/bin/sh");
	_exit(1);
}

void	n(void)
{
	char	buffer[520];

	fgets(buffer, 0x200, stdin); // 512
	printf(buffer);
	exit(1);
}

int		main(void)
{
	n();
	return (0);
}