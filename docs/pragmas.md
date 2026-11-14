# Pragmas

c6502 recognizes two `#pragma` directives. Each one goes on its own
line, just before the code it applies to, and each starts with
`#pragma c6502`:

| Pragma | Goes before | What it does |
|---|---|---|
| `#pragma c6502 loop unroll(enable)` | a `for` loop | Unrolls the loop completely. |
| `#pragma c6502 call save(hardware_stack)` | a statement or declaration in a function | Lets a recursive call save its caller's locals on the 6502's hardware stack. |

Both take effect only under `--optimize`. Without it, the code
compiles as if the pragma weren't there, though a misplaced `call
save` pragma is still an error.

A `#pragma c6502` line the compiler doesn't recognize is an error, so
a misspelled pragma doesn't go unnoticed. Any other `#pragma`, such as
`#pragma once` or one meant for another compiler, is ignored, as C99
§6.10.6 requires.

## `#pragma c6502 loop unroll(enable)`

```c
uint8_t row[4];

void fill(uint8_t v) {
#pragma c6502 loop unroll(enable)
    for (uint8_t i = 0; i < 4; i++)
        row[i] = v + i;
}
```

The loop is replaced by one copy of its body per iteration, with `i`
replaced by that iteration's value in each copy. The copies need no
compare, branch, or increment, and a constant index lets `row[i]`
use the element's address directly. The cost is code size, which
grows with the number of iterations.

The loop has to have a simple shape the compiler can count:

- The `for` declares one integer variable, initialized to a
  constant.
- The condition compares that variable with a constant using `<`,
  `<=`, `>`, or `>=`.
- The step is `++`, `--`, `+= K`, or `-= K`, with `K` a positive
  constant.
- The body has no `break`, `continue`, `goto`, or label, and doesn't
  change the variable or take its address.
- The loop ends within 256 iterations.

A marked loop that doesn't fit this shape is a compile error, since
the pragma asks for the loop to be unrolled. `#pragma c6502 loop
unroll(disable)` is accepted and does nothing. [Loop
unrolling](optimizations/unroll.md) has the full rules, including
which constants count.

## `#pragma c6502 call save(hardware_stack)`

This pragma is for recursive functions. Use it only after reading
the cautions below.

### What it does

A function that calls itself, directly or through other functions,
normally keeps its locals in zero-page bytes that it shares with
other functions on the same terms as everyone else: it saves the
ones it uses when it starts and restores them when it returns. It
does that on every call, including the calls that don't recurse,
and it needs a soft-stack frame to save them into.

The pragma marks calls that recurse. When every call that can recurse
is marked, the function instead gets zero-page bytes of its own, as
a non-recursive function does. Around each marked call, the caller
pushes each local it still needs onto the hardware stack, and pulls
them back after the call:

```c
void print_zstring(void) {
    ...
            uint8_t locked = LOCKED_ALPHABET;
            uint8_t state = ZDECOMPRESS_STATE;
            load_packed_address();
#pragma c6502 call save(hardware_stack)
            print_zstring();
            ZDECOMPRESS_STATE = state;
            LOCKED_ALPHABET = locked;
    ...
}
```

compiles to

```
   LDA   LOCKED_ALPHABET
   PHA
   LDA   ZDECOMPRESS_STATE
   PHA
   JSR   load_packed_address
   JSR   print_zstring
   PLA
   STA   ZDECOMPRESS_STATE
   PLA
   STA   LOCKED_ALPHABET
```

and the function no longer saves anything when it starts or ends. In
`examples/zork/print_zstring/zstring.c` this makes `print_zstring` 180
bytes smaller, and faster on every call.

### Where it goes

The pragma goes inside a function, just before one of these:

- an expression statement, such as `f(x);` or `y = f(x) + 1;`,
- a `return` statement, such as `return a + f(x);`,
- a declaration, such as `int r = f(x);`.

It marks every call in that statement or declaration, and each of
them has to be a direct call that can recurse, that is, a call to a
function that can call the caller back. A call to any other function,
a call through a function pointer, or a statement with no call at
all is an error. Before an `if`, a loop, or a block, the pragma is
also an error, because it would cover the calls in the statements
nested inside.

The pragma takes effect only when every call that can recurse is
marked. If a function `f` calls `g` and `g` calls `f`, both calls
need it. If one is unmarked, the functions compile as they would
without the pragma. The pragma also has no effect on a function that
couldn't get its own zero-page bytes anyway, such as one that calls
through a function pointer.

Parameters, and locals whose address is taken, stay on the soft
stack, where each call has its own copy. They aren't pushed.

The calls have to be in one source file. `compile.py --link` accepts
such a cycle, which the link metadata marks call by call, and reports
any other recursion as an error. examples/zork/print_zstring links
`print_zstring` this way.

### Cautions

**The compiler can't check that the hardware stack is big enough.**
It has no way to know how deep your recursion goes, so the pragma is
your promise that it's shallow enough. That's why it isn't on by
default.

The 6502's hardware stack is a single 256-byte page, `$0100`-`$01FF`.
Every call in progress already keeps a 2-byte return address there.
With the pragma, every call in progress also keeps the bytes it
pushed until the call returns. A recursive call that saves 7 bytes
uses 9 bytes of hardware stack per level, so 20 levels take 180
bytes. The rest of the program needs room too:

- the return addresses of the calls that led to the recursion,
- the runtime helpers and the soft-stack epilogue, which push a few
  bytes for a moment,
- an interrupt handler, which can run at any point and pushes at
  least 3 bytes, plus whatever it pushes itself,
- ROM routines you call, such as the Monitor's, which may use the
  stack too.

**Running out of hardware stack fails silently.** The stack pointer
wraps around within page 1, and the next push overwrites the oldest
bytes on the stack, usually the return addresses of the outermost
calls. Nothing reports it. The program returns to the wrong place,
often much later, and the crash points nowhere near its cause.

**You may push more than you expect.** Every local that's still
needed after the call is pushed, along with any value the compiler
keeps in a temporary across the call, and a `long` local takes 4
bytes. To see the real number, compile with `--codegen --optimize`
and count the `PHA`s before the marked `JSR`.

Use the pragma when you know the recursion is shallow.

Without the pragma, recursion keeps everything but return addresses
on the soft stack, which can grow much larger than 256 bytes. That's
slower, but it doesn't depend on how deep the recursion goes. [The
calling conventions guide](calling_conventions.md#limits) covers the
limits of both stacks.

[Saves around a recursive call](optimizations/recursion_saves.md)
describes how the compiler decides what to save, for readers who
want the details.
