# CTF Solution: Heap-Based Buffer Overflow

## Root of the Vulnerability: strcpy and Heap Memory Management

The primary security vulnerability in the level6 binary was a heap-based buffer overflow, triggered by the use of the unsafe strcpy function in C.

### Main Function Behavior

#### Two malloc Calls

1. malloc(0x40)  
   Allocated a 64-byte memory region on the heap. This served as the main buffer for user input.

2. malloc(0x4)  
   Allocated a 4-byte memory region. This memory held a function pointer, initially pointing to the m function.

#### Use of strcpy

The program used strcpy to copy the command-line argument into the 64-byte buffer:

    strcpy(buffer, argv[1]);

### Why This Causes a Vulnerability

The strcpy function does not perform bounds checking. If the input string is longer than 64 bytes, it overflows into adjacent memory and overwrites the function pointer.

### Heap Memory Layout

Two malloc calls often return adjacent memory blocks. In this case:

    [64-byte buffer][8-byte padding][4-byte function pointer]

Overflowing 72 bytes total allows us to overwrite the function pointer.

---

## The Exploit: Overwriting the Function Pointer

### Objective

Redirect execution from m() to n() by overwriting the function pointer.

### Step 1: Determine Offset

Through testing:

- 64 bytes → safe
- 72 bytes → segfault

So:

    Offset = 64 (buffer) + 8 (padding) = 72 bytes

### Step 2: Identify the Target Function Address

n function address:

    0x08048454

Little-endian representation:

    \x54\x84\x04\x08

### Step 3: Construct the Payload

    "A" * 72 + "\x54\x84\x04\x08"

---

## Exploit Command

    ./level6 $(python -c 'print "A" * 72 + "\x54\x84\x04\x08"')

This command sends a 76-byte payload as a command-line argument. The overflow replaces the function pointer with the address of n(). As a result, the program calls n().
