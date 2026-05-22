# passes/optimization/framework/CLAUDE.md

Pass infrastructure for the TAC-level optimizer. Provides the `Pass`
ABC, six per-function phase ABCs, a `PhaseDriver` that owns SSA
construction and destruction, an LLVM-style pattern combinator DSL,
and four specialised base classes (`WindowPass`, `DefUsePass`,
`OperandRewritePass`, `SinkPass`) that cover the common shapes. Import
from the package root: `from passes.optimization.framework import
PhaseDriver, WindowPass, m_Binary, …`.

## Module roster

- `base.py` — `Pass` ABC + `PassContext` dataclass. Bottom of the type
  hierarchy; every pass class (however specialised) ultimately inherits
  `Pass`.
- `phases.py` — six per-function phase ABCs (`PreSsaPass`,
  `PreFixedpointPass`, `FixedpointPass`, `PostFixedpointPass`,
  `PostDestructionPass`, `PostDestructionFixedpointPass`) plus
  `ProgramPass` (whole-program variant).
- `driver.py` — `PhaseDriver` dataclass. Accepts a list of passes per
  phase, owns SSA construction/destruction, runs the fixedpoint loop.
- `patterns.py` — LLVM-PatternMatch-style combinator DSL: `m_Any`,
  `m_Constant`, `m_Var`, `m_Specific`, `m_Binary`, `m_Unary`,
  `m_Cast`, `m_Copy`, `m_Load`, `m_Store`, `m_JumpIfTrue`,
  `m_JumpIfFalse`, `m_OneOf`, `m_Commutative`, plus `MatchResult`,
  `Pattern`, and the `match` entry point.
- `window.py` — `WindowPass`: adjacency-based N-instruction peephole
  base. Slides a fixed-size window over `fn.instructions`; calls
  `rewrite` on every matching position.
- `defuse.py` — `DefUsePass`, `DefUseEnv`, `Rewrite`: SSA def-use
  chain peephole base. Walks every instruction as a potential USE site,
  lets the subclass walk back to the SSA def via `env.def_of(var)`.
- `operand_rewrite.py` — `OperandRewritePass`: USE-position operand
  rewrite base. Visits every operand-position `Type_val`; returns a
  replacement or None.
- `sink.py` — `SinkPass`: single-instruction forward move base. Finds
  a target index for a matching instruction and slides it forward.
- `__init__.py` — re-exports all public names from the submodules
  (everything in `__all__`) so callers can `from
  passes.optimization.framework import …` without knowing the
  submodule layout.

## Pass + PassContext

`Pass` (in `base.py`) is the abstract base for every optimizer pass:

```python
class Pass(abc.ABC):
    name: ClassVar[str]

    @abc.abstractmethod
    def run(self, fn: tac_ast.Function, ctx: PassContext) -> tac_ast.Function: ...
```

Every pass is a pure function on `tac_ast.Function`; it receives a
`PassContext` and returns the (possibly new) function. The `name`
class variable is a human-readable identifier used in debug output.

`PassContext` carries the two pieces of shared state all passes may
inspect:

```python
@dataclass
class PassContext:
    symbols: object | None = None   # SymbolTable; None in synthetic tests
    ssa_dsts: set[str] | None = None  # populated by to_ssa
```

- `symbols` is the type checker's `SymbolTable`. Passes that need type
  information (width queries, const-static checks) read it here.
  Legacy unit tests that call `optimize_function` on synthetic
  `Function` objects without a symbol table leave it `None`, and every
  pass that needs it must handle that case gracefully.
- `ssa_dsts` is the set of SSA-renamed Var names populated by `to_ssa`
  at the start of the SSA-form pipeline. Passes that rely on the
  single-def guarantee (def-use walks, Rewrite.drop_defs) gate on
  this being non-None.

`PassContext` is extensible-but-additive: add a new field with a
default, no driver change needed. Removing or renaming a field is a
breaking change; prefer None-default additions.

## Phase ABCs

All six live in `phases.py` and inherit `Pass`. They carry no
additional interface — subclassing one is a pure declaration of *when*
the pass runs.

