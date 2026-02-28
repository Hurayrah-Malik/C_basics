# Memory, Pointers & the Stack/Heap — Full Reference

---

## 1. How Memory is Laid Out

When your program runs, RAM is divided into regions:

```
HIGH ADDRESSES
┌─────────────────┐
│      STACK      │  ← local variables, function parameters
│       ↓         │  grows downward
│                 │
│                 │
│       ↑         │  grows upward
│      HEAP       │  ← malloc/calloc lives here
├─────────────────┤
│      CODE       │  ← your compiled program instructions
└─────────────────┘
LOW ADDRESSES
```

---

## 2. The Stack

- Holds **local variables** and **function parameters**
- Automatically managed — variables appear when a function is called, disappear when it returns
- Fast, but limited in size

```c
void foo() {
    int x = 5;    // x lives on the stack
    int y = 10;   // y lives on the stack
}                 // x and y are GONE — stack frame destroyed
```

Stack during foo():
```
┌──────────┐
│  y = 10  │
├──────────┤
│  x = 5   │
└──────────┘
```

After foo() returns:
```
┌──────────┐
│ (empty)  │
└──────────┘
```

---

## 3. The Heap

- Holds memory you allocate manually with `malloc` / `calloc`
- Persists until you call `free()`
- Larger than the stack, but slower
- You are responsible for cleaning it up

```c
int *p = malloc(sizeof(int));   // carve out 4 bytes on heap
*p = 42;                        // store 42 there
free(p);                        // give it back
```

```
STACK              HEAP
┌─────────┐        ┌──────┐
│ p=0x100 │ ──────►│  42  │
└─────────┘        └──────┘
```

The **pointer is on the stack**. The **data is on the heap**. Always.

---

## 4. Variables and Addresses

Every variable lives at a memory address. The variable name is just an alias for that address.

```c
int x = 42;
```

```
Address 0x100:  [ 42 ]
                   ↑
           this slot IS x
           x and 42 are not separate — x IS the memory slot containing 42
```

The compiler replaces `x` with `0x100` everywhere before your program runs.
You never see the address — unless you ask for it with `&`.

---

## 5. Pointers

A pointer is a variable that stores a memory address.

```c
int x = 42;        // x lives at 0x100, value is 42
int *p = &x;       // p lives at 0x200, value is 0x100 (address of x)
```

```
Address: 0x100    0x200
Value:   [ 42 ]   [0x100]
            x         p
```

| Expression | Meaning                          | Value    |
|------------|----------------------------------|----------|
| `x`        | value of x                       | 42       |
| `&x`       | address of x                     | 0x100    |
| `p`        | value of p (address it holds)    | 0x100    |
| `*p`       | follow p's address, get value    | 42       |
| `*p = 99`  | follow p's address, set value    | x is now 99 |

---

## 6. Declaring Pointers

The `*` in the declaration tells the compiler "this variable holds an address, and what it points to is this type":

```c
int *p        // pointer to int
char *p       // pointer to char
double *p     // pointer to double
struct CPU *p // pointer to CPU struct
```

Why does the type matter? Because when you do `*p`, the compiler needs to know how many bytes to read. An `int` is 4 bytes. A `char` is 1 byte. The type tells it.

---

## 7. Passing Pointers to Functions

```c
// WRONG — gets a copy, original unchanged
void set(int n) {
    n = 99;    // modifies copy only
}

// CORRECT — gets the address, modifies the real thing
void set(int *p) {
    *p = 99;   // follows address, modifies original
}

int main() {
    int x = 5;
    set(&x);           // pass address of x
    printf("%d\n", x); // prints 99
}
```

When you call `set(&x)`, C does this internally:
```c
int *p = &x;  // p is a copy of the address — both point to same x
```

---

## 8. Structs and Pointers

With a regular struct variable, use `.` to access fields:
```c
struct CPU cpu;
cpu.pc = 0;
```

With a pointer to a struct, use `->`:
```c
struct CPU *cpu = calloc(1, sizeof(struct CPU));
cpu->pc = 0;        // same as (*cpu).pc = 0
cpu->registers[0] = 5;
```

`->` is just shorthand for "dereference then access field."

---

## 9. malloc vs calloc

