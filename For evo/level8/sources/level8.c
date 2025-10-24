#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *auth = NULL;
char *service = NULL;

int main(void)
{
    unsigned char b_zero = 0;              /* corresponds to bVar14 = 0 */
    unsigned char input_buf[5];            /* local_90[5] in decompiled output */
    char auth_buf[2];                      /* local_8b[2] */
    char service_buf[125];                 /* acStack_89[125] */

    char ch;
    char *p1;
    char *p2;
    char *p3;
    int i;
    unsigned int u;
    unsigned char *pb1;
    unsigned char *pb2;
    bool less;
    bool equal;
    bool cond;

    do {
        printf("%p, %p \n", auth, service);

        if (fgets((char *)input_buf, 0x80, stdin) == NULL) {
            return 0;
        }

        /* ---------- Check if input starts with "auth " (5 bytes) ---------- */
        i = 5;
        pb1 = input_buf;
        pb2 = (unsigned char *)"auth ";
        less = false;
        equal = false;
        do {
            if (i == 0) break;
            i = i - 1;
            less = *pb1 < *pb2;
            equal = *pb1 == *pb2;
            /* original had pb1 = pb1 + (uint)bVar14 * -2 + 1; with bVar14==0 => +1 */
            pb1 = pb1 + 1;
            pb2 = pb2 + 1;
        } while (equal);

        /* If input starts with "auth " then allocate auth and copy auth_buf into it (with length check) */
        if (( !less && !equal ) == less) {
            /* allocate 4 bytes and zero them out (original did auth = malloc(4) and set 4 bytes '\0') */
            auth = (char *)malloc(4);
            if (auth == NULL) {
                /* keep behavior simple: on malloc failure, skip further auth handling */
            } else {
                /* set all 4 bytes to '\0' */
                auth[0] = '\0';
                auth[1] = '\0';
                auth[2] = '\0';
                auth[3] = '\0';
            }

            /* compute length of auth_buf (original used weird ~uVar6 - 1 trick) */
            u = 0xffffffff;
            p1 = auth_buf;
            do {
                if (u == 0) break;
                u = u - 1;
                ch = *p1;
                /* same increment logic: original used p1 = p1 + (uint)bVar14 * -2 + 1; which is +1 */
                p1 = p1 + 1;
            } while (ch != '\0');
            u = ~u - 1; /* length computed to match original decompiled steps */

            if (u < 0x1f) {
                /* strcpy(auth, local_8b) in original */
                if (auth != NULL) strcpy(auth, auth_buf);
            }
        }

        /* ---------- Check if input starts with "reset" (5 bytes) ---------- */
        i = 5;
        pb1 = input_buf;
        pb2 = (unsigned char *)"reset";
        do {
            if (i == 0) break;
            i = i - 1;
            less = *pb1 < *pb2;
            equal = *pb1 == *pb2;
            pb1 = pb1 + 1;  /* original expression reduces to +1 */
            pb2 = pb2 + 1;
        } while (equal);

        /* If input is "reset", free(auth) */
        if (( !less && !equal ) == less) {
            free(auth);
            auth = NULL;
        }

        /* ---------- Check if input starts with "service" (6 bytes) ---------- */
        i = 6;
        pb1 = input_buf;
        pb2 = (unsigned char *)"service";
        do {
            if (i == 0) break;
            i = i - 1;
            /* original assigned to uVar11/uVar10; here we keep semantics with less/equal */
            less = *pb1 < *pb2;
            equal = *pb1 == *pb2;
            pb1 = pb1 + 1;
            pb2 = pb2 + 1;
        } while (equal);

        /* If input is "service", strdup the current service_buf into global 'service' */
        /* original set an unused flag uVar13 = 0; then service = strdup(acStack_89); */
        if (( !less && !equal ) == less) {
            /* duplicate service_buf */
            if (service != NULL) {
                free(service);
            }
            service = strdup(service_buf); 
        }

        /* ---------- Check if input starts with "login" (5 bytes) ---------- */
        i = 5;
        pb1 = input_buf;
        pb2 = (unsigned char *)"login";
        do {
            if (i == 0) break;
            i = i - 1;
            less = *pb1 < *pb2;
            equal = *pb1 == *pb2;
            pb1 = pb1 + 1;
            pb2 = pb2 + 1;
        } while (equal);

        if (( !less && !equal ) == less) {
            /* Note: original does *(int *)(auth + 0x20) == 0 check.
               We replicate that check exactly; it may dereference invalid memory if auth is small/NULL,
               matching original binary behavior. */
            if (auth == NULL || *(int *)(auth + 0x20) == 0) {      //0x20 == 32 bytes
                fwrite("Password:\n", 1, 10, stdout);
            } else {
                system("/bin/sh");
            }
        }

    } while (1);

    return 0;
}
