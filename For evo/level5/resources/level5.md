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

### Payload Analysis

The payload used is as follows:

```sh
(python -c 'print "\x38\x98\x04\x08" + "%134513824d%4$n"'; cat -) | ./level5
```

This payload consists of two main parts:

1.  **Address (`\x38\x98\x04\x08`)**: This is the address of the target we want to overwrite. In this case, it is the address of the `exit` function in the GOT. We place this address at the beginning of the buffer to put it on the stack.

2.  **Format String (`%134513824d%4$n`)**:
    *   `%4$n`: This specifier tells `printf` to write the number of characters printed so far to the address pointed to by the 4th argument on the stack. Since our buffer corresponds to the 4th argument on the stack, the write operation will be performed at the address `\x38\x98\x04\x08`.
    *   `%134513824d`: This specifier is used to control the value that `%n` will write. It makes `printf` print `134513824` characters. When the 4 bytes of the address at the beginning are added to this number, the total number of characters printed becomes `134513828`.
    *   The hexadecimal representation of the number `134513828` is `0x8049824`, which is the memory address of the `o()` function.

### Result

When the payload is executed, the `printf` function writes the address of the `o()` function (`0x08049824`) to the `exit@got.plt` address (`0x08049838`). Subsequently, when the program calls `exit()`, the `o()` function is actually executed, and the attacker obtains a command-line shell.