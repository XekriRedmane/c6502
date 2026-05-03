# The Optimization Pipeline

This is a from-scratch tour of how `--optimize` turns "correct but
slow" TAC into "correct and faster" TAC. Written for someone who has
read CLAUDE.md but never touched the optimizer code.

## Why optimize at all?

Without `--optimize`, the c6502 compiler produces working 6502 code
that puts every variable on the soft stack. Reading a variable costs
8 cycles (`LDY #off; LDA (FP),Y`). With optimization on, hot
variables instead live in zero-page bytes (`$80-$FF`) and reading
costs 3 cycles (`LDA $XX`). About 3× faster per access.

Plus, the optimizer folds constants, drops dead code, and propagates
copies — the usual dance.

## Where the optimizer fits

```
parse → resolve names → check types → c99_to_tac
                                            │
                                            ▼ TAC
                                       [optimize]   ← the subject
                                            │
                                            ▼ optimized TAC + Coloring
                              tac_to_asm → replace_pseudoregisters → asm_emit
```

The optimizer takes a `tac_ast.Program` and returns one. Same
shape, just better. It also returns a `dict[func_name, Coloring]`
that downstream `replace_pseudoregisters` consumes to know which
variables go in zero-page.

The driver lives in `passes/optimization/optimizer.py`.

## The phases at a glance

```
to_ssa → [constant_fold → UCE → copy_propagate → DSE]* → regalloc → from_ssa
            └────────── fixed-point loop ───────────┘
```

Five distinct phases:

1. **`to_ssa`** renames variables so each has exactly one definition.
   Inserts Phi nodes at control-flow merges.
2. **The fixed-point loop** runs four cleanup passes in rotation
   until none of them change anything.
3. **Register allocation** computes a coloring that maps SSA
   variables to zero-page slots.
4. **`from_ssa`** lowers Phi nodes back to ordinary Copies so
   `tac_to_asm` can handle the result.
5. (Outside the optimizer.) `replace_pseudoregisters` turns the
   coloring into actual `ZP(addr, offset)` operands and lays out
   the frame around them; `asm_emit` produces 6502 mnemonics.

The rest of this document walks through each phase in detail.

---

## Phase 1: `to_ssa` (passes/optimization/ssa_construction.py)

### The problem

Real code reassigns variables a lot:

```c
x = 1;
x = x + 5;
x = x * 2;
```

There are three different "x"s here, but they all share a name.
That makes the optimizer's life hard: "what value does `x` have at
line 3?" depends on where exactly you point.

### The idea: SSA

What if every assignment got a fresh, unique name? Like, instead of
three `x`s, we have `x.1`, `x.2`, `x.3`:

```
x.1 = 1
x.2 = x.1 + 5
x.3 = x.2 * 2
```

Now there's no ambiguity. Each variable is **statically defined
exactly once** — hence "Static Single Assignment". This is the SSA
property.

In SSA, asking "what's the value of x.2?" has one answer: `x.1 + 5`.
That makes a lot of optimizations almost trivial. Copy propagation,
dead-store elimination, value numbering — all dramatically simpler
when each name has one definition.

### The wrinkle: branches

What about:

```c
if (c) {
    x = 1;
} else {
    x = 2;
}
return x;
```

Two definitions. After the join, "what's x?" depends on which
branch executed. We can't just pick a single name.

The fix is a **Phi node**:

```
if (c) {
    x.1 = 1;
} else {
    x.2 = 2;
}
.join:
    x.3 = phi(then-branch: x.1, else-branch: x.2);
return x.3;
```

A Phi says: "x.3 takes whichever value flowed in along the edge we
arrived on." Think of it like a conductor at a busy intersection.
When you reach the intersection from the south, x.3 is x.1; from the
east, x.3 is x.2.

Phis are not real machine instructions. They exist only inside the
optimizer. `from_ssa` will turn them back into regular Copies later.

### How `to_ssa` actually works

The classical algorithm (Cytron et al. 1991), in five steps:

1. **Identify promotable Vars.** Not every variable can be SSA-
   renamed. To qualify:
   - It must be `LocalAttr` (a block-scope local, function param,
     or compiler-introduced temp). Globals and statics stay put.
   - It must be a scalar type (Int, Long, Float, Pointer, ...).
     Arrays and structs go through `GetAddress + Load/Store` and
     are effectively address-taken.
   - Nobody must have taken its address with `&`. If `&x` exists
     somewhere, x's storage must stay at a fixed location, so we
     can't rename it.

2. **Build the CFG and compute dominators.** A node A "dominates"
   node B if every path from the start to B goes through A. The
   immediate dominator (idom) is the closest such ancestor.

3. **Compute dominance frontiers (DF).** DF(B) is the set of
   "boundary" blocks where B's definitions stop being uniquely
   in charge — the join points where another path could come in
   with a different value. Phi nodes for variables defined in B go
   at the iterated DF of B (DF, plus DF of each block in DF, plus
   ...).

4. **Place Phis (pruned).** For each promotable variable v, find
   every block that defines v, then put empty Phi nodes at the
   iterated DF of those blocks. **Pruning**: only place a Phi at
   block X if v is "live in" at X. If v is dead at X, the Phi's
   result would be unread anyway, so don't bother. This avoids a
   lot of useless Phis.

5. **Rename.** Walk the dominator tree top-down. Each definition
   gets a fresh number (`x.1`, `x.2`, ...). Each use gets the most
   recent number on a stack. Phi dsts also get fresh numbers.
   When a successor's Phi has an arg from this block's edge, the
   arg's source is whatever's currently on top of the stack for
   that variable.

The output is a function with the same instruction shape, except
each variable name has been replaced with `<orig>.<N>`, and Phi
nodes have been inserted at appropriate joins.

### A concrete example

Source:

```c
int main(void) {
    int i = 100;
    do ; while ((i = i - 5) >= 50);
    return i;
}
```

After `to_ssa`:

```
.preheader:
    Copy(100, @0.i.1)
.loop_start:
    Phi(@0.i.2, [(.preheader, @0.i.1), (.continue, @0.i.3)])
.continue:
    Binary(Sub, @0.i.2, 5, %0.1)
    Copy(%0.1, @0.i.3)
    Binary(GreaterOrEqual, @0.i.3, 50, %1.1)
    JumpIfTrue(%1.1, .loop_start)
.loop_break:
    Ret(@0.i.3)
```

Three SSA names for `i`:
- `@0.i.1`: the initial value (100)
- `@0.i.2`: the Phi result at the loop top — either the initial
  value or the previous iteration's update
- `@0.i.3`: the post-update value (i - 5)

The Phi on entry has two predecessors: `.preheader` (entry path,
contributing `@0.i.1`) and `.continue` (back-edge, contributing
`@0.i.3`).

---

## Phase 2: The Fixed-Point Loop

```
while True:
    prev = fn
    fn = constant_fold(fn)
    fn = eliminate_unreachable_code(fn)
    fn = copy_propagate(fn)
    fn = eliminate_dead_stores(fn)
    if fn == prev:
        break
```

Four passes take turns simplifying. Why a loop? Because each pass
enables the others. Constant folding might make a conditional
jump unconditional, which lets UCE drop a block, which makes some
copies dead, which lets DSE remove them, which exposes more
constants...

The loop stops when one full cycle made no structural change
(`fn == prev`, dataclass equality).

### 2a. `constant_fold` (passes/optimization/constant_folding.py)

If you can compute it now, why wait until runtime?

Folds:
- **Arithmetic**: `Binary(Add, 3, 4)` → `Copy(7, dst)`.
- **Comparisons**: `Binary(GT, 5, 3)` → `Copy(1, dst)` (true).
- **Casts of constants**: `Cast(Long, ConstInt(5))` →
  `ConstLong(5)`.
