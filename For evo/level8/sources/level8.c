#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Global pointers that will be manipulated by user commands.
char *auth = NULL;
char *service = NULL;

int main(void) {
    // Buffer to hold user input.
    char input_buffer[128];

    // Main loop to process commands indefinitely.
    while (1) {
        printf("auth pointer: %p, service pointer: %p\n", auth, service);

        // Read a line of input from the user.
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            // Exit if there's an error or end-of-file.
            return 0;
        }

        // Command: auth <name>
        // Allocates a small buffer and copies the name into it.
        if (strncmp(input_buffer, "auth ", 5) == 0) {
            // --- VULNERABILITY 1: Buffer Overflow ---
            // It allocates only 4 bytes but then copies up to 30 bytes,
            // causing a heap overflow.
            auth = malloc(4);
            // The argument starts 5 characters after the beginning of the input.
            char *auth_name = input_buffer + 5;
            if (strlen(auth_name) < 31) {
                strcpy(auth, auth_name);
            }
        }

        // Command: reset
        // Frees the memory associated with 'auth'.
        else if (strncmp(input_buffer, "reset", 5) == 0) {
            // --- VULNERABILITY 2: Use-After-Free (Part 1) ---
            // The memory 'auth' points to is freed, but the 'auth' pointer
            // itself is NOT set to NULL. It becomes a "dangling pointer".
            free(auth);
        }

        // Command: service <data>
        // Allocates memory for service data.
        else if (strncmp(input_buffer, "service", 7) == 0) {
            // --- VULNERABILITY 2: Use-After-Free (Part 2) ---
            // When strdup allocates new memory, the memory manager is likely
            // to reuse the chunk that was just freed by the 'reset' command.
            // This means 'service' and the dangling 'auth' pointer may now
            // point to the same memory location.
            service = strdup(input_buffer + 8);
        }

        // Command: login
        // Checks for authentication and grants a shell.
        else if (strncmp(input_buffer, "login", 5) == 0) {
            // --- VULNERABILITY 2: Use-After-Free (Part 3) ---
            // The program checks memory using the dangling 'auth' pointer.
            // If an attacker used the 'service' command to place specific data
            // at this location, they can pass this check.
            // The check looks 32 bytes (0x20) past the start of the buffer.
            if (auth && *(int *)(auth + 32) != 0) {
                // If the check passes, give the user a root shell.
                printf("Login successful!\n");
                system("/bin/sh");
            } else {
                printf("Password:\n");
            }
        }
    }

    return 0;
}
