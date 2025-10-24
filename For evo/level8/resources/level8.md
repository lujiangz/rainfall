## Vulnerabilities

### Vulnerability 1: Stack Buffer Overflow
**Location:** Main function

**Code Snippet:**
```c
unsigned char input_buf[5];
...
if (fgets((char *)input_buf, 0x80, stdin) == NULL) { // 0x80 = 128 bytes
```

**Description:**  
The program allocates a small 5-byte buffer (`input_buf`) on the stack for user input. However, the `fgets` function is configured to read up to 128 bytes from stdin into this buffer. This mismatch allows an attacker to overflow the buffer, overwriting adjacent stack variables such as `auth_buf` and `service_buf`. By crafting input that exceeds 5 bytes, we can control the contents of these variables, setting the stage for further exploitation.

### Vulnerability 2: Heap Buffer Overflow
**Location:** "auth " command handler

**Code Snippet:**
```c
auth = (char *)malloc(4); // Allocate only 4 bytes
...
if (u < 0x1f) {
    if (auth != NULL) strcpy(auth, auth_buf); // Copy auth_buf into auth
}
```

**Description:**  
When the program processes an "auth " command, it allocates a mere 4 bytes on the heap for the `auth` pointer. It then copies the contents of `auth_buf` (which we can control via Vulnerability 1) into this small heap buffer using `strcpy`. Since `strcpy` doesn't perform bounds checking, if `auth_buf` contains more than 4 bytes, it will overflow the allocated heap space, allowing us to write beyond the intended boundaries.

## Target: Login Command
**Location:** "login" command handler

**Code Snippet:**
```c
if (auth == NULL || *(int *)(auth + 0x20) == 0) {
    fwrite("Password:\n", 1, 10, stdout);
} else {
    system("/bin/sh"); // Target: Gain shell access
}
```

**Description:**  
The goal is to execute `system("/bin/sh")` to spawn a shell. This happens only if `auth` is not NULL and the 4-byte integer at offset 0x20 (32 bytes) from the `auth` pointer is non-zero. Normally, this check would fail, prompting for a password. By exploiting the heap overflow, we can overwrite this critical memory location.

## Exploit Strategy
To successfully exploit this challenge, we need to chain the two vulnerabilities:

1. **Trigger Stack Overflow:** Send input starting with "auth " that overflows `input_buf` and overwrites `auth_buf` with controlled data.
2. **Trigger Heap Overflow:** The "auth " command allocates 4 bytes for `auth` and copies `auth_buf` into it, overflowing the heap.
3. **Bypass Login Check:** Ensure the overflow reaches `auth + 0x20` and sets it to a non-zero value.
4. **Execute Login:** Send "login" to trigger the shell spawn.

The minimum data needed is 36 bytes after "auth " to reach `auth + 0x20` (4 bytes allocated + 32 bytes offset).

## Payload Construction

### Payload 1: Setup Overflow
```
"auth " + "A" * 36 + "\n"
```
- `"auth "`: 5 bytes to trigger the auth command.
- `"A" * 36`: 36 'A' characters (0x41 in ASCII) to fill `input_buf` (5 bytes) and overflow into `auth_buf`. This sets up the heap overflow payload.

After sending this, the program processes "auth ", allocates 4 bytes for `auth`, and copies the 36 'A's into it, overflowing the heap.

### Payload 2: Trigger Shell
```
"login\n"
```
- Sends the "login" command.
- The program checks `*(int *)(auth + 0x20)`, which is now `0x41414141` (four 'A's), a non-zero value.
- Since the condition fails, it enters the else block and executes `system("/bin/sh")`.

## Example Execution
```
auth AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
service AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
login
```