- **Unary ops**: `Unary(Negate, 7)` → `Copy(-7, dst)`,
  `Unary(LogicalNot, 0)` → `Copy(1, dst)`.
- **Conditional jumps with constant conditions**: `JumpIfTrue(true)`
  → `Jump(target)`; `JumpIfTrue(false)` → dropped entirely.
- **Phis where every arg agrees**: `Phi(dst, [(_, c), (_, c)])` →
  `Copy(c, dst)`.

Critically, it does **arithmetic at the right width**. The 6502 has
narrow types: `int` is 16 bits. So `30000 + 5000 = 35000`, but
35000 doesn't fit in signed 16-bit — it wraps to -30536. The
folder reproduces that wraparound so the optimized code matches
exactly what the 6502 would compute at runtime. Width canonicalization
goes through `_INTEGER_CONST_BITS` (Int=16, Long=32, LongLong=64;
unsigned variants the same widths).

### 2b. `eliminate_unreachable_code` (UCE)

After folding, some code can't run anymore.

Five sub-steps:

1. **Drop unreachable blocks.** Forward DFS from the entry. Any
   block we don't visit is dead — dropped. Phi args in surviving
   blocks whose `pred_label` named a dropped block are also
   dropped.

2. **Prune dead Phi-edge args.** If constant folding dropped a
   conditional jump, the edge from one block to another is gone —
   but the destination's Phi still references the source's label.
   Walk every Phi and drop args whose `pred_label` doesn't match
   an actual current predecessor.

3. **Fold singleton Phis.** A Phi with only one remaining arg is
   semantically just a Copy. Rewrite `Phi(dst, [(_, src)])` →
   `Copy(src, dst)`. (A zero-arg Phi means its block became
   unreachable; defensive drop.)

4. **Drop useless jumps.** A `Jump(L)` whose target L is the very
   next block is redundant — fall-through gets there for free.
   Same for conditional jumps where both successors equal the next
   block.

5. **Drop useless labels.** A `Label(L)` that no remaining Jump
   targets AND no Phi `pred_label` references is just decoration.
   Drop it.

### 2c. `copy_propagate`

`Copy(src, dst)` says "dst equals src." So everywhere that uses
`dst`, we can equally well use `src`:

```
Copy(5, x)              ⇒    Copy(5, x)
Binary(Add, x, 3, y)         Binary(Add, 5, 3, y)
```

(Then constant folding kicks in next round and computes `5 + 3 = 8`.)

Importantly, this is sound **in SSA form** because every variable
has exactly one definition. `dst` was set ONCE by the Copy, so
substituting `src` for it is always correct. In non-SSA TAC,
`dst` could be reassigned later, breaking the equation.

The pass also chains: if `x = y` and `y = z`, then uses of `x`
become `z` directly.

### 2d. `eliminate_dead_stores` (DSE)

A definition whose result is never used can be dropped:

```
x = 5;        ← x is never used, drop it
y = 7;
return y;
```

In SSA, "is x used?" is just "does the name `x` appear anywhere as
a use?" If not, drop the def.

Special case for function calls: a call may have side effects
(prints, writes through pointers, calls into hardware). Don't drop
the call itself, just drop the unused dst:
`FunctionCall(f, args, dst=x)` → `FunctionCall(f, args, dst=None)`
when x is unused.

The pass iterates to fixed point internally too — dropping one
def can make its inputs dead, which can make their inputs dead, etc.

---

## Phase 3: Register Allocation

This is the headline feature. The 6502 has only A, X, Y as real
registers, but C code needs many more "register-like" slots. The
trick: use zero-page memory ($80-$FF, configurable) as a register
file. 128 byte-wide "registers" in the default pool.

The phase has three sub-phases.

### 3a. liveness (passes/optimization/liveness.py)

For each program point, which variables are live? A variable is
**live** if it has a value that someone might still read.

Rules:
- A use of x makes x live (someone wants its value).
- A def of x kills x (the old value is gone; the new value will
  be live until its next use).

This is computed as a backward dataflow: walk each block in
reverse, maintaining a `live` set. At each instruction, kill the
defs and add the uses. Iterate the per-block live-in/live-out
across the CFG until fixed point.

Two SSA-specific wrinkles for Phis:
- A Phi's source is conceptually used at the END of the matching
  predecessor (the edge), not at the Phi's block. So `phi.args`
  contribute to predecessor live-out, not to the Phi block's
  live-in.
- A Phi's dst is conceptually defined at the END of every
  predecessor (since `from_ssa` will insert a Copy there). So Phi
  dsts also contribute to predecessor live-out — otherwise the
  regalloc could happily share the dst's slot with another value
  that's still live in the predecessor, and the future de-SSA
  Copy would clobber it.

The result is per-block `live_in[B]` and `live_out[B]`, plus a
lazy per-instruction `live_after(bid, i)` query.

### 3b. interference graph (passes/optimization/interference.py)

Two variables **interfere** if they're both live at the same
program point. Such pairs can't share a slot — putting them in the
same byte would mean one overwrites the other.

The graph:
- Nodes: variables (well, the colorable ones — locals and SSA
  temps; statics and function names get filtered out).
- Edges: pairs (a, b) where a and b are simultaneously live.
- Each node carries:
  - **`width`**: 1, 2, 4, or 8 bytes (read from the symbol table:
    Char/SChar/UChar=1; Int/UInt/Pointer=2; Long/ULong/Float=4;
    LongLong/ULongLong/Double=8).
  - **`lives_across_call`**: True iff this value is live at the
    moment some `FunctionCall` happens. Drives the caller-saved
    vs callee-saved decision.

Built by walking each block in reverse, maintaining a `live` set
initialized to `live_out[B]`. At each instruction's defs, edge each
def with everything currently live. Then remove defs from live; add
uses to live. Sibling Phi dsts in the same block all interfere with
each other and with everything live just before the first non-Phi
instruction.

For SSA, this graph has a beautiful property: it's **chordal**. That
means it admits a perfect elimination order, and greedy coloring in
that order gives the optimal solution. (Without SSA, coloring is
NP-hard.)

### 3c. coloring (passes/optimization/register_allocation.py)

"Coloring" means assigning each node a color (a starting ZP byte
address) such that no two adjacent nodes (interfering vars) overlap.

Imagine a map of countries: each country needs a different color
from its neighbors. Same idea, except our "colors" are ranges of
byte addresses, sized by each variable's width.

The algorithm:

1. **Compute a perfect elimination order (PEO).** Walk the
   dominator tree top-down listing each value definition. Reverse
   the result. Params and any other variables that aren't defined
   by an instruction (e.g. SSA-renamed params) appear LAST in the
   build order, so they're FIRST in the reversed PEO and get
   colored first.

2. **For each variable in PEO order:**
   - Compute `blocked_bytes`: union of byte ranges of every
     already-colored neighbor.
   - Choose a pool based on `lives_across_call`:
     - **True** → callee-saved first. The function's prologue
       saves these slots and the epilogue restores them, so callees
       can't disturb them. Doesn't fall back to caller-saved
       (caller-saved would be clobbered by the call).
     - **False** → caller-saved first. No save/restore overhead.
       Falls back to callee-saved if caller-saved is full.
   - In the chosen pool, find the lowest base such that `[base,
     base+width)` is fully inside the pool's range AND disjoint
     from `blocked_bytes`. Pick that.
   - If nothing fits, **spill** the variable (added to
     `coloring.spilled`). It'll get a frame slot via
     `replace_pseudoregisters` later — slower than ZP, but
     correct.

The result is a `Coloring`:

```python
@dataclass
class Coloring:
    assignments: dict[str, int]    # name -> ZP base address
    spilled: set[str]              # names that didn't fit
    pool: Pool                     # echo of the pool used
```

### Caller-saved vs callee-saved (the convention)

This is about who's responsible for preserving a value across a
function call.

**Caller-saved**: if the caller has a value in caller-saved ZP and
calls a function, the callee might trash it. The caller must save
it before the call (or just not put values that need to survive
across the call there).

**Callee-saved**: if a function uses a callee-saved ZP byte, IT
promises to save the prior contents in its own prologue and restore
them in its own epilogue. Callers can rely on their callee-saved
values surviving across the call.

The c6502 default split: $80-$BF (64 bytes) caller-saved, $C0-$FF
(64 bytes) callee-saved. The starting address is configurable via
`Pool(start=...)`.

The actual save/restore happens later, in
`replace_pseudoregisters` + `asm_emit`. See Phase 5.

---

## Phase 4: `from_ssa` (passes/optimization/ssa_destruction.py)

We're done optimizing. Now undo the SSA renaming so `tac_to_asm`
can lower the code (it doesn't know how to handle Phi nodes).

### The plan

A Phi like:

```
.join:
    x.3 = phi(then-branch: x.1, else-branch: x.2)
```

becomes one Copy in each predecessor:

```
.then:
    ...
    Copy(x.1, x.3)            ← inserted here
    jump .join
.else:
    ...
    Copy(x.2, x.3)            ← inserted here
.join:
    use x.3
```

Now there are no more Phis. Each branch ends with a Copy that
puts the right value into `x.3` for what comes after.

### The wrinkle: parallel-copy semantics

Multiple Phis at the same join produce multiple Copies in the same
predecessor. Naive emission can break things.

Example after copy propagation in a loop:

```
.loop_top:
    Phi(@i.new, [..., (.continue, %4)])
    Phi(@counter, [..., (.continue, @i.new)])
```

In `.continue`, the de-SSA emits TWO Copies. Naive source order:

```
Copy(%4, @i.new)         ← writes @i.new
Copy(@i.new, @counter)   ← reads @i.new — but it just got
                           overwritten with the new value!
```

This is the classic **lost copy** problem. We need the second Copy
to read the OLD value of `@i.new`, but the first Copy already
overwrote it.

### The fix: topological sort

Reorder the Copies so reads happen before overlapping writes:

```
Copy(@i.new, @counter)   ← read @i.new first (still has old value)
Copy(%4, @i.new)         ← now overwrite
```

Algorithm: repeatedly emit any pending Copy whose dst isn't read
by another pending Copy. Remove. Repeat. When no such Copy
exists, all remaining Copies form a cycle.

### Cycles need a temp

```
Copy(a, b)    ← b = a
Copy(b, a)    ← a = b
```

A literal swap. Topological sort can't help — every Copy reads
what another Copy writes. Solution: a temporary:

```
Copy(b, tmp)   ← save old b
Copy(a, b)     ← b = old a
Copy(tmp, a)   ← a = old b (from saved tmp)
```

The temp is a fresh `<funcname>.cycle_tmp@N` Var. Its type is
registered in the symbol table (matching the cycle members'
type). Since regalloc has already run, the temp doesn't get a
zero-page slot — `replace_pseudoregisters` will give it a frame
slot, slower than ZP but correct.

After `from_ssa`, the function has zero Phi nodes and is back to
regular non-SSA TAC.

---

## Phase 5: After the optimizer (replace_pseudoregisters + asm_emit)

The optimizer returns `(prog, colorings)`. Downstream code
consumes both.

### `tac_to_asm`

Lowers TAC to asm IR. Each TAC value becomes an asm `Pseudo(name,
offset)` operand. Doesn't know about coloring yet — just produces
Pseudos uniformly.

### `replace_pseudoregisters`

Receives the `colorings` dict and rewrites every Pseudo in the
function:

| If the Pseudo's name is...                | Lowers to                    |
|-------------------------------------------|------------------------------|
| in the program's static-storage set       | `Data(name, offset)`         |
| in the function's params                  | `Frame(...)` (calling conv)  |
| in `coloring.assignments`                 | `ZP(addr, offset)`           |
| anything else (uncolored / spilled)       | `Frame(...)` (frame slot)    |

Static-storage objects use absolute addressing by symbolic name.
Params arrive on the soft stack (per the calling convention) so
they ALWAYS go to Frame even if regalloc colored them. ZP-colored
locals go to direct zero-page addressing. Uncolored or spilled
locals fall back to frame-relative storage.

Additionally, this pass **derives the callee-saved-byte set**: for
each colored value, walk its bytes (`[base, base+width)`); the
ones that fall in `coloring.pool.callee_saved()` need to be saved
by the function's prologue. The save area sits at the bottom of
the frame: `FP+1..FP+S` where S is the number of saved bytes.
Locals shift up by S to leave room.

### `asm_emit`

Lowers each asm-IR instruction to actual 6502 mnemonics:

- `LDA $XX` for `ZP(0xXX, 0)`
- `LDA $XY` for `ZP(0xXX, 1)` (offset folded at emit time —
  XY = XX + 1)
- `LDA name+0` (= `LDA name`) for `Data(name, 0)`
- `LDY #off; LDA (FP),Y` for `Frame(off)`
- ...and so on.

The **prologue** emits the standard FP setup, then for each
callee-saved address, `LDA $XX; LDY #(slot+1); STA (FP),Y`. The
**epilogue** does the reverse: `LDY #(slot+1); LDA (FP),Y; STA
$XX` for each addr, BEFORE the SSP/FP teardown (so FP is still
valid for the indirect-Y reads).

A small **self-Mov peephole** drops `Mov(src, dst)` when `src ==
dst`. This catches the case where a Phi's source and destination
ended up at the same color — the de-SSA Copy is technically `LDA
$XX; STA $XX`, a no-op. Also covers the now-correct Reg(A)→Reg(A)
self-transfers.

---

## Putting it all together: end-to-end example

C source:

```c
int main(int n) {
    int a = n + 1;
    int b = 0;
    for (int i = 0; i < n; i = i + 1) {
        b = b + a;
    }
    return b;
}
```

After `c99_to_tac` (sketch):

```
main(n):
    a = n + 1
    b = 0
    i = 0
.loop_start:
    cond = i < n
    if !cond goto .loop_end
    b = b + a
    i = i + 1
    goto .loop_start
.loop_end:
    return b
```

After `to_ssa` — Phis at the loop top for `b` and `i` (which
change each iteration). `a` doesn't change, so no Phi for it.

```
main(n):
    a.1 = n + 1
    b.1 = 0
    i.1 = 0
.loop_start:
    b.2 = phi(.entry: b.1, .continue: b.3)
    i.2 = phi(.entry: i.1, .continue: i.3)
    cond.1 = i.2 < n
    if !cond.1 goto .loop_end
    b.3 = b.2 + a.1
    i.3 = i.2 + 1
    goto .loop_start
.loop_end:
    return b.2
```

After the **fixed-point loop**: nothing folds (n is unknown at
compile time), so the function is structurally unchanged.

After **regalloc** (default pool, $80-$FF):
- `n`, `a.1`, `b.2`, `i.2` are live across the loop iterations and
  the Phi merges, so they get callee-saved colors (≥ $C0).
- `b.3`, `i.3`, `cond.1` are short-lived; they go in caller-saved
  (≥ $80, < $C0).

After `from_ssa` — Phis lowered to Copies in `.continue` (the
back-edge pred), topologically sorted:

```
main(n):
    a.1 = n + 1
    b.1 = 0
    i.1 = 0
    b.2 = b.1     ← from the entry-edge Phi sources
    i.2 = i.1
.loop_start:
    cond.1 = i.2 < n
    if !cond.1 goto .loop_end
    b.3 = b.2 + a.1
    i.3 = i.2 + 1
    b.2 = b.3     ← from the back-edge Phi sources
    i.2 = i.3     ← (sorted to read before overwriting)
    goto .loop_start
.loop_end:
    return b.2
```

When this lowers to 6502 asm via `tac_to_asm` +
`replace_pseudoregisters` + `asm_emit`, the loop body is fast: each
variable read is `LDA $XX` instead of `LDY #off; LDA (FP),Y`. And
the prologue saves whichever ZP bytes the function uses from
$C0-$FF, restoring them in the epilogue, so the caller's view of
those bytes survives the call.

### The actual 6502 assembly

Here's the output of
`compile.py - --codegen --optimize` on the C source above:

```
main:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   FP
   STA   (SSP),Y
   INY
   LDA   FP+1
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

.main@ssa_block@0:
   LDY   #$03                 ; load param n (low byte) from FP+3
   LDA   (FP),Y
   CLC
   ADC   #$01                 ; n + 1
   STA   $8A                  ; a.1 lo  → ZP $8A
   LDY   #$04                 ; load param n (high byte) from FP+4
   LDA   (FP),Y
   ADC   #$00
   STA   $8B                  ; a.1 hi  → ZP $8B
   LDA   #$00                 ; b.2 = 0 (init)
   STA   $88                  ;   b.2 lo → ZP $88
   LDA   #$00
   STA   $89                  ;   b.2 hi → ZP $89
   LDA   #$00                 ; i.2 = 0 (init)
   STA   $86                  ;   i.2 lo → ZP $86
   LDA   #$00
   STA   $87                  ;   i.2 hi → ZP $87
.loop@0_start:
   LDA   $86                  ; cond.1 = i.2 < n  (signed compare)
   SEC
   LDY   #$03
   SBC   (FP),Y               ; subtract n's low byte from i.2 lo
   LDA   $87
   LDY   #$04
   SBC   (FP),Y               ; subtract n's high byte from i.2 hi (with borrow)
   BVC   .cmp_novf@0          ; correct N flag for signed overflow
   EOR   #$80
.cmp_novf@0:
   BMI   .cmp_true@1          ; if signed result negative → i.2 < n
   LDA   #$00
   JMP   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   STA   $82                  ; cond.1 lo → ZP $82
   LDA   #$00
   STA   $83                  ; cond.1 hi → ZP $83
   LDA   $82                  ; if !cond.1 goto .loop_break
   ORA   $83
   BEQ   .loop@0_break
   LDA   $88                  ; b.3 = b.2 + a.1
   CLC
   ADC   $8A
   STA   $84                  ;   b.3 lo → ZP $84
   LDA   $89
   ADC   $8B
   STA   $85                  ;   b.3 hi → ZP $85
.loop@0_continue:
   LDA   $86                  ; %temp = i.2 + 1  (the i+1 computation)
   CLC
   ADC   #$01
   STA   $82                  ;   %temp reuses ZP $82 (cond.1's slot —
   LDA   $87                  ;   they don't interfere)
   ADC   #$00
   STA   $83
   LDA   $84                  ; de-SSA Copy: b.2 ← b.3
   STA   $88
   LDA   $85
   STA   $89
   LDA   $82                  ; de-SSA Copy: i.2 ← %temp
   STA   $86                  ; (topo-sorted: read i.2 before overwriting)
   LDA   $83
   STA   $87
   JMP   .loop@0_start
.loop@0_break:
   LDA   $88                  ; return b.2 (via HARGS, the 2-byte return slot)
   STA   HARGS
   LDA   $89
   STA   HARGS+1

   ; epilogue
   CLC                        ; SSP = FP + 4 (rewind: M=0, N=2, +2 saved-FP)
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01                 ; restore caller FP from saved-FP slot
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   STX   FP
   RTS
```

A few things to notice:

- **No callee-saved overhead.** `local_bytes = 0` and there are
  no save/restore sequences, because this function has no `int*`-
  taken args, no function calls, and no values that span calls.
  Every regalloc-eligible variable went into caller-saved
  ($80-$BF) and there was nothing to preserve across.

- **All ZP loads use the fast addressing mode.** Every variable
  read in the loop body is a 3-cycle `LDA $XX`. With `--codegen`
  alone (no `--optimize`), every one would be the 8-cycle `LDY
  #off; LDA (FP),Y` sequence. The loop runs roughly 2.5× faster
  in cycles per iteration.

- **Slots are reused across non-interfering values.** ZP $82/$83
  holds `cond.1` first, then later holds the temp for `i + 1`.
  The interference graph said they're never live at the same
  point, so they can share.

- **Param `n` stays in the frame.** The two `LDA (FP),Y` reads
  inside the loop are param accesses — params arrive on the
  soft stack and `replace_pseudoregisters` keeps them there
  regardless of any color regalloc may have assigned. (Future
  work: copy hot params into ZP at the prologue.)

- **The de-SSA Copies sort correctly.** Look at the sequence at
  the bottom of the loop body: first `Copy(b.3, b.2)` (writing
  ZP $88/$89), then `Copy(%temp, i.2)`. The `i.2` read for the
  comparison happened at the TOP of the loop, well before this
  point — so overwriting i.2 here is safe. Topological sort on
  the parallel-copy set is what guarantees this.

---

## The same example through `--optimize-asm`

Same C source. The early stages (parse, resolve, type-check,
`c99_to_tac`) run identically. The fork begins INSIDE
`optimize_tac`: `--optimize-asm` calls it with
`do_regalloc=False`, so the TAC fixed-point loop and `from_ssa`
run, but TAC-level register allocation is skipped. The TAC
arriving at `tac_to_asm` has every local as a Pseudo with no
Coloring assigned.

### After `tac_to_asm` (`bare_exit=True`)

Same byte-level fan-out as the legacy path, but two differences:

- Each function ends with a bare `asm_ast.Return(save_a=False)`
  atom (vs the compound `Ret(arg_bytes=0, local_bytes=0,
  save_a=False)`). The save_a flag is False because Int returns
  larger than 1 byte (Int=2 bytes here) ride in HARGS — the
  staging Mov pair `STA HARGS / STA HARGS+1` doesn't go through
  A across the SSP/FP arithmetic.
- No `FunctionPrologue` is prepended. The frame setup decision
  is deferred to `prologue_synthesis` after register allocation
  has decided what (if anything) needs spilling.

Every multi-byte TAC operation fans out per-byte. The Long-style
`Binary(Add, b.2, a.1, b.3)` becomes:

```
Mov(Pseudo(b.2, 0), Reg(A))     ; load b.2 byte 0 into A
ClearCarry
Add(Pseudo(a.1, 0), Reg(A))     ; A += a.1 byte 0
Mov(Reg(A), Pseudo(b.3, 0))     ; store b.3 byte 0
Mov(Pseudo(b.2, 1), Reg(A))     ; load b.2 byte 1 (LDA only sets N/Z, C survives)
Add(Pseudo(a.1, 1), Reg(A))     ; A += a.1 byte 1 + carry from prior ADC
Mov(Reg(A), Pseudo(b.3, 1))     ; store b.3 byte 1
```

Every Pseudo references either offset 0 or offset 1 (Int = 2
bytes in c6502).

### After asm-level `to_ssa`

This is where the byte-level versioning happens. Each
`(Pseudo name, byte offset)` pair becomes its own SSA
variable, with its own Phis at iterated dominance frontiers.
The renamed Pseudo encodes the byte position in its name, with
`offset = 0`:

```
Pseudo("@2.b.2", 0)   →   Pseudo("@2.b.2.b0.v1", 0)
Pseudo("@2.b.2", 1)   →   Pseudo("@2.b.2.b1.v1", 0)
Pseudo("@3.i.2", 0)   →   Pseudo("@3.i.2.b0.v1", 0)
Pseudo("@3.i.2", 1)   →   Pseudo("@3.i.2.b1.v1", 0)
```

The TAC names like `@2.b.2` and `@3.i.2` come from
identifier_resolution's `@N.<orig>` rewrites (the `.2` is a TAC
SSA version stamped earlier, kept after TAC's `from_ssa`). The
`.b0.v1` / `.b1.v1` suffixes are asm-level SSA's own additions.

The loop top gets one Phi per `(name, offset)` pair, since the
loop merges the entry-edge initial values with the back-edge
post-iteration values. Both `b` and `i` are 2-byte Ints, so
that's 4 byte-level Phis at the loop top:

```
.loop_start:
  Phi(@2.b.2.b0.v2, [(.preheader, @2.b.2.b0.v1), (.continue, @2.b.2.b0.v3)])
  Phi(@2.b.2.b1.v2, [(.preheader, @2.b.2.b1.v1), (.continue, @2.b.2.b1.v3)])
  Phi(@3.i.2.b0.v2, [(.preheader, @3.i.2.b0.v1), (.continue, @3.i.2.b0.v3)])
  Phi(@3.i.2.b1.v2, [(.preheader, @3.i.2.b1.v1), (.continue, @3.i.2.b1.v3)])
  ...
```

`a.1` doesn't change inside the loop, so neither of its bytes
gets a Phi.

### After `byte_dce`

Every byte of every value is read at some point in this program
— `b` and `i` are both used in 2-byte arithmetic (`b + a`,
`i + 1`) and in the 2-byte signed comparison `i < n`. So
`byte_dce` finds nothing to drop here.

(For an example where it WOULD drop something: `(long)y`
followed downstream by `(int)y` truncating back. The
`SignExtend` lowering writes 4 bytes; only the low 2 are read;
bytes 2 and 3 of the Long are independently dead and removed,
shrinking M from 6 to 4 frame bytes on the typical case.)

### After `liveness + interference + color_graph`

Each byte-versioned name is a 1-byte interference graph node.
Inside the loop, this clique is mutually live:

- `a.1.b0`, `a.1.b1` (loop-invariant, read every iteration)
- `@2.b.2.b0.v2`, `@2.b.2.b1.v2` (Phi dsts at loop top)
- `@3.i.2.b0.v2`, `@3.i.2.b1.v2` (Phi dsts at loop top)

So those 6 nodes get 6 distinct colors. The actual assignments
the chordal-PEO greedy allocator picks (caller-saved pool first,
since none of these is cross-call):

| Name                      | Color |
|---------------------------|-------|
| `%0.1.b0.v1` (= a.1 byte 0) | `$87` |
| `%0.1.b1.v1` (= a.1 byte 1) | `$86` |
| `@2.b.2.{b0,b1}.{v1,v2,v3}` | `$83` / `$82` |
| `@3.i.2.{b0,b1}.v1`        | `$81` / `$80` |
| `@3.i.2.{b0,b1}.v2`        | `$85` / `$84` |
| `@3.i.2.{b0,b1}.v3`        | `$81` / `$80` |
| `%1.1.{b0,b1}.v1` (cond)   | `$81` / `$80` |
| `%3.1.{b0,b1}.v1` (i+1)    | `$81` / `$80` |

Two interesting placements:

- **Every version of `b.2` shares one ZP pair.** v1 (initial),
  v2 (Phi dst), v3 (back-edge `b + a` result) all live in
  `$83/$82`. The de-SSA Movs `Mov(b.v1, b.v2)` and
  `Mov(b.v3, b.v2)` after `apply_coloring` become
  `Mov(ZP($83), ZP($83))` self-Movs, which `asm_emit`'s
  peephole drops. The accumulator stays in place across
  iterations without explicit copy moves.

- **`cond.1`, `i+1` temp, and `i.v1` / `i.v3` all share
  `$81/$80`.** Their lifetimes don't overlap: `cond.1` dies
  when the conditional branch consumes it, the `i+1` temp
  lives only between increment and store-back, and `i.v3`
  lives only on the back-edge into `i.v2`. That's classic
  caller-saved slot reuse for short-lived values.

Total ZP byte usage: 8 bytes (`$80`–`$87`) = 4 pairs.

### After `apply_coloring`

Every Pseudo whose name is in `coloring.assignments` becomes
a `ZP(addr, 0)` operand. Param `n` (the only uncolored Pseudo
in this function) keeps its Pseudo form for
`replace_pseudoregisters` to lower to Frame addressing later.
Phi nodes still exist, but their `dst` and each
`args[i].source` are now ZP for the colored names.

### After `from_ssa`

Phi destruction inserts Movs on each predecessor edge. The
back-edge `.continue → .loop_start` ends with `JMP` (no
flag-sensitive terminator), so no critical-edge split — Movs
go directly before the JMP. Same for the entry-edge from
preheader.

The parallel-copy ordering uses storage keys, not SSA names,
so it sees through `apply_coloring`'s rewrite. For b's Phi:
each lowered Mov is `Mov(ZP($83), ZP($83))` (b.v1 → b.v2 on
the entry edge, b.v3 → b.v2 on the back edge) — a self-Mov
collapsed by `asm_emit`'s peephole. For i's Phi: each lowered
Mov is `Mov(ZP($81), ZP($85))` (and the byte-1 mate) — a real
LDA/STA pair.

### After `replace_pseudoregisters_bare_exit` + `prologue_synthesis`

Param `n` (the only remaining Pseudo) gets Frame addressing
at offset M+3=3, M+4=4 (M=0, so the param starts right after
the saved-FP slot at FP+3). Synthesis checks N=2 (param `n` is
2 bytes) and M=0 (no Frame locals) and no callee-saved bytes
— but `N > 0` so it still needs the saved-FP slot. It prepends
`FunctionPrologue(N=2, M=0, [])` and rewrites the bare
`Return(save_a=False)` to a full
`Ret(N=2, M=0, save_a=False, [])`.

### The actual 6502 assembly

Output of `compile.py - --codegen --optimize-asm` on the same
C source:

```
main:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   LDY   #$02
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

.main@asm_ssa_preheader@0:
.main@ssa_block@0:
   LDY   #$03                  ; load param n byte 0 from FP+3
   LDA   (FP),Y
   CLC
   ADC   #$01                  ; n + 1
   STA   $87                   ; a.1 byte 0 → ZP $87
   LDY   #$04                  ; load param n byte 1 from FP+4
   LDA   (FP),Y
   ADC   #$00
   STA   $86                   ; a.1 byte 1 → ZP $86
   LDA   #$00                  ; b.2.v1 = 0 (init)
   STA   $83                   ;   b byte 0 → ZP $83 (shared by all b versions)
   LDA   #$00
   STA   $82                   ;   b byte 1 → ZP $82
   LDA   #$00                  ; i.2.v1 = 0 (init)
   STA   $81                   ;   i.v1 byte 0 → ZP $81
   LDA   #$00
   STA   $80                   ;   i.v1 byte 1 → ZP $80
   LDA   $81                   ; entry-edge Phi Mov: i.v2.b0 ← i.v1.b0
   STA   $85                   ;   $81 (i.v1) → $85 (i.v2)
   LDA   $80                   ; entry-edge Phi Mov: i.v2.b1 ← i.v1.b1
   STA   $84                   ; (b's entry-edge Movs collapse to self-
                               ;  Movs at $83/$82 → dropped)
.loop@0_start:
   LDA   $85                   ; cond = i.2 < n  (signed compare)
   SEC
   LDY   #$03
   SBC   (FP),Y                ;   subtract n's low byte
   LDA   $84
   LDY   #$04
   SBC   (FP),Y                ;   subtract n's high byte (with borrow)
   BVC   .cmp_novf@0           ;   V-correction for signed overflow
.main@asm_ssa_block@0:
   EOR   #$80
.cmp_novf@0:
   BMI   .cmp_true@1           ;   if signed result negative → i < n
.main@asm_ssa_block@1:
   LDA   #$00
   JMP   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   STA   $81                   ; cond byte 0 → $81 (reuses i.v1's slot)
   LDA   #$00
   STA   $80                   ; cond byte 1 → $80
   LDA   $81                   ; if !cond goto .loop_break
   ORA   $80
   BEQ   .loop@0_break
.main@asm_ssa_block@2:
   LDA   $83                   ; b.3 = b.2 + a.1
   CLC
   ADC   $87
   STA   $83                   ;   b.3 byte 0 → $83 (overwrites b.2 in place)
   LDA   $82
   ADC   $86
   STA   $82                   ;   b.3 byte 1 → $82
.loop@0_continue:
   LDA   $85                   ; i+1 temp = i.2 + 1
   CLC
   ADC   #$01
   STA   $81                   ;   temp byte 0 → $81 (reuses cond's slot)
   LDA   $84
   ADC   #$00
   STA   $80
   LDA   $81                   ; back-edge Phi Mov: i.v2 ← i.v3
   STA   $85                   ;   $81 (i.v3) → $85 (i.v2)
   LDA   $80
   STA   $84
   JMP   .loop@0_start
.loop@0_break:
   LDA   $83                   ; return b.2 (via HARGS, the 2-byte return slot)
   STA   HARGS
   LDA   $82
   STA   HARGS+1

   ; epilogue
   CLC                         ; SSP = FP + 4 (rewind: M=0, N=2, +2 saved-FP)
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   LDY   #$02
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
```

A few things to notice:

- **Same prologue / epilogue shape as `--optimize`.** N=2, M=0,
  no callee-saved bytes — synthesis decided a frame is still
  needed (N>0 means params arrive on the soft stack and need
  FP-relative addressing) but there's no extra ZP-byte
  preservation work. Identical structurally to what
  `--optimize` emits for the same case.

- **Fewer ZP bytes used than `--optimize`.** This run uses 8
  bytes (`$80`–`$87`) vs `--optimize`'s 10 bytes (`$82`–`$8B`).
  The difference is `b.2` and `b.3`: `--optimize` colors them
  separately ($88/$89 vs $84/$85); `--optimize-asm` colors
  every version of `@2.b.2` to one pair ($83/$82) because
  the byte-versioned SSA exposes that they're the same
  value chain across iterations and the collapsed Phi Movs
  let them physically share a slot. That's coalescing-by-
  accident, riding on the apply_coloring + storage-key
  parallel-copy interaction.

- **One extra synthetic block label.** The
  `.main@asm_ssa_preheader@0:` line is minted by
  `_maybe_prepend_preheader` so the loop top has a real
  labeled predecessor on the entry edge for Phi
  destruction. It carries no instructions and no flow
  effect — it's just a marker that the assembler resolves
  to the same address as the immediately following block.

- **Different Mov placements at the loop edges.** The
  entry-edge Phi Movs `LDA $81; STA $85; LDA $80; STA $84`
  appear at the END of the entry block (just before flow
  reaches the loop test); the back-edge Phi Movs appear
  inside `.loop@0_continue` just before the `JMP
  .loop@0_start`. The corresponding `--optimize` output
  inlines its de-SSA Copies in the same positions, just
  with different ZP slots picked.

---

## Inspecting intermediate results

Several `compile.py` flags let you peek at what the optimizer is
doing:

- `--tac` shows the TAC right after `c99_to_tac` (no optimization).
- `--tac --optimize` shows the TAC AFTER the full optimizer
  pipeline (post-de-SSA).
- `--tac --optimize-asm` shows the TAC after the alt path's TAC
  opts (regalloc skipped, otherwise identical to `--optimize`).
- `--codegen` shows the final 6502 asm (no optimization).
- `--codegen --optimize` shows the final asm with regalloc applied.
- `--codegen --optimize-asm` shows the final asm from the alt
  pipeline (asm-SSA round-trip; no regalloc until step 7 lands).

For digging deeper into individual phases, the test files are the
best examples. See `tests/test_ssa.py`, `tests/test_liveness.py`,
`tests/test_interference.py`, `tests/test_register_allocation.py`,
`tests/test_constant_folding.py`, etc.

---

## What's not done yet

These are documented as future work in the codebase:

- **Move coalescing.** When a Phi's source and dst could share a
  color, regalloc doesn't actively try to make that happen. We
  rely on the self-Mov peephole to clean up the chance pairings.
  Real coalescing during coloring would be smarter.
- **Global value numbering.** Two computations of the same
  expression aren't deduplicated.
- **Loop optimizations.** Loop-invariant code motion, strength
  reduction, etc.
- **Inlining.** Every function call is a real call.
- **Smarter spill heuristics.** When a variable spills, we don't
  re-color the function — we just send the spilled value to
  frame storage forever.
- **Variable-width-aware optimal coloring.** Greedy coloring with
  variable widths is no longer provably optimal (only chordal-
  with-unit-widths is). It works well in practice, but a more
  principled allocator could do better in fragmented cases.

---

## Files at a glance

| File | What it does |
|------|--------------|
| `passes/optimization/optimizer.py` | Driver. Glues everything together. |
| `passes/optimization/cfg.py` | Basic blocks, dominators, dominance frontiers. |
| `passes/optimization/var_visit.py` | Shared use/def walkers (pure structural). |
| `passes/optimization/ssa_construction.py` | `to_ssa` — places Phis, renames. |
| `passes/optimization/constant_folding.py` | Compile-time arithmetic. |
| `passes/optimization/unreachable_code_elimination.py` | UCE — drops dead blocks/edges/labels. |
| `passes/optimization/copy_propagation.py` | SSA-aware copy-substitution. |
| `passes/optimization/dead_store_elimination.py` | SSA-aware DSE. |
| `passes/optimization/liveness.py` | Per-block + per-instruction liveness. |
| `passes/optimization/interference.py` | Builds the chordal interference graph. |
| `passes/optimization/pool.py` | Caller/callee-saved ZP pool config. |
| `passes/optimization/register_allocation.py` | Width-aware chordal coloring. |
| `passes/optimization/ssa_destruction.py` | `from_ssa` — Phis to Copies, topo-sorted, cycles broken. |
| `passes/replace_pseudoregisters.py` | Lays out the frame; consumes the Coloring. |
| `asm_emit.py` | 6502 mnemonic emission, including prologue save/restore. |

---

## The `--optimize-asm` alternate pipeline

`--optimize-asm` selects an alternate optimization path that runs
TAC-level fixed-point opts but defers register allocation to an
asm-level layer. The motivation: doing optimization at the asm IR
exposes byte-granular structure that's invisible at TAC, and a
late, asm-aware regalloc can color individual bytes (rather than
whole multi-byte values like the TAC-level allocator does today).

Status: byte-level DCE and byte-granular regalloc are now both
in place. `--optimize-asm` is sim-correct on the chapter_1..12
corpus and produces output that places live values in ZP slots
the same way `--optimize` does — with the bonus that byte-level
DCE drops dead high-byte work that the TAC-level pipeline can't
see (e.g. a `(long)y` cast whose result is later truncated back
to int).

### How it differs from `--optimize`

```
--optimize               : c99_to_tac → optimize_tac (incl. regalloc)
                           → tac_to_asm → replace_pseudoregisters
                           → expand_long_branches → asm_to_asm2
                           → asm_emit

--optimize-asm           : c99_to_tac → optimize_tac (NO regalloc)
                           → tac_to_asm bare_exit=True
                           → asm_opt.optimize_program (asm-SSA round-trip)
                           → replace_pseudoregisters_bare_exit
                           → prologue_synthesis
                           → expand_long_branches → asm_to_asm2
                           → asm_emit
```

Three architectural pieces are new under `--optimize-asm`:

**1. Bare-exit emission from `tac_to_asm`.** With `bare_exit=True`,
phase 9 emits an atomic `asm_ast.Return(save_a)` at each function
exit instead of the compound `Ret(arg_bytes, local_bytes, save_a,
callee_saved_addrs)`. The matching `FunctionPrologue` is *not*
prepended either. The asm tree leaving phase 9 has no SSP/FP
arithmetic — just the body that staged the return value (in A or
HARGS per the calling convention) plus a bare exit marker.

**2. `replace_pseudoregisters_bare_exit`.** Same Pseudo → Frame /
ZP / Data rewriting as the regular pass, but skips the prologue
prepend and the `Ret(...)` payload patches. Returns the asm
program alongside a per-function `dict[name, FrameDims]` carrying
the metrics (arg_bytes, local_bytes, callee_saved_addrs) that the
synthesis pass later consumes.

**3. `prologue_synthesis` (`passes/prologue_synthesis.py`).** Takes
the bare-exit program plus the frame dims, and either:

| Condition | Result |
|---|---|
| `arg_bytes == 0` AND `local_bytes == 0` AND no callee-saved bytes | Leave the bare `Return(save_a)` atoms in place. No `FunctionPrologue` prepended. |
| Otherwise | Prepend `FunctionPrologue(N, M, callee_saved_addrs)`; rewrite each `Return(save_a)` to `Ret(N, M, save_a, callee_saved_addrs)`. |

The frame-less case is byte-equivalent to `--optimize`'s current
collapsed RTS, but the asm tree is now self-describing: any
later pass can see "this function has no frame" by looking at the
IR directly (no need to consult `FunctionPrologue.arg_bytes ==
0`).

### Asm-level SSA (passes/optimization_asm/)

Between `tac_to_asm bare_exit=True` and
`replace_pseudoregisters_bare_exit`, the alt pipeline runs an
asm-level SSA round-trip on the Pseudo-bearing IR. Today (step 5e
of the build plan) it's a no-op round-trip — `to_ssa` →
`from_ssa` with no opts in between. Steps 6 (byte-level DCE,
peepholes) and 7 (byte-granular regalloc) will populate the
sandwich.

The interesting design choice: asm-level SSA versions each
`(Pseudo name, byte offset)` pair *independently*. Byte 0 of a
4-byte Long has its own def/use chain separate from byte 1.
That's what lets a future byte-DCE pass drop the high-byte work
when the value's used only in its low byte (e.g. a Long that fits
in an Int).