```c
// malloc — allocates memory, contents are garbage (random leftover bits)
int *p = malloc(sizeof(int));

// calloc — allocates AND zeros everything out
int *p = calloc(1, sizeof(int));
// args: (number of items, size of each item)
```

Use `calloc` when you need a clean slate.
Use `malloc` when you're about to fill everything yourself anyway (slightly faster).

Always `free()` what you allocate:
```c
free(p);
p = NULL;  // good habit — prevents accidentally using freed memory
```

---

## 10. Memory Leaks

A memory leak happens when you allocate memory and lose the pointer to it:

```c
int *p = malloc(sizeof(int));
p = NULL;   // address lost — that memory is stuck forever
            // this is the "lost parking ticket" — spot stays occupied
```

Small programs get away with it. Long-running programs (servers, emulators) will eventually run out of memory.

---

## 11. Pointer Arithmetic

Adding to a pointer moves it forward by multiples of the type size:

```c
int arr[3] = {10, 20, 30};
int *p = arr;   // p points to arr[0]
```

```
Address: 0x100   0x104   0x108
Value:  [ 10 ]  [ 20 ]  [ 30 ]
          p      p+1     p+2
```

- `p` → 0x100 → value 10
- `p + 1` → 0x104 → value 20  (moves by sizeof(int) = 4 bytes)
- `p + 2` → 0x108 → value 30
- `*(p + 1)` → 20

`p + 1` does NOT move by 1 byte. It moves by `sizeof(*p)` bytes.
A `char *` moves by 1. A `double *` moves by 8. The type determines the step.

**This is how array indexing works internally:**
```c
arr[1]      // same as *(arr + 1)
arr[2]      // same as *(arr + 2)
```

---

## 12. Pointers to Pointers

```c
int x = 10;
int *p = &x;    // p points to x
int **pp = &p;  // pp points to p
```

```
0x100      0x200      0x300
[ 10 ]  ←  [0x100] ←  [0x200]
  x            p          pp
```

- `pp`   → 0x200 (address of p)
- `*pp`  → 0x100 (value of p, which is address of x)
- `**pp` → 10    (value of x)
- `**pp = 99` → sets x to 99

---

## 13. Dangerous Mistakes

**Freeing stack memory** — only free what malloc/calloc gave you:
```c
int x = 5;
int *p = &x;
free(p);   // WRONG — x is on the stack, not the heap. crash.
```

**Use after free** — using a pointer after freeing it:
```c
int *p = malloc(sizeof(int));
free(p);
*p = 42;   // WRONG — that memory belongs to the OS now. undefined behavior.
```

**Going out of bounds** — C won't stop you:
```c
int arr[3] = {10, 20, 30};
int *p = arr;
*(p + 10);  // reads garbage memory way past the array — no error, just corruption
```
This is how buffer overflow exploits work — writing past the end of an array into adjacent memory.

---

## 14. Changing a Variable Through a Pointer

**Question that came up:** "If I change x after creating a pointer to it, does the pointer change?"

`p` itself doesn't change — it still holds the same address. But `*p` reflects the new value because p still points to the same slot.

```c
int x = 5;
int *p = &x;   // p holds address 0x100

x = 99;        // changes what's at 0x100

printf("%d\n", x);   // 99
printf("%d\n", *p);  // 99 — p still points to 0x100, value there is now 99
printf("%p\n", p);   // 0x100 — p itself didn't change at all
```

Think of `p` as a sticky note with a room number. Changing what's in the room doesn't change the sticky note. But when you walk to that room, you see the new stuff.

---

## 15. Dangling Pointer — Returning Address of a Local Variable

This looks safe but crashes or gives garbage:

```c
int *get_value() {
    int x = 5;      // x lives on the STACK inside this function
    return &x;      // returns address of x
}                   // function ends — x is GONE. stack frame destroyed.

int main() {
    int *p = get_value();
    printf("%d\n", *p);   // p points to dead memory — garbage or crash
}
```

`x` was on the stack. When `get_value()` returned, that stack frame was destroyed.
`p` still holds the address (say 0x1000), but 0x1000 is no longer valid memory.

**This is called a dangling pointer** — a pointer that holds an address to memory that no longer belongs to you.

