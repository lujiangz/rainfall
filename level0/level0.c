#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
int input = atoi(argv[1]);
if (input == 0x1a7) 
	{
	char *arg = strdup("/bin/sh");
        char *args[] = { arg, NULL };
      	gid_t egid = getegid();
        uid_t euid = geteuid();	       
	setresgid(egid, egid, egid);
        setresuid(euid, euid, euid);
	execv(arg, args);
	} 
else
	fwrite("NO !\n", 7, 1, stdout);			

return 0;
}

