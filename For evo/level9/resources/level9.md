
```diff
+level9@RainFall:~$ gdb ./level9
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/user/level9/level9...(no debugging symbols found)...done.
+(gdb) b main
Breakpoint 1 at 0x80485f8
(gdb) r
Starting program: /home/user/level9/level9

Breakpoint 1, 0x080485f8 in main ()
+(gdb) info proc map
process 3729
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
         0x8048000  0x8049000     0x1000        0x0 /home/user/level9/level9
         0x8049000  0x804a000     0x1000        0x0 /home/user/level9/level9
        0xb7cfa000 0xb7cfc000     0x2000        0x0
        0xb7cfc000 0xb7d18000    0x1c000        0x0 /lib/i386-linux-gnu/libgcc_s.so.1
        0xb7d18000 0xb7d19000     0x1000    0x1b000 /lib/i386-linux-gnu/libgcc_s.so.1
        0xb7d19000 0xb7d1a000     0x1000    0x1c000 /lib/i386-linux-gnu/libgcc_s.so.1
        0xb7d1a000 0xb7d44000    0x2a000        0x0 /lib/i386-linux-gnu/libm-2.15.so
        0xb7d44000 0xb7d45000     0x1000    0x29000 /lib/i386-linux-gnu/libm-2.15.so
        0xb7d45000 0xb7d46000     0x1000    0x2a000 /lib/i386-linux-gnu/libm-2.15.so
        0xb7d46000 0xb7d47000     0x1000        0x0
        0xb7d47000 0xb7eea000   0x1a3000        0x0 /lib/i386-linux-gnu/libc-2.15.so
        0xb7eea000 0xb7eec000     0x2000   0x1a3000 /lib/i386-linux-gnu/libc-2.15.so
        0xb7eec000 0xb7eed000     0x1000   0x1a5000 /lib/i386-linux-gnu/libc-2.15.so
        0xb7eed000 0xb7ef0000     0x3000        0x0
        0xb7ef0000 0xb7fc8000    0xd8000        0x0 /usr/lib/i386-linux-gnu/libstdc++.so.6.0.16
        0xb7fc8000 0xb7fc9000     0x1000    0xd8000 /usr/lib/i386-linux-gnu/libstdc++.so.6.0.16
        0xb7fc9000 0xb7fcd000     0x4000    0xd8000 /usr/lib/i386-linux-gnu/libstdc++.so.6.0.16
        0xb7fcd000 0xb7fce000     0x1000    0xdc000 /usr/lib/i386-linux-gnu/libstdc++.so.6.0.16
        0xb7fce000 0xb7fd5000     0x7000        0x0
        0xb7fdb000 0xb7fdd000     0x2000        0x0
        0xb7fdd000 0xb7fde000     0x1000        0x0 [vdso]
        0xb7fde000 0xb7ffe000    0x20000        0x0 /lib/i386-linux-gnu/ld-2.15.so
        0xb7ffe000 0xb7fff000     0x1000    0x1f000 /lib/i386-linux-gnu/ld-2.15.so
        0xb7fff000 0xb8000000     0x1000    0x20000 /lib/i386-linux-gnu/ld-2.15.so
        0xbffdf000 0xc0000000    0x21000        0x0 [stack]
+(gdb) find 0xb7d47000,0xb7eed000, "/bin/sh"
-0xb7ea7c58
1 pattern found.
```

Or you can use this directly;

```diff
+level9@RainFall:~$ gdb ./level9
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/user/level9/level9...(no debugging symbols found)...done.
+(gdb) b main
Breakpoint 1 at 0x80485f8
+(gdb) r
Starting program: /home/user/level9/level9

Breakpoint 1, 0x080485f8 in main ()
+(gdb) find &system, &system+2000000,"/bin/sh"
-0xb7ea7c58
1 pattern found.
(gdb)
```


## /bin/sh
```
address         =   0xb7ea7c58
little indian   =   "\x58\x7c\xea\xb7"
```

> https://ctf101.org/binary-exploitation/return-oriented-programming/