```
to_ssa(fn, statics):
  → CFG construction (passes/optimization_asm/cfg.py)
  → ensure every reachable block has a leading Label (for
    Phi pred_label tagging)
  → identify excluded names: address-taken (LoadAddress.src),
    read-modify-write targets (Inc/Dec/ASL/LSR/ROL/ROR.dst),
    and static-storage names (passed in via `statics`)
  → identify promotable (name, offset) pairs (everything else)
  → place pruned Phis at iterated dominance frontiers
  → rename: each def of (name, offset) mints a fresh
    `<name>.b<offset>.v<N>` Pseudo with offset=0
```

The renaming convention encodes the byte position into the new
Pseudo's name (with `offset=0`), so each byte becomes a 1-byte
virtual variable. After SSA destruction the renamed names persist;
`replace_pseudoregisters`'s default 1-byte size for unknown names
gives each its own Frame slot. Multi-byte values get split across
non-contiguous frame bytes — fine for byte-level access (the
operand carries the address; no instruction relies on adjacency)
but obviously broken for `&x` access, which is why address-taken
names are excluded from versioning.

### `from_ssa` and critical-edge splitting

Asm-level SSA destruction has one significant difference from the
TAC version: it has to be careful about flag-clobbering Movs.

The standard "Phi → Mov in predecessor before terminator" pattern
breaks at the asm IR because predecessors can end with a flag-
sensitive `Branch` (BCC / BEQ / BMI / etc.). An LDA (the Mov's
load) sets N and Z, so inserting it between a flag-setting
instruction (an ORA, CMP, ADC etc.) and the Branch that reads its
flags clobbers the flag. The original TAC version doesn't have
this issue — TAC's `JumpIfTrue` / `JumpIfFalse` carry the
condition value as an operand, not via flags.

