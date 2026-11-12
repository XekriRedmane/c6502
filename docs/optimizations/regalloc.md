# Byte-granular register allocation

`color_graph` in
[passes/optimization_asm/regalloc.py](../../passes/optimization_asm/regalloc.py)
gives each value of a function a zero-page byte, or the X or Y
register, and `apply_coloring` in
[passes/optimization_asm/apply_coloring.py](../../passes/optimization_asm/apply_coloring.py)
rewrites the function's operands to match. Both run once per function
in each of the two runs of the asm-level SSA optimizer
(`_optimize_function` in
[passes/optimization_asm/optimizer.py](../../passes/optimization_asm/optimizer.py)),
after the copy propagation and dead-code fixed point and [move
coalescing](coalescing.md), and before SSA destruction. The
preliminary run uses the default zero-page partition and is used
only to measure each function's demand for bytes. The final run draws from
the private pools sized from that demand, in the functions that have
one. Only the `--optimize` pipeline
allocates registers. The unoptimized pipeline puts every local on the
soft-stack frame.

A local on the soft-stack frame is read with `LDY #off; LDA (FP),Y`,
which is 4 bytes and 7 cycles. A local in zero page is read with `LDA
zp`, which is 2 bytes and 3 cycles, and it can be the operand of
`INC`, `ASL`, `CPX`, and the other instructions that have no `(zp),Y`
form. A value in X or Y takes no load before an `abs,X` or `(zp),Y`
access, and it steps with `INX` or `DEY`.

The allocation works on single bytes. Asm SSA has split every
multi-byte value into one name per byte and per definition, so the
two bytes of an `int` can go to unrelated places, one of them in a
register. A few names stay whole and take a run of contiguous bytes.
These are the destination of a `LoadAddress` and any name referenced
at more than one offset, such as the target of an in-place multi-byte
shift.

## How it works

`compute_liveness` in
[liveness.py](../../passes/optimization_asm/liveness.py) computes
which names are live at each point, and `build_interference` in
[interference.py](../../passes/optimization_asm/interference.py)
joins two names with an edge when one is defined while the other is
live. All of a block's Phi destinations interfere with each other.
The builder also marks each name that's live across a `Call`.

`color_graph` then colors in two steps.

1. It places names in X and Y first (see below).
2. It walks the remaining names in an order taken from the dominator
   tree and gives each the lowest base address whose bytes no
   already-colored neighbor occupies and no blocked address covers. A
   name that finds no room is spilled, and `replace_pseudoregisters`
   puts it on the soft-stack frame.

`apply_coloring` replaces each colored Pseudo with its byte or
register. A byte in the function's private pool becomes a `Data`
operand named by its slot symbol, such as `__local_sum__s`, so the
multi-TU linker can move it. Any other byte becomes a numeric `ZP`
operand.

## Where the bytes come from

When the function has a private pool from
[allocate_function_locals](zp_local_pools.md), every byte comes from
that pool. The pool is disjoint from the pools of the function's
callers and callees, so a value can stay in it across any call, and
the allocator doesn't look at which values are live across calls. A
pool that ran out of zero page extends above `$FF`, and those bytes
assemble with absolute addressing.

Otherwise the allocator uses the default partition. A value live
across a call takes a callee-saved byte in `$C0`-`$FF`, which the
function's prologue saves and its epilogue restores. Any other value
takes a caller-saved byte in `$80`-`$BF`, or a callee-saved one when
the caller-saved half is full.

Some bytes are never handed out. These are the function's own zp_abi
parameter slots, the parameter slots of every zp_abi function it
calls directly, and the bytes of the zero-page globals. Each is
blocked for the whole function.

## Placement in X and Y

`scan_function` in
[hwreg_eligibility.py](../../passes/optimization_asm/hwreg_eligibility.py)
finds the names that could live in X or in Y. Every definition and
every use must be one of these.

- A copy to or from an immediate, a register, a `Data` or `ZP` byte,
  or another value, which becomes `LDX`, `STX`, `TAX`, or `TXA`. A
  copy to or from the soft stack or through a pointer doesn't
  qualify, because `LDX` and `STX` have no `(zp),Y` mode. Neither
  does a copy to or from a value whose address is taken, since such
  a value gets no color and may end up on the soft stack.
