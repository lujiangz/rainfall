#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void n(void) {
    system("/bin/cat /home/user/level7/.pass");
}

void m(void) {
    puts("Nope");
}

int main(int argc, char *argv[]) {
    char *buffer;
    void (**func_ptr)(void); // A pointer to a function pointer

    buffer = (char *)malloc(64);
    func_ptr = malloc(sizeof(void *));

    *func_ptr = m;

    if (argc > 1) {
        strcpy(buffer, argv[1]);
    }

    (*func_ptr)();

    return 0;
}