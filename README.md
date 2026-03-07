# Virtual CPU Emulator in C

A simple virtual CPU emulator written in C. This is a rewrite of an earlier Python project, built to learn systems programming concepts hands-on.

## What it does

- Emulates a CPU with 8 registers and 256 words of memory
- Supports 4 instructions: `LOAD`, `ADD`, `PRINT`, `HALT`
- Instructions are packed into 32-bit integers using bitwise encoding
- Programs are written to and loaded from a binary file (`program.bin`)
- Uses a dispatch table (array of function pointers) instead of a switch statement

## Instruction format

Each instruction is a 32-bit integer packed as:

```
bits 31-24: opcode
bits 23-16: operand 1 (destination register)
bits 15-8:  operand 2 (value or source register 1)
bits  7-0:  operand 3 (source register 2, ADD only)
```

## C concepts practiced

- Structs and struct pointers
- Heap allocation with `malloc`/`calloc`/`free`
- Function pointers and dispatch tables
- Bitwise operations (encoding and decoding)
- File I/O with `fopen`, `fwrite`, `fread`, `fclose`

## Build and run

```bash
gcc main.c -o main
./main
```

## Sample output

```
R2 = 15
=== REGISTERS ===
R0: 5
R1: 10
R2: 15
...
PC: 5
```
