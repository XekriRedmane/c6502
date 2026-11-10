# The soft stack

A C program keeps each function's arguments and local variables
somewhere, and a recursive function has a separate copy of them for
each call that's in progress. The 6502's own stack is a poor fit for
that. It's a single 256-byte page at `$0100`-`$01FF`, and the only
ways to reach into it are `PHA` / `PLA` and the stack-pointer
transfers, so there's no way to read "the third byte of my frame"
directly.

So c6502 keeps a second stack in main memory, which it manages
itself, called the *soft stack* here. The 6502's stack
still holds what it's good at, which is return addresses (`JSR` /
`RTS`) and values saved for a few instructions (`PHA` / `PLA`).
Arguments, locals, and the bookkeeping that links one call's storage
to the next all go on the soft stack.

| | Hardware stack | Soft stack |
|---|---|---|
| Where | `$0100`-`$01FF` | main memory, below an address the startup code picks |
| Holds | return addresses, short-lived `PHA` saves | arguments, locals, saved frame pointers |
| Pointer | the 6502's S register | `SSP`, two bytes in zero page |
| Reached with | `PHA`, `PLA`, `JSR`, `RTS` | `LDY #offset` then `LDA (SSP),Y` or `LDA (FP),Y` |

## Two pointers, SSP and FP

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

## A call, step by step

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
both bytes are read.

After `RTS`, `SSP` and `FP` are back where they were before the
caller pushed the arguments. The callee removed the arguments, so the
caller has nothing to clean up.

A function with no arguments and no locals has no prologue, and its
epilogue is just `RTS`.

## Return values

A 1-byte result comes back in A, and a 2-byte `int` in A and X. The
epilogue saves those across the frame teardown, as above. A pointer,
and anything wider than 2 bytes, comes back in the zero-page block
`HARGS`, which the teardown doesn't touch. A function declared
`return_bool_in_carry` returns its result in the carry flag, and its
epilogue saves the flags with `PHP` / `PLP`.

## Taking the address of a local

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

## When the soft stack is used

Without `--optimize`, every function that has arguments or locals
uses it.

With `--optimize`, most functions don't. A function that qualifies
for the zp_abi convention takes its arguments in fixed zero-page
bytes, keeps its locals in zero page too, and has no frame at all.
Here is `add3` with `--optimize`.

```
add3:
   LDA   __zpabi_add3__a_0
   CLC
   ADC   __zpabi_add3__b_0
   STA   __local_add3__0
   LDA   __zpabi_add3__a_1
   ADC   __zpabi_add3__b_1
   TAX
   LDA   __local_add3__0
   CLC
   ADC   #$03
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   RTS
```

A function keeps the soft stack, even with `--optimize`, when any of
these is true.

- It can call itself, directly or through other functions. Each
  active call has its own copy of the arguments, and fixed zero-page
  bytes would be shared by all of them.
- Its address is taken, or it calls through a function pointer. The
  function a call through a pointer reaches isn't fixed when the
  program is compiled, so such a call always passes its arguments
  on the soft stack.
- Its arguments don't fit the 64 bytes of zero page set aside for
  them (`$80`-`$BF`).
- It returns a struct or union.
- It calls a function that uses the soft stack.

Such a function still keeps most of its working values in zero page.
Its frame holds its arguments and anything whose address is taken.
For example, in this recursive function the argument `n` is at
`FP+3`, and the frame has no locals.

```c
unsigned int fact(unsigned char n) {
    if (n <= 1) return 1;
    return n * fact(n - 1);
}
```

With `__attribute__((zp_abi))` on a function, the compiler reports an
error when the function can't use the zero-page convention, instead
of giving it the soft stack (see [attributes.md](attributes.md)).

## Costs

A prologue is 17 instructions, whatever the frame's size. The
epilogue in the example above is 22, counting the `RTS`, and is 4
shorter for a function that returns one byte, since there's no X to
save. Every access to an argument or a local is a `LDY #offset`
followed by the `(FP),Y` instruction. An `LDA (FP),Y` takes 5
cycles, or 6 when the access crosses a page, and `STA (FP),Y` takes
6, plus 2 for the `LDY`. A zero-page `LDA` takes 3. This is the main
reason the optimizer moves what it can to zero page.

## Limits

**Frame size.** The offset in `LDY #offset` is one byte, so a frame
has to fit in 255 bytes. A function's locals can take at most 253
bytes, counting the temporaries the unoptimized compiler makes, and a
larger frame stops the compile with an error such as "local_bytes 310
out of range 0..253". A large array is better off as a `static` or a
global.

**No overflow check.** The soft stack grows down toward whatever is
below it, which in the simulator is the program's code and data, and
nothing checks that it stays clear. Deep recursion, or large frames
many calls deep, can overwrite them.

**The hardware stack still counts.** Each call that's in progress
keeps a 2-byte return address on the 256-byte hardware stack, and an
epilogue briefly pushes up to 2 more bytes. That caps recursion at
about 120 calls deep, and fewer when other code also uses the
hardware stack, however much room the soft stack has.