| ABC | When |
|-----|------|
| `PreSsaPass` | One-shot, before `to_ssa`. Operates on pre-SSA TAC. |
| `PreFixedpointPass` | One-shot, after `to_ssa`, before the main fixedpoint loop. |
| `FixedpointPass` | Member of the main SSA-form fixedpoint loop. |
| `PostFixedpointPass` | One-shot, after the fixedpoint converges, before `from_ssa`. |
| `PostDestructionPass` | One-shot, after `from_ssa`. Operates on post-SSA TAC. |
| `PostDestructionFixedpointPass` | Member of the post-destruction fixedpoint loop. |

`ProgramPass` is the odd one out: it operates on a whole
`tac_ast.Program` rather than a single function. Its `run` method
raises `TypeError` — subclasses implement `run_program(prog, ctx)`
instead. The driver does NOT call `ProgramPass` instances; callers
invoke `run_program` directly (see `dispatch_pointer_array.py`).

`WindowPass`, `DefUsePass`, `OperandRewritePass`, and `SinkPass` all
inherit `FixedpointPass`. For a phase other than `FixedpointPass`, host
a module-level instance and delegate from a phase-specific subclass
(see "Phase-mismatch trick" in the WindowPass section).

## PhaseDriver

`PhaseDriver` (in `driver.py`) is a `@dataclass` that accepts six
ordered sequences of passes and runs them over a single
`tac_ast.Function`:

```python
@dataclass
class PhaseDriver:
    pre_ssa: Sequence[PreSsaPass]
    pre_fixedpoint: Sequence[PreFixedpointPass]
    fixedpoint: Sequence[FixedpointPass]
    post_fixedpoint: Sequence[PostFixedpointPass]
    post_destruction: Sequence[PostDestructionPass]
    post_destruction_fixedpoint: Sequence[PostDestructionFixedpointPass]
    fixedpoint_cap: int = 256
```

`__post_init__` enforces phase membership at construction time:
passing a `FixedpointPass` in `pre_ssa` raises `TypeError` immediately.
This catches misplaced passes in tests and during driver setup.

`apply(fn, ctx)` runs the pipeline:

1. All `pre_ssa` passes (always, even without `ctx.symbols`).
2. If `ctx.symbols is not None`: call `to_ssa(fn, symbols)`, store the
   resulting `ssa_dsts` on `ctx`, run all `pre_fixedpoint` passes.
3. Main fixedpoint: rotate through all `fixedpoint` passes until the
   function is structurally unchanged (dataclass `__eq__`) or
   `fixedpoint_cap` is reached.
4. If `ctx.symbols is not None`: run all `post_fixedpoint` passes,
   then call `from_ssa(fn, symbols)`.
5. All `post_destruction` passes (one-shots).
6. Post-destruction fixedpoint: same convergence check against
   `post_destruction_fixedpoint` passes.

When `ctx.symbols is None` (legacy synthetic tests), steps 2 and 4 are
skipped entirely — `pre_fixedpoint` and `post_fixedpoint` passes don't
run, SSA-aware passes in the fixedpoint loop become no-ops.

The module-level `_DRIVER` instance in `optimizer.py` is constructed
once and reused across all `optimize_function` calls.

## Pattern DSL (`patterns.py`)

Modelled after LLVM's `PatternMatch.h`. The top-level entry point is:

```python
result = match(pattern, node)  # -> MatchResult | None
```

Returns a `MatchResult` whose `.bindings` dict holds captured values
on success, or `None` on failure.

`MatchResult` has `snapshot()` / `restore(snap)` methods. Compound
patterns call `snapshot` before matching children and `restore` on
failure, so a partial sub-match never contaminates the bindings dict.

Operand / value patterns:

| Pattern | Matches |
|---------|---------|
| `m_Any(capture=…)` | Anything. Optionally captures the node. |
| `m_Constant(value=…, variant=…, capture=…)` | `tac_ast.Constant`. Optionally constrains the underlying value or const class (e.g. `tac_ast.ConstInt`). |
| `m_Var(name=…, capture=…)` | `tac_ast.Var`. Optionally constrains by name (rarely needed directly — use `m_Specific` for backrefs). |
| `m_Specific(name)` | A node structurally equal (`__eq__`) to whatever was bound to `name` earlier. Fails if `name` wasn't bound. Used to require the same Var appears in two positions. |