**Common confusion:** "Is `&x` not really a pointer?" — `&x` IS a pointer (it's an address). The bug isn't the type — it's the *lifetime*. The fix is to use the heap:

```c
int *get_value() {
    int *x = malloc(sizeof(int));   // heap memory — survives function return
    *x = 5;
    return x;
}

int main() {
    int *p = get_value();
    printf("%d\n", *p);  // 5 — safe, heap memory is still alive
    free(p);
}
```

**Function return type `int *get_value()`:** The `*` is part of the return type. It means "this function returns a pointer to an int" (an address), not a plain int.

---

## 16. Pointer Arithmetic Does NOT Modify the Value

This is a common trap — moving a pointer is NOT the same as changing what it points to:

```c
void double_it(int *p) {
    p = p + 1;   // moves p forward 4 bytes — does NOT touch x's value
}

int main() {
    int x = 5;
    double_it(&x);
    printf("%d\n", x);   // still 5 — double_it did nothing to x
}
```

`p = p + 1` just changes where p points, locally inside the function. p is a copy of the address — moving the copy doesn't affect main at all.

To actually change x's value you need to dereference:
```c
void double_it(int *p) {
    *p = *p * 2;   // follow the address, double the value there
}
```

---

## 17. Double Pointers — Modifying a Pointer From Inside a Function

If a function needs to change **what a pointer points to** (not just the value it points at), you need a pointer to a pointer.

**The broken version:**
```c
void allocate(int *p) {
    p = malloc(sizeof(int));  // p is a LOCAL COPY — x in main never changes
    *p = 42;
}

int main() {
    int *x = NULL;
    allocate(x);       // passes a copy of NULL into the function
    printf("%d\n", *x);  // x is still NULL — CRASH
}
```

`p` inside the function is just a copy of x's value (NULL). Reassigning `p` doesn't affect `x` in main.

**The fix — pass the address of the pointer itself (`&x`):**
```c
void allocate(int **p) {    // p is now a pointer to x
    *p = malloc(sizeof(int));  // follow p to reach x, set x = heap address
    **p = 42;                  // follow p → x → heap block, write 42
}

int main() {
    int *x = NULL;
    allocate(&x);          // pass address of x, not x's value
    printf("%d\n", *x);   // prints 42
    free(x);
}
```

**Visual breakdown:**

```
Before allocate(&x):
  STACK
  x at 0x100 = NULL

Inside allocate — p is created:
  STACK
  x at 0x100 = NULL
  p at 0x200 = 0x100      ← p points TO x

After *p = malloc(...):
  STACK                    HEAP
  x at 0x100 = 0x300  ───► [ ??? ] at 0x300
  p at 0x200 = 0x100       ← p still points to x, but x now points to heap

After **p = 42:
  STACK                    HEAP
  x at 0x100 = 0x300  ───► [ 42 ] at 0x300

Back in main (p is gone, x was permanently updated):
  STACK                    HEAP
  x at 0x100 = 0x300  ───► [ 42 ]
```

**Simpler alternative — just return the pointer:**
```c
int *allocate() {
    int *p = malloc(sizeof(int));
    *p = 42;
    return p;     // hand the pointer back directly — cleaner
}

int main() {
    int *x = allocate();
    printf("%d\n", *x);  // 42
    free(x);
}
```

**Is `**` common in C?** Yes, in specific places:
- `int main(int argc, char **argv)` — argv is an array of strings, each string is a `char *`
- Functions that allocate and return pointers (when not using return value)
- Linked lists where you need to modify the head pointer from a function

---

## 18. Quick Reference

```
int x = 5          x is at some address, value is 5
&x                 address of x
int *p = &x        p stores the address of x
p                  the address (same as &x)
*p                 value at that address (same as x)
*p = 99            set value at that address (x becomes 99)
p + 1              next int-sized slot in memory
*(p + 1)           value at next slot
int **pp = &p      pointer to a pointer
**pp               two dereferences — gets the original value
p->field           access struct field through pointer (same as (*p).field)
malloc(n)          allocate n bytes on heap, return address (garbage contents)
calloc(1, n)       allocate n bytes on heap, zeroed out
free(p)            release heap memory back to OS
```