- An increment or decrement, which becomes `INX` or `DEX`.
- A compare with the value on the left and an immediate or a `Data`
  or `ZP` byte on the right, which becomes `CPX`.
- A Phi, as its destination or as one of its arguments.

A name used in arithmetic, a shift, a push, or on the right of a
compare is out, and so is one stored into an indexed operand, since
`STX` and `STY` have no indexed absolute mode. A load from `abs,X`
makes a name eligible for Y only, and a load from `abs,Y` for X only,
because `LDX abs,X` and `LDY abs,Y` don't exist. The eligibility is
per name before coalescing, and a coalesced node is eligible only
when all its members are.

`_try_hwreg_assign` in `regalloc.py` then pins names in this order.

1. A `register` local must go in Y. If it can't, compilation stops
   with a `RegallocError`, because the local has nowhere else to live.
2. Each hinted name goes in its preferred register, or in the other
   one when the preferred one fails the checks below.
   A name gets an X hint when it feeds a `Mov(P, A); Mov(A, X)`
   index setup, and a Y hint for the same with Y. A 1-byte
   `register` parameter passed in X gets an X hint, and if the hint
   fails the parameter stays in its zp_abi slot. Within each group,
   names with more references go first.

The allocator makes no attempt to pin an eligible name with no hint
and no `register` keyword, and the name takes a byte like any other. A
loop counter that never serves as an index therefore stays in memory,
unless a later promotion such as
[counter_to_index_reg](counter_to_index_reg.md) moves it.

A pin succeeds only if the node is one byte wide, isn't live across a
call, has no neighbor already in the same register, and no other
instruction writes that register while the name is live. A is never
a color, because the asm IR routes nearly every operation through
it.

After the substitution, an index setup for a pinned name becomes a
transfer from the register to itself through A, and `apply_coloring`
drops it. When the pinned register differs from the index register,
it drops the two transfers and changes the index of the following
`IndexedData` accesses to the pinned register.

## Example

```c
#include <stdint.h>
uint16_t sum(const uint8_t *p, uint8_t n) {
    uint16_t s = 0;
    for (uint8_t i = 0; i < n; i++) s += p[i];
    return s;
}
```

The listings are `apply_coloring`'s input and output in the final run.

### Before

```
Label(.sum@asm_ssa_preheader@0)
Label(.sum@ssa_block@0)
Label(.loop@0_start)
Phi(Pseudo(@2.s.2.b0.v2, 0), [AsmPhiArg(.sum@ssa_block@0, Imm(0)), AsmPhiArg(.loop@0_continue, Pseudo(%9.1.b0.v1, 0))])
Phi(Pseudo(@2.s.2.b1.v2, 0), [AsmPhiArg(.sum@ssa_block@0, Imm(0)), AsmPhiArg(.loop@0_continue, Pseudo(%9.1.b1.v1, 0))])
Phi(Pseudo(@3.i.2.b0.v2, 0), [AsmPhiArg(.sum@ssa_block@0, Imm(0)), AsmPhiArg(.loop@0_continue, Pseudo(%11.1.b0.v1, 0))])
Mov(Pseudo(@3.i.2.b0.v2, 0), A)
Compare(A, Pseudo(@1.n, 0))
Branch(CS, .loop@0_break)
Label(.sum@asm_ssa_block@0)
Mov(Pseudo(@0.p, 0), A)
Mov(A, Data(DPTR, 0))
Mov(Pseudo(@0.p, 1), A)
Mov(A, Data(DPTR, 1))
Mov(Pseudo(@3.i.2.b0.v2, 0), A)
Mov(A, Y)
Mov(IndirectY, A)
Mov(A, Pseudo(%7.1.b0.v1, 0))
Mov(Pseudo(@2.s.2.b0.v2, 0), A)
ClearCarry
Add(Pseudo(%7.1.b0.v1, 0), A)
Mov(A, Pseudo(%9.1.b0.v1, 0))
Mov(Pseudo(@2.s.2.b1.v2, 0), A)
Add(Imm(0), A)
Mov(A, Pseudo(%9.1.b1.v1, 0))
Label(.loop@0_continue)
Mov(Pseudo(@3.i.2.b0.v2, 0), A)
ClearCarry
Add(Imm(1), A)
Mov(A, Pseudo(%11.1.b0.v1, 0))
Jump(.loop@0_start, [])
Label(.loop@0_break)
Mov(Pseudo(@2.s.2.b1.v2, 0), A)
Mov(A, X)
Mov(Pseudo(@2.s.2.b0.v2, 0), A)
Return(True, True)
```