Instruction patterns:

| Pattern | Matches |
|---------|---------|
| `m_Binary(op=…, src1=…, src2=…, dst=…, capture=…)` | `tac_ast.Binary`. `op` accepts a class or tuple. Child patterns default to `m_Any`. |
| `m_Unary(op=…, src=…, dst=…, capture=…)` | `tac_ast.Unary`. Same shape. |
| `m_Copy(src=…, dst=…, capture=…)` | `tac_ast.Copy`. |
| `m_Cast(kind=…, src=…, dst=…, capture=…)` | Any of SignExtend / ZeroExtend / Truncate / IntToFloat / IntToDouble / FloatToInt / DoubleToInt / FloatToDouble / DoubleToFloat. `kind` narrows to a subset. |
| `m_JumpIfTrue(condition=…, target=…, capture=…)` | `tac_ast.JumpIfTrue`. |
| `m_JumpIfFalse(condition=…, target=…, capture=…)` | `tac_ast.JumpIfFalse`. |
| `m_Store(src=…, dst_ptr=…, capture=…)` | `tac_ast.Store`. |
| `m_Load(src_ptr=…, dst=…, capture=…)` | `tac_ast.Load`. |

Combinators:

| Pattern | Behaviour |
|---------|-----------|
| `m_OneOf(*alternatives)` | Try each alternative; first match wins. Restores bindings between attempts. |
| `m_Commutative(op, lhs, rhs, dst=…, capture=…)` | Match a Binary in either `(src1=lhs, src2=rhs)` or `(src1=rhs, src2=lhs)` order. Tries direct order first; falls back to swapped. |

Typical usage (from `lnot_jump_fold.py`):

```python
pattern = [
    m_Unary(op=tac_ast.LogicalNot, dst=m_Var(capture='not_dst')),
    m_OneOf(
        m_JumpIfTrue(condition=m_Specific('not_dst'), capture='jmp'),
        m_JumpIfFalse(condition=m_Specific('not_dst'), capture='jmp'),
    ),
]
```

`m_Specific('not_dst')` ensures the JumpIf's condition is exactly
the same Var that the Unary wrote — a backref that `m_Var` alone
can't express cleanly.

## WindowPass

Use when: your rewrite inspects N consecutive instructions in a linear
scan and replaces them with M instructions (0 = delete, 1..k =
substitute).

Subclass interface:

```python
class MyPeephole(WindowPass):
    name = "my_peephole"
    window_size = 2              # ClassVar[int]
    pattern = [pat0, pat1]      # ClassVar[Pattern | Sequence[Pattern]]

    def prepare(self, fn, ctx):  # optional; runs once per run() call
        return some_whole_fn_analysis(fn)

    def rewrite(self, m, prep, ctx) -> list | None:
        # m.bindings holds captures.
        # Return replacement list (possibly empty), or None to decline.
        ...
```

- `window_size = 1`: `pattern` is a single `Pattern` (or a
  1-element sequence).
- `window_size > 1`: `pattern` is a sequence of length `window_size`;
  `pattern[k]` matches `instrs[i+k]`.
- Iteration: on a successful rewrite (non-None return), advances by
  `window_size`. On no-match or declined rewrite (None return),
  advances by 1. The output is not re-matched in the same pass.

`WindowPass` inherits `FixedpointPass`, so instances go in the
`fixedpoint` slot. For a post-destruction peephole, use the
**phase-mismatch trick**: host a module-level instance and delegate:

```python
_IMPL = MyPeephole()

class MyPeepholePostDestruction(PostDestructionFixedpointPass):
    name = "my_peephole"
    def run(self, fn, ctx):
        return _IMPL.run(fn, ctx)
```

(The `FoldShortCircuitJump` pattern is slightly different — it inherits
`PostDestructionFixedpointPass` directly and implements `run` by hand,
because its multi-block shape doesn't fit a linear window. But for true
window peepholes the delegation trick is idiomatic.)

