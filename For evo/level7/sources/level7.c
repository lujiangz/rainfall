#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char c[0x44];  // 0x44 == 68 bytes

void m(void *param_1, int param_2, char *param_3, int param_4, int param_5) {
    time_t current_time = time(NULL);
    printf("%s - %ld\n", c, current_time);
    return;
}

int main(int argc, char **argv) {
    int *user1 = (int *)malloc(8);   // 2 integers: [ID, pointer to name]
    int *user2 = (int *)malloc(8);   // 2 integers: [ID, pointer to name]

    char *name1 = (char *)malloc(8);
    char *name2 = (char *)malloc(8);

    user1[0] = 1;           // user1 ID
    user1[1] = (int)name1;  // pointer to name1

    user2[0] = 2;           // user2 ID
    user2[1] = (int)name2;  // pointer to name2

    strcpy(name1, argv[1]);  // first input argument
    strcpy(name2, argv[2]);  // second input argument

    // Read the password file into global buffer c
    FILE *fp = fopen("/home/user/level8/.pass", "r");
    if (fp != NULL) {
        fgets(c, sizeof(c), fp);  // read at most 68 bytes into c
        fclose(fp);
    }

    puts("~~");

    return 0;
}
