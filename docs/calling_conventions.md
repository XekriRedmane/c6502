# Calling conventions

A *calling convention* is the agreement between a function and the
code that calls it about where the arguments go, where the function
keeps its locals, and where the result comes back. c6502 has two.

- **The soft stack.** Arguments and locals go on a stack in main
  memory that the compiler manages itself. Any function can use it,
  including a recursive one, but every access costs a few cycles.
- **zp_abi.** Arguments and locals go in zero-page bytes fixed for
  each function, and the function has no frame at all. It's faster
  and smaller, but only a function that meets some conditions can
  use it.

## Which convention a function gets

| Compiled | Function | Convention |
|---|---|---|
| without `--optimize` | any | soft stack |
| with `--optimize` | qualifies for zp_abi (see "Which functions qualify") | zp_abi |
| with `--optimize` | doesn't qualify | soft stack |
| either way | declared with `__attribute__((address(...)))` | what its declaration says |

With `__attribute__((zp_abi))` on a function, the compiler reports an
error when the function doesn't qualify, instead of giving it the
soft stack. A routine at a fixed address, such as a ROM routine,
takes its convention from its declaration alone (see
[attributes.md](attributes.md)).

## Return values

Results come back the same way under both conventions.

| Result | Where |
|---|---|
| 1 byte (`char`, `bool`) | A |
| 2 bytes (`int`, `unsigned int`) | low byte in A, high byte in X |
| a pointer, or anything wider than 2 bytes | the zero-page block `HARGS` |
| `bool` from a `return_bool_in_carry` function | the carry flag |
| a struct of a value and a `bool` from a `return_bool_in_carry` function | the value in A (and X), the `bool` in the carry flag |
| nothing (`void`) | nothing |

A result in `HARGS` has to be picked up right after the call, before
anything else uses `HARGS`.

## The soft stack

A C program keeps each function's arguments and local variables
somewhere, and a recursive function has a separate copy of them for
each call that's in progress. The 6502's own stack is a poor fit for
that. It's a single 256-byte page at `$0100`-`$01FF`, and the only
ways to reach into it are `PHA` / `PLA` and the stack-pointer
transfers, so there's no way to read "the third byte of my frame"
directly.

So c6502 keeps a second stack in main memory, which it manages
itself, called the *soft stack* here. The 6502's stack still holds
what it's good at, which is return addresses (`JSR` / `RTS`) and
values saved for a few instructions (`PHA` / `PLA`). Arguments,
locals, and the bookkeeping that links one call's storage to the
next all go on the soft stack.

| | Hardware stack | Soft stack |
|---|---|---|
| Where | `$0100`-`$01FF` | main memory, below an address the startup code picks |
| Holds | return addresses, short-lived `PHA` saves | arguments, locals, saved frame pointers |
| Pointer | the 6502's S register | `SSP`, two bytes in zero page |
| Reached with | `PHA`, `PLA`, `JSR`, `RTS` | `LDY #offset` then `LDA (SSP),Y` or `LDA (FP),Y` |

### Two pointers, SSP and FP

`SSP`, the soft stack pointer, is a 2-byte pointer in zero page. The
soft stack grows downward, toward lower addresses, the same way the
hardware stack does. `SSP` always points at the next *free* byte, so
the byte pushed most recently is at `SSP+1`, the one before it at
`SSP+2`, and so on.

`SSP` moves whenever something is pushed, for instance when a
function sets up the arguments for a call it's about to make. If a
function found its locals relative to `SSP`, their offsets would
change in the middle of the function. So each function that has a
frame also sets `FP`, the frame pointer, once at the start, and finds
everything relative to `FP`, which doesn't move until the function
returns.

Both are reached the 6502's indirect-indexed way. To read the byte
at `FP+5`, the code loads 5 into Y and uses `(FP),Y`.

```
   LDY   #$05
   LDA   (FP),Y
```

That's why the Y register is scratch in compiled code. Almost any
access to an argument or a local changes it.