## DefUsePass

Use when: your rewrite looks at a USE instruction and needs to walk
back through the SSA def chain to inspect the producer, then
optionally atomically drop one or more now-dead intermediate defs.

Subclass interface:

```python
class MyFold(DefUsePass):
    name = "my_fold"
    pattern = m_Cast(kind=tac_ast.Truncate, src=m_Var(capture='src'))

    def prepare_extra(self, fn, ctx):  # optional; stored at env.extra
        return count_uses(fn)

    def rewrite(self, m, env, ctx) -> Type_instruction | Rewrite | None:
        src_var = m.bindings['src']
        producer = env.def_of(src_var)   # walks back via def_idx
        if not isinstance(producer, tac_ast.ZeroExtend):
            return None
        # Return a new instruction (replace the match) or a Rewrite
        # (replace + atomically drop defs).
        return tac_ast.Copy(src=producer.src, dst=m.bindings['dst'])
```

Key points:

- `pattern` matches the USE site (the instruction being inspected).
  Capture any Var whose def you want to walk back through.
- `env.def_of(var)` looks up `var.name` in the function's def-index
  and returns the defining instruction, or None. Sound only for
  SSA-renamed names (single def guaranteed).
- Gate on `ctx.ssa_dsts is None or var.name not in ctx.ssa_dsts` to
  skip non-SSA names safely. The pass itself is a no-op when
  `ctx.ssa_dsts is None`.
- `prepare_extra(fn, ctx)` runs once per `run` call; its return value
  is stored at `env.extra`. Use it for whole-function analyses
  (use-count dicts, etc.).
- Return type: a plain instruction (replace the matched USE), a
  `Rewrite(replacement, drop_defs=(...))` (replace AND atomically drop
  the named intermediate defs in the same rebuild), or None to leave
  the instruction unchanged.

**`instructions_view` mechanism**: `DefUsePass.run` maintains a mutable
`instructions_view` list alongside `fn.instructions`. Every successful
rewrite updates `instructions_view[i]` immediately, so a subsequent
`env.def_of` call within the same `run` sees the already-rewritten
form. This is load-bearing for `ReassocConstants` (chained
`reassoc_const` rewrites that fold A→B then B→C in a single pass).

**Atomic eager drops via `Rewrite`**: when `drop_defs` contains Vars
whose defining instructions should be deleted alongside the USE
replacement, `DefUsePass` resolves each to its def index and removes
those instructions in the final rebuild pass. If two rewrites in the
same run would drop the same index, the later one is skipped (conflict
guard) to avoid double-drop. Leftover dead defs are cleaned up by DSE
on the next fixedpoint iteration.

## OperandRewritePass

Use when: your rewrite substitutes individual operand VALUES (USE
positions) without changing the instruction's opcode or arity —
effectively a per-operand find-and-replace.

Subclass interface:

```python
class MyOperandFold(OperandRewritePass):
    name = "my_fold"
    operand_pattern = m_Var(capture='var')

    def prepare(self, fn, ctx):     # optional
        return build_substitution_map(fn, ctx)

    def rewrite_operand(self, m, prep, ctx) -> Type_val | None:
        var = m.bindings['var']
        return prep.get(var.name)   # Constant or None
```

`run` calls `rewrite_uses_in(instr, mapper)` for each instruction,
where `mapper` applies the pattern and returns the replacement or the
original operand. Only USE-position `Type_val` fields are visited; DEF
positions (instruction `dst`) are untouched.

The `FoldStaticConstReads` pass uses the delegation pattern here too:
the `OperandRewritePass` subclass `_FoldStaticConstReadsImpl` is a
private module-level instance, and the `PreFixedpointPass` subclass
`FoldStaticConstReads` delegates its `run` to it.

## SinkPass

Use when: your pass moves a single instruction forward in the
instruction list — the instruction is not rewritten, just relocated.

Subclass interface:

