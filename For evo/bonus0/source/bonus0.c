#include <stdio.h>
#include <string.h>
#include <unistd.h>

const char *prompt = " - ";

void p(char *dst, const char *msg);
void pp(char *dst);

int main(void) {
    char buffer[54];

    pp(buffer);
    puts(buffer);

    return 0;
}

void p(char *dst, const char *msg) {
    char input[4104];
    char *newline;

    puts(msg);
    read(0, input, 0x1000);

    newline = strchr(input, '\n');
    *newline = '\0';

    strncpy(dst, input, 0x14);
}

void pp(char *dst) {
    char part1[20];
    char part2[20];
    char *ptr;
    char c;
    unsigned int len;

    p(part1, prompt);
    p(part2, prompt);

    strcpy(dst, part1);

    len = 0xffffffff;
    ptr = dst;
    do {
        if (len == 0) break;
        len--;
        c = *ptr;
        ptr++;
    } while (c != '\0');

    dst[(~len - 1)] = ' ';
    dst[(~len - 1) + 1] = '\0';

    strcat(dst, part2);
}