The startup code sets `SSP` before it calls `main`. The simulator's
startup code sets it to `$7FFF`. `FP` has no starting value to set,
because `main`'s own prologue saves whatever is in it and restores it
on the way out. `SSP` and `FP` normally sit at `$00` and `$02`, but
they can move when a program reserves those bytes (see
[attributes.md](attributes.md), "Sharing zero page with the
Monitor"). The output says where they are, in its `SSP EQU` and
`FP EQU` lines.

### A call, step by step

Here is a small program, compiled without `--optimize` so that every
function uses the soft stack.

```c
int add3(int a, int b) {
    int t = a + b;
    return t + 3;
}

int main(void) {
    return add3(10, 20);
}
```

`add3` has 4 bytes of arguments, two `int`s. It also has 6 bytes of
locals, which are `t` plus two temporaries the unoptimized compiler
makes for the intermediate results.

**1. The caller makes room for the arguments and writes them.** It
subtracts 4 from `SSP`, then writes `a` at `SSP+1` and `SSP+2`, low
byte first, and `b` at `SSP+3` and `SSP+4`.

```
   SEC                  ; SSP -= 4
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$0A           ; a = 10
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$14           ; b = 20
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   add3
```

`JSR` pushes the return address on the hardware stack. The soft
stack doesn't change.

**2. The callee's prologue builds its frame.** It subtracts 8 from
`SSP`, which is 6 bytes for its locals and 2 for a copy of the
caller's `FP`. It saves the caller's `FP` in those 2 bytes, then sets
`FP` to `SSP`.

```
   ; prologue: 4 arg bytes, 6 local bytes
   SEC                  ; SSP -= 6 + 2
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP             ; save the caller's FP at SSP+7, SSP+8
   LDY   #$07
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP            ; FP = SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1
```

**3. The body reaches everything through `FP`.** The frame now looks
like this, with higher addresses at the top.

```
            +---------------------------+
   FP+12    | b, high byte              |  \
   FP+11    | b, low byte               |   |  arguments, written
   FP+10    | a, high byte              |   |  by the caller
   FP+9     | a, low byte               |  /
            +---------------------------+
   FP+8     | caller's FP, high byte    |  \  saved by the
   FP+7     | caller's FP, low byte     |  /  prologue
            +---------------------------+
   FP+6     |                           |  \
     ...    | t and two temporaries     |   |  locals
   FP+1     |                           |  /
            +---------------------------+
   FP       | next free byte            |  <- FP, and SSP
            +---------------------------+
```

So `a + b` starts like this.

```
   LDY   #$09
   LDA   (FP),Y         ; a, low byte
   CLC
   LDY   #$0B
   ADC   (FP),Y         ; + b, low byte
```

In general, for a function with N bytes of arguments and M bytes of
locals, the locals are at `FP+1` through `FP+M`, the saved `FP` at
`FP+M+1` and `FP+M+2`, and argument byte j at `FP+M+3+j`.

**4. The epilogue takes the frame down.** The result is an `int`, so
it's in A (low byte) and X (high byte). The epilogue saves both on
the hardware stack, because restoring the pointers uses A and X.
It sets `SSP` to `FP + 12`, which is past the locals, the saved `FP`,
and the arguments, all at once. Then it reloads the caller's `FP`
from the frame, and restores the result.

```
   ; epilogue
   PHA                  ; save the result
   TXA
   PHA
   CLC                  ; SSP = FP + 6 + 4 + 2
   LDA   FP
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$07           ; FP = the caller's FP
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA                  ; restore the result
   TAX
   PLA
   RTS
```

The low byte of the caller's `FP` waits in X while the high byte is
read, because `FP` itself has to keep pointing at this frame until
both bytes are read. A result in the carry flag is saved with `PHP` /
`PLP` instead, and a result in `HARGS` isn't saved at all, since the
teardown doesn't touch `HARGS`.

After `RTS`, `SSP` and `FP` are back where they were before the
caller pushed the arguments. The callee removed the arguments, so the
caller has nothing to clean up.

A function with no arguments and no locals has no prologue, and its
epilogue is just `RTS`.

### Taking the address of a local

A local whose address is taken lives in the frame, and its address is
computed when the program runs, as `FP` plus its offset.

```
   CLC                  ; &x, where x is at FP+2
   LDA   FP
   ADC   #$02
   ...
   LDA   FP+1
   ADC   #$00
```

The address stays valid until the function returns, since `FP`
doesn't move until then.

### The soft stack with `--optimize`

A function that doesn't qualify for zp_abi still keeps most of its
working values in zero page. The compiler's pool is split in two.
`$80`-`$BF` is *caller-saved*, so a value there has to be saved by a
function that reads it again after a call, and `$C0`-`$FF` is
*callee-saved*, so a function that uses one of those bytes saves the
caller's value in its prologue and restores it at the end. The frame
holds the arguments, the saved bytes, and anything whose address is
taken. In this recursive function, `n` is at `FP+3`.

```c
unsigned int fact(unsigned char n) {
    if (n <= 1) return 1;
    return n * fact(n - 1);
}
```

## The zp_abi convention

With `--optimize`, a function that qualifies gets its arguments in
zero-page bytes set aside for it, keeps its locals in zero page too,
and has no prologue or epilogue. It's a plain body ending in `RTS`.

### Arguments in fixed zero-page bytes

```c
unsigned char inner(unsigned char x) { return x * 2; }
unsigned char other(unsigned char y) { return y + 7; }

unsigned char outer(unsigned char a, unsigned char b) {
    unsigned char t = inner(a);
    return t + other(b) + a;
}

int main(void) { return outer(3, 4); }
```

Each parameter gets a zero-page byte, named
`__zpabi_<function>__<parameter>`, and the output binds each name
with an `EQU` line. A declaration that leaves a parameter unnamed, as
`void put(unsigned char);` does, names its byte by the parameter's
position instead, `__zpabi_put__0`.

```
__zpabi_outer__a	EQU	$80
__zpabi_outer__b	EQU	$81
__zpabi_inner__x	EQU	$82
__zpabi_other__y	EQU	$82
__local_outer__0	EQU	$84
__local_outer__1	EQU	$85
```

The caller stores each argument in the callee's byte and calls it,
and the callee reads its parameters from those bytes.

```
main:
   LDA   #$03
   STA   __zpabi_outer__a
   LDA   #$04
   STA   __zpabi_outer__b
   JSR   outer
   ...

inner:
   LDA   __zpabi_inner__x
   ASL   A
   RTS
```

A multi-byte parameter gets one byte per byte of its type, with
`_0`, `_1`, and so on after its name, low byte first.

### Why two functions can share a byte

`inner` and `other` both use `$82`. That's safe because they're never
running at the same time. `outer` calls `inner`, which returns, and
only then calls `other`. Neither calls the other.

`outer`'s bytes are a different matter. `outer` is still running
while `inner` and `other` run, and it reads `a` again after `inner`
returns, so `$80` and `$81` are kept apart from theirs. The compiler
computes this from the *call graph*, the record of which function
calls which. Two functions get different bytes when one can be
running while the other is, that is, when one calls the other,
directly or through other functions. Otherwise they may share.

That's also why a recursive function can't use zp_abi. Its own
earlier calls are still running, and every one of them would read
the same bytes.

### Recursion that saves on the hardware stack

A recursive function's locals get zero-page bytes of their own under
`--optimize` when you mark the calls that recurse:

```c
void print_zstring(void) {
    ...
            load_packed_address();
#pragma c6502 call save(hardware_stack)
            print_zstring();
    ...
}
```

The pragma applies to the calls in the statement or declaration
right after it, each of which has to be a direct call that can
recurse. Before such
a call, the caller pushes each local that's live across it with `LDA;
PHA`, and after the call it pulls them back with `PLA; STA`. Because
the caller saves what it needs, the function doesn't save anything
at entry or exit, and it has no frame unless it has parameters or
locals whose address is taken. Those stay on the soft stack, one
copy per call.

It applies only when every call from one function on the cycle to
another is marked. The compiler can't tell how deep the recursion
goes, so the pragma is your promise that the hardware stack has room.
Read the cautions in [pragmas.md](pragmas.md) before using it, and see
[saves around a recursive call](optimizations/recursion_saves.md) for
the details.

### Arguments in registers

A parameter declared `register` arrives in a register instead. The
first `register` byte goes in A and the second in X.

```c
unsigned char add(register unsigned char a, register unsigned char b) {
    return a + b;
}

unsigned char twice(unsigned char v) { return add(v, v); }
```

```
add:
   STX   __zpabi_add__b
   CLC
   ADC   __zpabi_add__b
   RTS

twice:
   LDX   __zpabi_twice__v
   TXA
   JMP   add
```

`add` uses `a` straight from A. The 6502 can't add X to A, so the
first thing `add` does is store `b` in its zero-page byte. `twice`
loads both arguments and jumps to `add`, since nothing is left to do
after the call.

A `register` parameter can also name the register it arrives in, with
`__attribute__((in_a))`, `in_x`, or `in_y` after its name, or for a
2-byte parameter a pair such as `in_xy`, low byte first. That's the
only way to pass an argument in Y, so it's how to declare a ROM or DOS
routine that takes one there. A `register` parameter without the
attribute still goes in A, then X, skipping a register another
parameter names, so a function can take up to three bytes in
registers.
"[`in_a`, `in_x`, and `in_y`](attributes.md#in_a-in_x-and-in_y)" in
attributes.md lists the pairs and shows a call to DOS's RWTS, and
"[What the compiler rejects](attributes.md#what-the-compiler-rejects)"
lists the mistakes it reports.

```c
unsigned char out[2];
unsigned char g;

void put(register unsigned char hi __attribute__((in_a)),
         register unsigned char lo __attribute__((in_y))) {
    out[0] = hi;
    out[1] = lo;
}

void show(void) { put(0xC1, g); }
```

```
put:
   STA   out
   STY   out+1
   RTS

show:
   LDY   g
   LDA   #$C1
   JMP   put
```

The caller loads X first, then Y, then A, because loading X or Y goes
through A (`LDA v; TAX`, until a peephole makes it `LDX v`). Without
`--optimize`, an argument that's on the soft stack is loaded with
`LDY #off; LDA (FP),Y`, which would destroy an argument already in Y.
So when a call passes an argument in Y, the A argument is loaded first
and kept on the hardware stack with `PHA` until X and Y are loaded,
and `PLA` puts it back in A. When the X and Y arguments are constants
or bytes at a fixed address, they're loaded with `LDX` / `LDY`
directly, nothing in between touches A, and a peephole drops the
`PHA` / `PLA`.

A pointer can't be a `register` parameter. The 6502 reaches memory
through a pointer only with `(zp),Y` addressing, which reads the
pointer's two bytes from consecutive zero-page addresses, so a
pointer is only usable once it's in zero page. One that arrived in A
and X would have to be stored to zero page before its first use. A
pointer parameter's own zero-page bytes already serve as that base,
with no extra step.

```c
unsigned char first(const unsigned char *p) { return *p; }
```

```
first:
   LDY   #$00
   LDA   (__zpabi_first__p_0),Y
   RTS
```

The caller stores the pointer's two bytes in `__zpabi_first__p_0` and
`__zpabi_first__p_1`, and `first` reads through them directly.

### Locals

A zp_abi function's locals get zero-page bytes by the same call-graph
rule, so a function never shares a byte with one that can be running
at the same time. Their names start with `__local_<function>__`.
`outer`'s `t` and its other temporary are at `$84` and `$85`, clear
of every parameter byte that `inner` and `other` use. A local whose
address is taken gets a zero-page byte too when one is free in the
function's range, and `&x` is then a constant. Otherwise it goes in a
frame on the soft stack.

### Which functions qualify

A function uses zp_abi with `--optimize` unless one of these is true.

- It can call itself, directly or through other functions.
- Its address is taken, or it calls through a function pointer. The
  function a call through a pointer reaches isn't fixed when the
  program is compiled, so such a call always passes its arguments on
  the soft stack.
- Its arguments don't fit the 64 bytes of zero page set aside for
  them (`$80`-`$BF`).
- It returns a struct or union.
- It calls a function that uses the soft stack and has no zero-page
  bytes of its own. Such a function may use `$80`-`$BF` as scratch,
  and that's where the caller's own parameters are, so they could be
  overwritten by the call. A soft-stack function that does have its
  own bytes, such as one whose address is taken, or a recursive one
  that saves on the hardware stack, is safe to call.

### Mixing the conventions

A soft-stack function can call a zp_abi function. It stores the
arguments in the callee's zero-page bytes like any other caller. The
recursive `fact` above could call a zp_abi helper that way. The
other direction works only when the soft-stack function has
zero-page bytes of its own, as the last condition above says.

### Across source files

Each source file is compiled on its own, and the parameter and local
bytes it binds with `EQU` lines are only right for that file. So
source files compiled with `--optimize` are joined with
`compile.py --link`, which assigns the bytes for the whole program at
once. A function declared `extern` and defined in another file gets
zp_abi on both sides by the same rules, and the linker checks that
the files agree on the size of its parameters and the registers they
arrive in. Each file names the parameter bytes after its own
declaration, so a declaration may name a parameter differently from
the definition, or leave it unnamed. The linker gives each of its
bytes the address of the definition's byte in the same position. The
one thing it rejects is a name that means different bytes in two
files, as when a declaration swaps the names of two parameters of the
same size.

`--link` handles only programs whose functions all use zp_abi. It
rejects a recursive function and one that calls through a pointer,
with an error that names them.

## Costs

A soft-stack prologue is 17 instructions, whatever the frame's size.
The epilogue in the `add3` example is 22, counting the `RTS`, and is
4 shorter for a function that returns one byte, since there's no X to
save. Every access to an argument or a local is a `LDY #offset`
followed by the `(FP),Y` instruction. An `LDA (FP),Y` takes 5
cycles, or 6 when the access crosses a page, and `STA (FP),Y` takes
6, plus 2 for the `LDY`.

A zp_abi function has no prologue or epilogue, and an access to an
argument or a local is a zero-page instruction. A zero-page `LDA`
takes 3 cycles and 2 bytes. Passing an argument costs the caller a
store to the callee's byte, or nothing more than loading it, for a
`register` parameter.

The `add3` example shows the difference. Without `--optimize` it's
74 instructions, and with `--optimize`, where it uses zp_abi, it's
13, counting the `RTS` in both.

## Limits

**Frame size.** The offset in `LDY #offset` is one byte, so a
soft-stack frame has to fit in 255 bytes. A function's locals can
take at most 253 bytes, counting the temporaries the unoptimized
compiler makes, and a larger frame stops the compile with an error
such as "local_bytes 310 out of range 0..253". A large array is
better off as a `static` or a global.

**No overflow check.** The soft stack grows down toward whatever is
below it, which in the simulator is the program's code and data, and
nothing checks that it stays clear. Deep recursion, or large frames
many calls deep, can overwrite them.

**The hardware stack still counts.** Each call that's in progress
keeps a 2-byte return address on the 256-byte hardware stack, and a
soft-stack epilogue briefly pushes up to 2 more bytes. That caps
recursion at about 120 calls deep, and fewer when other code also
uses the hardware stack, however much room the soft stack has. A call
under `#pragma c6502 call save(hardware_stack)` also keeps the bytes
it saved there until it returns, so a call that saves 7 bytes uses 9
per level.

**Zero page runs out.** zp_abi parameters come from the 64 bytes at
`$80`-`$BF`. When a long chain of calls uses more than that, the
bytes past the end are placed in ordinary memory instead. The code
doesn't change, but those accesses take 3-byte absolute instructions.
