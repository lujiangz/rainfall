#include <stdio.h>
#include <stdlib.h>

int m; 

void main(void)
{
    v();
    return;
}

void v(void)
{
    char local_20c[520];

    fgets(local_20c, 0x200, stdin); // 0x200 == 512
    printf(local_20c);

    if (m == 0x40) {      // 0x40 == 64
        fwrite("Wait what?!\n", 1, 0xc, stdout);
        system("/bin/sh");
    }
    return;
}