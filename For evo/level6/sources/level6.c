#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// n() is the function we want to call. It prints the password.
void n(void) {
    system("/bin/cat /home/user/level7/.pass");
}

// m() is the function that is normally called.
void m(void) {
    puts("Nope");
}

// The main function contains a buffer overflow vulnerability.
int main(int argc, char *argv[]) {
    char *buffer;
    void (**func_ptr)(void); // A pointer to a function pointer

    // Allocate memory for a 64-byte buffer and a function pointer.
    buffer = (char *)malloc(64);
    func_ptr = malloc(sizeof(void *));

    // Make the function pointer point to the 'm' function.
    *func_ptr = m;

    // If an argument is provided, copy it into the buffer.
    // VULNERABILITY: No size check is performed, allowing a buffer overflow.
    // An attacker can overwrite the 'func_ptr' on the heap with the address of 'n'.
    if (argc > 1) {
        strcpy(buffer, argv[1]);
    }

    // Call the function pointed to by func_ptr.
    // Normally this is m(), but can be changed to n() via the overflow.
    (*func_ptr)();

    return 0;
}