### After

```
Label(.sum@asm_ssa_preheader@0)
Label(.sum@ssa_block@0)
Label(.loop@0_start)
Phi(Data(__local_sum__s, 0), [AsmPhiArg(.sum@ssa_block@0, Imm(0)), AsmPhiArg(.loop@0_continue, Data(__local_sum__s, 0))])
Phi(X, [AsmPhiArg(.sum@ssa_block@0, Imm(0)), AsmPhiArg(.loop@0_continue, X)])
Phi(Y, [AsmPhiArg(.sum@ssa_block@0, Imm(0)), AsmPhiArg(.loop@0_continue, Y)])
Mov(Y, A)
Compare(A, Pseudo(@1.n, 0))
Branch(CS, .loop@0_break)
Label(.sum@asm_ssa_block@0)
Mov(Pseudo(@0.p, 0), A)
Mov(A, Data(DPTR, 0))
Mov(Pseudo(@0.p, 1), A)
Mov(A, Data(DPTR, 1))
Mov(IndirectY, A)
Mov(A, Data(__local_sum__0, 0))
Mov(Data(__local_sum__s, 0), A)
ClearCarry
Add(Data(__local_sum__0, 0), A)
Mov(A, Data(__local_sum__s, 0))
Mov(X, A)
Add(Imm(0), A)
Mov(A, X)
Label(.loop@0_continue)
Mov(Y, A)
ClearCarry
Add(Imm(1), A)
Mov(A, Y)
Jump(.loop@0_start, [])
Label(.loop@0_break)
Mov(Data(__local_sum__s, 0), A)
Return(True, True)
```

The loop counter `i` goes in Y, from its hint as the index of the
`(zp),Y` load, and the setup `LDA i; TAY` in front of that load is
gone. The high byte of `s` goes in X, from its hint as the high byte
of the return value, which travels in X. The low byte of `s` and the
loaded byte take two bytes of `sum`'s pool. The parameters `p` and
`n` stay Pseudos, because they live in their zp_abi slots. In the
preliminary run the same two bytes come out as `ZP(132, 0)` and
`ZP(131, 0)`, which are caller-saved bytes above the parameter slots
at `$80`-`$82`. The peepholes later turn `Mov(Y, A); Compare(A, n)`
into `CPY` and the X carry byte into `BCC; INX`, and the final
assembly is shown under [move coalescing](coalescing.md).

## When it applies

Every function in the `--optimize` pipeline goes through the
allocator. A name is colored when it's a node of the interference
graph, which excludes statics, parameters, and the sources of a
`LoadAddress`.

## What it leaves alone

- Parameters. A zp_abi parameter lives in its slot symbol, and a
  soft-stack parameter lives on the frame. A `register` parameter's
  X hint applies to the SSA names the body copies it into.
- Statics, which stay at their link-time addresses, and address-taken
  locals, which [address-taken locals in zero
  page](address_taken_zp.md) places separately.
- X or Y for a value live across a call that writes the register.
  The pin check counts a call as writing X and Y unless the routine
  is declared `preserves_x` / `preserves_y`
  (`_instr_writes_hwregs`), so a `register` local can live in Y
  across a call to `COUT1`, but across any other call it's a compile
  error. [loop_counter_to_x](loop_counter_to_x.md) can move a
  counter into X across a call that changes X later, saving and
  reloading it around the call.
- A, which is never a color.
- A copy cycle that the coloring creates. When two Phi copies on one
  edge swap the contents of two bytes, SSA destruction breaks the
  cycle through a fresh temporary that no pool covers, so the
  temporary goes on the soft-stack frame. In the loop step `t = a +
  b; a = b; b = t;`, `t` takes `a`'s byte, the edge copies `a ← b` and
  `b ← t` form a cycle, and the function gets a full soft-stack
  prologue for one byte.

## Related

[Move coalescing](coalescing.md) merges nodes before coloring, and
[private local pools](zp_local_pools.md) and [zp_abi](zp_abi.md)
supply the bytes. [Prologue synthesis](prologue_synthesis.md) emits
the saves for callee-saved bytes and the frame for spills.
