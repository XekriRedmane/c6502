# Backward copy propagation

`backward_copy_propagate` in
[passes/optimization_asm/backward_copy_propagation.py](../../passes/optimization_asm/backward_copy_propagation.py)
makes a temporary's definition store straight to the memory the
temporary is later copied to, and deletes the copy. It runs in the
asm SSA fixed point inside `optimize_program`, after
`copy_propagate` in each round, and repeats internally until it
changes nothing. It runs only under `--optimize`.

`tac_to_asm` computes a value into a temporary through A and later
copies the temporary to its destination through A, as `LDA t; STA
D`. When that copy is the temporary's only read, the store that
defined the temporary can write `D` instead, and both the copy and
the temporary disappear. The usual case is a pointer or other wide
return value, which goes to HARGS. Each byte saves a load and a store
of a zero-page byte, 4 bytes and 6 cycles, along with the temporary's
zero-page byte.

## Example

```c
unsigned char *skip(unsigned char *p, unsigned char n) { return p + n; }
```

### Before

```
Label(.skip@asm_ssa_block@0)
Mov(Pseudo(@1.n, 0), Pseudo(%0.1.b0.v1, 0))
Mov(Imm(0), Pseudo(%0.1.b1.v1, 0))
Mov(Pseudo(@0.p, 0), A)
ClearCarry
Add(Pseudo(@1.n, 0), A)
Mov(A, Pseudo(%1.1.b0.v1, 0))
Mov(Pseudo(@0.p, 1), A)
Add(Imm(0), A)
Mov(A, Pseudo(%1.1.b1.v1, 0))
Mov(Pseudo(%1.1.b0.v1, 0), A)
Mov(A, Data(HARGS, 0))
Mov(Pseudo(%1.1.b1.v1, 0), A)
Mov(A, Data(HARGS, 1))
Return(False)
```

### After

```
Label(.skip@asm_ssa_block@0)
Mov(Pseudo(@1.n, 0), Pseudo(%0.1.b0.v1, 0))
Mov(Imm(0), Pseudo(%0.1.b1.v1, 0))
Mov(Pseudo(@0.p, 0), A)
ClearCarry
Add(Pseudo(@1.n, 0), A)
Mov(A, Data(HARGS, 0))
Mov(Pseudo(@0.p, 1), A)
Add(Imm(0), A)
Mov(A, Data(HARGS, 1))
Return(False)
```

The two `Mov`s into `%0.1` are dead copies that forward copy
propagation left in the same round, and byte DCE drops them next.
With the pass turned off, the final code stores the sum in
`__local_skip__1` and `__local_skip__0` and then has `LDA
__local_skip__1; STA HARGS; LDA __local_skip__0; STA HARGS+1` in
front of the `RTS`.

## When it applies

The pass matches `Mov(P, A); Mov(A, D)` at positions `i` and `i+1`,
or a single memory-to-memory `Mov(P, D)` at `i`, which emits as the
same `LDA P; STA D`. `tac_to_asm` writes each byte of a zp_abi
argument that way, so a pointer computed only to be passed, such as
`print_ascii_string(prompts[which].text, 15)` in
`examples/zork/instr_save/prompts.c`, is stored straight into the
callee's parameter bytes. It requires all of these.

- Neither `Mov` is volatile, and neither is the `Mov(A, P)` that
  defines `P`.
- `P` is a `Pseudo` that isn't a static and isn't in the set the SSA
  construction leaves unrenamed (address-taken names, `INC`, `DEC`,
  and shift targets, DPTR-staged pointers, and names volatile `Mov`s
  touch).
- `P` is read exactly once in the function, by this load. Every
  operand position counts, including `LoadAddress` sources and `Phi`
  arguments.
- Exactly one `Mov` before `i` writes `P`, and its source is A.
- `D` is a `Data` or `Stack` operand.
- Between the definition and the pair there is no `Label`, `Jump`,
  `Branch`, `Ret`, `Return`, `Call`, `AllocateStack`, or
  `FunctionPrologue` (the last two move SSP, so a `Stack(k)` before
  one names a different byte than after it), and no operand that can
  name `D`'s byte. `Data` and `Stack` operands match by name and offset,
  `ZP` by address, an `Indirect` matches DPTR, and an `IndirectZp`
  matches its two pointer bytes.
- A and the N and Z flags are dead after the copy, since the deleted
  `LDA P` would have set both.

The definition then becomes `Mov(A, D)` and the pair is deleted.

## What it leaves alone

- A temporary that is read more than once. With `unsigned char
  *last;` and `r = p + n; last = r; return r;`, the pass reports no
  change.
- A destination that is a `Pseudo`, including a static, because
  statics are still `Pseudo`s at this stage. Giving two `Pseudo`s
  one location is the job of [move coalescing](coalescing.md).
- A definition and copy in different basic blocks, or with a call
  between them, since the call can change `D`.
- Soft-stack arguments whose value was computed before the call's
  `AllocateStack`, since `Stack(k)` names a different byte once SSP
  has moved.

## Related

[Asm copy propagation](asm_copy_propagation.md) is the forward
direction. [Byte DCE](byte_dce.md) runs after it in each round.