```python
class MySink(SinkPass):
    name = "my_sink"
    pattern = m_Binary(op=tac_ast.Add, src2=m_Constant(), capture='instr')

    def prepare(self, fn, ctx):   # optional
        return ...

    def find_target(self, m, instrs, src_idx, prep, ctx) -> int | None:
        # Return the target index (> src_idx) to move to, or None.
        # The SUBCLASS is responsible for all soundness checks:
        # - no block-boundary crossing
        # - no live-range overlap with intervening instructions
        # - any other semantic gate
        ...
```

The framework moves the matched instruction so it ends up at `target`
in the output, with the instructions that were between `src_idx` and
`target` shifted left by one. If `find_target` returns None or a value
≤ `src_idx`, the instruction is left in place and iteration advances
by 1.

**The subclass owns all soundness.** `SinkPass` does not check
block-boundaries, live-range overlaps, or any semantic property —
those are the subclass's responsibility inside `find_target`. The
`SinkIncrements` pass (`sink_increment.py`) illustrates the typical
gate structure: it walks forward from `src_idx` looking for the last
use of the instruction's dst, checking at each step that no intervening
instruction reads the dst or aliases its def.

## Adding a new pass — decision tree

- **N adjacent instructions → M instructions**: `WindowPass`. Set
  `window_size`, declare `pattern`, implement `rewrite`.
- **Walk a use site and look back through SSA defs**: `DefUsePass`.
  Declare `pattern` for the use site, use `env.def_of(var)` in
  `rewrite`. Use `Rewrite(replacement, drop_defs=…)` to atomically
  drop now-dead producers.
- **Rewrite individual USE-position operands, same opcode**: 
  `OperandRewritePass`. Declare `operand_pattern`, implement
  `rewrite_operand`.
- **Move a single instruction forward without rewriting it**:
  `SinkPass`. Declare `pattern`, implement `find_target` with all
  soundness gates.
- **None of the above** (CFG-shaped dataflow, multi-block analysis,
  bespoke structural transform): subclass the appropriate phase ABC
  directly (`FixedpointPass`, `PostFixedpointPass`, etc.) and
  implement `run(fn, ctx)` by hand.

## What stays raw and why

Nine passes implement `run` directly rather than using a DSL base.

- `CopyPropagate` (`copy_propagation.py`) — raw `FixedpointPass`. CFG-
  shaped forward dataflow; the chain-following logic reads every block
  in dominator order and doesn't fit a linear-window or def-use model.
- `EliminateDeadStores` (`dead_store_elimination.py`) — raw
  `FixedpointPass`. SSA-aware liveness; requires scanning the whole
  def-use web to decide which defs are live, not a per-site rewrite.
- `EliminateDeadLoops` (`dead_loop_elimination.py`) — raw
  `FixedpointPass`. Detects natural loops via back-edges (CFG
  structure); the soundness gate requires inspecting entire loop bodies
  for side effects.
- `EliminateUnreachableCode` (`unreachable_code_elimination.py`) —
  raw `FixedpointPass`. Forward DFS from ENTRY; prunes dead Phi args
  and folds singleton Phis — inherently whole-CFG work.
- `SinkAndPastBranch` (`sink_and_past_branch.py`) — raw
  `FixedpointPass`. Duplicates a trio of instructions into multiple
  successors with SSA renaming — structurally different from
  `SinkPass`, which moves a single instruction forward in a flat list.
- `FoldShortCircuitJump` (`short_circuit_jump_fold.py`) — raw
  `PostDestructionFixedpointPass`. Multi-block retarget map with
  transitive closure; the pattern spans basic-block boundaries and
  requires a custom two-pass algorithm.
- `RotateSignedCountdownLoops` (`loop_rotate.py`) — raw `PreSsaPass`.
  Structural shuffle of instruction ranges in pre-SSA form; no
  per-site pattern applies.
- `FoldCopiesInFixedpoint` / `FoldCopiesPostDestruction`
  (`copy_folding.py`) — raw `FixedpointPass` / `PostDestructionPass`.
  Both delegate internally to a private `_FoldCopiesWindow` (a
  `WindowPass`), but the phase-wrapper classes are hand-written because
  `FoldCopiesPostDestruction` inherits `PostDestructionPass`, not
  `FixedpointPass`, which WindowPass can't express directly.
