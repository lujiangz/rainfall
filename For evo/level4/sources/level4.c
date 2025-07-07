#include <stdio.h>
#include <stdlib.h>

int		m = 0;

int		p(char *buffer)
{
	printf(buffer);
	return (0);
}

int		n(void)
{
	char	buffer[520];

	fgets(buffer, 0x200, stdin); // 512
	p(buffer);
	if (m == 0x1025544) // 16930116
		system("/bin/cat /home/user/level5/.pass");
	return (0);
}

int		main(void)
{
	n();
	return (0);
}