The fix is **critical-edge splitting**: for each edge from a
`Branch`-terminated predecessor to a Phi-bearing merge, insert a
fresh block on the edge:

| Edge type | Layout |
|---|---|
| Branch's TAKEN target is a Phi merge | New block appended to the function: `Label split, Movs, Jump merge`. Branch's target rewritten to `split`. |
| Branch's FALL-THROUGH target is a Phi merge | New block inserted in source order between the Branch's block and the merge: `Label split, Movs`. Falls through to merge naturally — no Jump needed. |
| Jump-terminated predecessor | No split. Movs go directly before the Jump (JMP doesn't read flags). |

Each affected `Phi.args[i].pred_label` is rewritten from the old
predecessor's leading label to the split block's label, so the
destruction step's label-to-block lookup finds the new block.

Parallel-copy ordering and cycle-breaking work the same as in the
TAC version (topological sort; mint a fresh
`.<funcname>@asm_cycle_tmp@<N>` Pseudo to break a cycle).

### Byte-level DCE (`passes/optimization_asm/byte_dce.py`)

Runs between `to_ssa` and `from_ssa`. Drops `Mov` and `Phi`
instructions whose Pseudo dst is never used as a source anywhere
else in the function. Iterates to a fixed point so dropping one
Mov can free its source's def for removal next round.

What it catches that TAC-level DSE doesn't: byte-level dead
writes. After asm-SSA versioning, byte 0 of a value and byte 3
of the same value are independent variables. A common case:
`(long)y` followed downstream by `(int)y` — the SignExtend writes
all 4 bytes of the Long, but only the low 2 bytes are read. Bytes
2 and 3 are independently dead and droppable. The `_translate_
sign_extend` helper produces a sequence ending in `LDA #$00 / LDA
#$FF; STA byte_2; STA byte_3`; byte-DCE removes the two STAs (and
the synthesis pass shrinks the frame accordingly — local_bytes
goes from 6 to 4 on a typical Int → Long → Int round trip).

Conservative by design — only `Mov` and `Phi` defs are
considered. `LoadAddress` stays even with an unused dst (so its
src Pseudo's frame slot still gets allocated by
`replace_pseudoregisters`); `Pop` stays (stack side effect);
`Add/Sub/And/Or/Xor` with Pseudo dsts (rare in current
emissions) stay because they double as carry-flag and N/Z
producers. Statics are never dropped — writes to file-scope
globals are observable to other functions.

### Asm-level regalloc (`passes/optimization_asm/{liveness,interference,regalloc}.py`)

Byte-granular, runs while still in SSA form. Mirrors the TAC
register allocator's structure but on the asm CFG and with each
node 1 byte wide (since asm-SSA already split multi-byte values).

```
liveness        — backward dataflow over Pseudo names. Phi
                  sources are predecessor-edge contributions;
                  Phi dsts are killed at block entry.
interference    — chordal graph; nodes are colorable Pseudo
                  names (= byte-versioned SSA names that aren't
                  in any of the excluded sets).
regalloc        — PEO + greedy fit. Cross-call values prefer
                  callee-saved ($C0-$FF); others prefer caller-
                  saved ($80-$BF). Spills land in `Coloring.
                  spilled` and fall through to Frame allocation
                  in `replace_pseudoregisters_bare_exit`.
```

The colorable-name filter in `interference.py` excludes:
* statics (file-scope storage)
* address-taken (`LoadAddress.src`)
* params (calling convention dictates Frame addressing)
* RMW targets (`Inc/Dec/ASL/LSR/ROL/ROR.dst` — defensive)
* any name with non-zero-offset Pseudo references (= unversioned
  multi-byte name, not eligible for 1-byte coloring)

### Apply-coloring + `from_ssa` cycle hazard

A subtle interaction: when two SSA-distinct names get assigned
the same physical ZP slot via coloring, the Phi destruction's
parallel-copy ordering must detect that as a cycle even though
the names are different. The original TAC `from_ssa` cycle check
compared by SSA name and missed this case.

Fix: between regalloc and `from_ssa`, run `apply_coloring`, which
substitutes every `Pseudo(name, offset)` whose name is in
`coloring.assignments` with the corresponding `ZP(addr+offset, 0)`
operand. After this rewrite, Phi destruction sees Movs whose
sources and dsts are ZP operands, and its cycle detector compares
by physical-storage key (`Pseudo` name+offset for unrenamed
Pseudos, `ZP` address for renamed ones, `Reg` kind, etc.). A
2-cycle at the ZP level — like `Mov ZP($A) → ZP($B)` paired with
`Mov ZP($B) → ZP($A)` — is correctly broken via a fresh
`.<funcname>@asm_cycle_tmp@<N>` Pseudo.

### What's done at the asm-opt layer

| Step | Status |
|---|---|
| 1. `--optimize-asm` flag plumbing | Done |
| 2. `Return(save_a)` atom in `asm_ast` | Done |
| 3. `replace_program_bare_exit` + `prologue_synthesis` | Done |
| 4. Synthesis collapses M=0/S=0 to bare RTS | Done |
| 5a. Asm-level CFG + dominance | Done |
| 5b. Same | Done |
| 5c. `to_ssa` (Phi placement + byte-granular renaming) | Done |
| 5d. `from_ssa` (Phi → Mov, critical-edge splitting, parallel-copy ordering by storage key, cycle break) | Done |
| 5e. Wire round-trip into `--optimize-asm`; chapter sim corpus passes | Done |
| 6. Byte-level DCE (drops dead high-byte work) | Done |
| 7. Asm-level byte-granular regalloc | Done |
| F1a. Parser support for `__attribute__((zp_abi))` | Done |
| F1b. `passes/abi_selection.py` — `ParamLayout` + validation | Done |
| F2. `tac_to_asm` ZP-ABI call-site lowering | Done |
| F3. `replace_pseudoregisters_bare_exit` resolves ZP-ABI params to ZP | Done |
| F4. Frame elimination on ZP-ABI leaf functions | Done |
| F5. Caller's body locals avoid ZP-ABI callee param addresses | Done (via `_blocked_addrs_for`) |
| F6. End-to-end wiring + chapter sim corpus + new ZP-ABI sim tests | Done |

### Files at a glance (asm-opt layer)

| File | What it does |
|------|--------------|
| `passes/prologue_synthesis.py` | Inserts `FunctionPrologue` and rewrites `Return` → `Ret` based on per-function `FrameDims`; collapses no-frame functions to bare RTS. |
| `passes/abi_selection.py` | Picks per-function `ParamLayout` from the `__attribute__((zp_abi))` annotation; validates leaf + not-address-taken + fits-in-window. |
| `passes/optimization_asm/cfg.py` | Asm-level basic-block CFG, idom, dominance frontiers — same shape as the TAC version. |
| `passes/optimization_asm/ssa_construction.py` | `to_ssa(fn, statics=...)` — byte-granular Phi placement + name versioning. |
| `passes/optimization_asm/byte_dce.py` | Drops Movs / Phis whose Pseudo dst is unused. |
| `passes/optimization_asm/liveness.py` | Backward dataflow over Pseudo names. |
| `passes/optimization_asm/interference.py` | Chordal graph; filters non-colorable names. |
| `passes/optimization_asm/regalloc.py` | PEO + greedy 1-byte coloring; accepts `blocked_addrs` to reserve ZP-ABI param slots. |
| `passes/optimization_asm/apply_coloring.py` | Substitutes colored `Pseudo` operands with `ZP` operands so `from_ssa` can detect physical-slot cycles. |
| `passes/optimization_asm/ssa_destruction.py` | `from_ssa(fn)` — critical-edge splitting + Phi → Mov + parallel-copy ordering by storage key. |
| `passes/optimization_asm/optimizer.py` | Per-function driver; threads to_ssa → byte_dce → regalloc → apply_coloring → from_ssa. Computes `blocked_addrs` from each function's own ParamLayout + every ZP-ABI callee's. |

### Frame elimination via `__attribute__((zp_abi))`

A separate optimization layered on top of `--optimize-asm`: any
function declared with `__attribute__((zp_abi))` participates in
the **per-function ZP-passing calling convention**. The function's
parameters live at fixed zero-page addresses chosen by
`passes/abi_selection.py`; the caller writes argument bytes
directly to those addresses (no `AllocateStack`, no `Stack(off)`
writes); the callee reads them via the same addresses (no
`Frame(M+3+...)` reads). When the callee also has no Frame-
resident locals (asm-level regalloc fit everything in ZP) and no
callee-saved bytes (which leaves don't need anyway), the
prologue / epilogue collapse entirely — the function emits as
pure body + bare `RTS`.

Constraints (validated at compile time, error otherwise):

- The function's body must contain no `FunctionCall` /
  `IndirectCall` (leaf only — see `docs/leaf_zp_abi.md` for the
  reasoning around recursion and indirect calls).
- The function's address must not be taken anywhere in the
  program.
- The total parameter byte count must fit in the available ZP
  window (default 64 bytes, $80–$BF).

When validation fails, the compiler reports the specific check
that failed with a clear error — the annotation has to be
correct, no silent fallback.

The annotation syntax is `__attribute__((zp_abi))` placed before
the declaration specifiers:

```c
__attribute__((zp_abi)) int add(int a, int b) {
    return a + b;
}
```

Compiled with `--codegen --optimize-asm`, `add` produces:

```
add:
   SUBROUTINE
.add@asm_ssa_block@0:
   LDA   $80          ; a's low byte (caller wrote it here)
   CLC
   ADC   $82          ; + b's low byte
   STA   $85          ; result's low byte (regalloc avoids $80-$83)
   LDA   $81
   ADC   $83
   STA   $84
   LDA   $85
   STA   HARGS
   LDA   $84
   STA   HARGS+1
   RTS                ; bare RTS — no SSP/FP teardown
```

Compare to the legacy soft-stack ABI (the `--optimize` form),
which emits ~17 instructions of prologue + matching epilogue
even for this 7-instruction function. The savings stack with
the existing asm-level optimizations: byte-DCE still trims dead
high-byte writes, byte-granular regalloc still picks tight
colors, and the absence of frame ceremony is on top.

A function calling a ZP-ABI callee gets a corresponding
adjustment: its own body regalloc avoids the callee's param ZP
addresses (see `_blocked_addrs_for` in
`passes/optimization_asm/optimizer.py`), so locals can't be
placed where outgoing-arg writes will land. This is more
conservative than a per-instruction outgoing-arg-window
liveness analysis — the addresses are blocked for the function's
entire body — but it's simple and correct.

The full design and build plan is in `docs/leaf_zp_abi.md`.
