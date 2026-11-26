# TODO

## Let zp_abi functions call soft-stack functions without a pool

A function only gets zp_abi when every function it calls directly
either has it too or has a private pool
(`abi_selection._demote_soft_stack_callers`). A soft-stack function
without a pool uses $80–$BF as caller-saved scratch, and the zp_abi
parameter slots and private local pools come from the same window.
Before the rule, `int f(int p) { int l = rec(3); return p + l; }`
with a recursive `rec` read a clobbered `p`. A soft-stack function
with a pool, such as one whose address is taken or a recursive one
whose recursive calls are marked `[[c6502::save_hardware_stack]]`,
writes only its pool, so calling it doesn't demote anything.

The callees that still demote are the ones whose writes are
unbounded: recursion without the attribute, a function that calls
through a pointer, and an `extern` that isn't zp_abi (such as a ROM
routine declared without `zp_abi`). One of these demotes every
function that can reach it, and a demoted function loses zp_abi
parameters and its private pool. A function marked
`[[c6502::zp_abi]]` or using `register` can't be demoted, so
calling one makes it fail to compile instead.

Possible approaches, roughly in order of effort:

- **Save and restore around the call.** Treat the caller's live
  slot and pool bytes like caller-saved registers, pushing them on
  the hardware stack before the JSR and pulling them back after.
  `passes/recursion_saves.py` already does this for calls that
  recurse (`plan_saves` picks the values, `tac_to_asm` emits `PHA` /
  `PLA`, and the asm optimizer checks nothing colorable is live
  across). Three differences:
  - The caller isn't on a cycle, so it appears at most once in any
    chain of calls, and the hardware stack these saves use is
    bounded by the call graph. The compiler can add it up and fall
    back to demotion past a budget, with no attribute.
  - The save set has to include the caller's zp_abi parameters,
    which `plan_saves` leaves out because a recursive function keeps
    them in its frame.
  - The saving spreads upward. If `h` calls `f` and `f` calls an
    unbounded `g`, `g` can overwrite `h`'s bytes too, so `h`'s call
    to `f` needs saves as well. That replaces today's upward
    demotion with upward saves, and with several call sites it can
    cost more than demotion, so it needs a per-function estimate.
  Demoting only when nothing is live across the call is the case of
  this with nothing to push.
- **Keep soft-stack scratch out of the zp_abi window.** Allocate
  the slots and pools of functions that call unbounded code from the
  callee-saved $C0–$FF range, which soft-stack prologues already
  save and restore. A pool there breaks an unbounded function that
  calls into it, since that function expects $C0–$FF to survive its
  calls, so every function reachable from unbounded code has to stay
  out of it. This needs a look at ZP pressure, since both pools
  would shrink.

Whatever approach is taken, the call-graph fuzzer from Nov 2026
(random DAGs mixing zp_abi, recursive, and pointer-called functions
with values live across calls) is the way to check it, because
every bug found in this area showed up only with a value live
across a call into a different calling convention.

## Optimizations found by Zork's string printer and save instruction

These were found in `examples/zork/print_zstring` and
`examples/zork/instr_save`, which were removed on 16 Nov 2026. The same
routines are in `examples/zork`, whose README compares it
with Zork's code.

- **Branch layout in an `if` / `else if` chain.** `print_zstring`
  (`zstring.c`) took about 3 more cycles per z-character than Zork's.
  `c99_to_tac` lowers each test as a jump past its case, so a letter,
  the common case, takes four branches (`BNE`, `BNE`, `BCS`, `BCS`)
  past the space, abbreviation, and shift cases, where Zork branches
  to those cases and falls through. Laying out a chain with the
  final `else` first would need some sense of which case is common.
- **A short shared tail copied instead of jumped to.** Each letter
  case ends `JMP .if_end`, which falls into `JSR buffer_char; JMP
  .loop`. Copying those 6 bytes over the 3-byte `JMP` saves 3 cycles
  a character, as Zork's layout does.
- **Value ranges across a call.** `get_alphabet() + 2 + z` in
  `print_zstring` (at most 2 + 2 + 3) needs no second `CLC`, which
  takes the callee's result range.
- **A global copied to a local before a loop that doesn't write it.**
  `find_index_of_page_table` (`pages.c`) copied `SCRATCH2`'s bytes
  into locals and compared against those, where Zork reads `SCRATCH2`
  in the loop.
- **`buffer_char`'s word wrap** (`output.c`). The loop that moves the
  next word to the start of the buffer kept `++BUFF_LINE_LEN` in a
  local and computed `BUFF_AREA[BUFF_END++]` through A, where Zork
  does `INC BUFF_LINE_LEN; LDX BUFF_LINE_LEN` and `LDX BUFF_END; STA
  BUFF_AREA,X; INC BUFF_END`.

## Volatile structs

A `volatile struct s g;` object crashes `c99_to_tac` with a Python
traceback when a member is accessed (`_member_offset` reads `.tag`
from the `Volatile` wrapper), and a volatile-qualified member
(`struct s { volatile unsigned char b; };`) is rejected by the type
checker as "static-storage object can't have non-object type
Volatile". Both should compile, with every access to a volatile
member kept and in order, which `direct_member_access` already
preserves through the `is_volatile` bit. Until then, the crash should
at least be a one-line error.

## More system-software headers

`include/apple2/monitor_io.h` and `interactive_monitor.h` reserve the
Monitor's zero page, and the runtime's block moves around it
(`passes/runtime_layout.py`). Headers for DOS 3.3, ProDOS, and
Applesoft would do the same: each declares the zero-page bytes that
software uses, from its listing, so a program that includes one keeps
those bytes free. `examples/zork/zork.h` declares the
bytes that RWTS uses by hand.

## Rotate loops with tests of more than one branch

`passes/rotate_loop_test.py` moves a loop's test to its bottom in the
asm, after register allocation, when the test is one branch to just
after the loop's `JMP` back. A test with more than one branch, such
as a 16-bit compare that branches on its high byte first, keeps its
place, and so does a loop whose test leaves to somewhere else, such
as a `return` inside the loop. The test at the bottom could keep each
branch, inverting the last one to go back to the body.

An earlier rotation in TAC (Nov 2026) was taken out because it made
other examples worse: `draw_sprite_opaque` went from 47 instructions
to 89 and gained a soft-stack frame, and `snd_delay_up` and
`sfx_tone` grew by about a third. The rotated loop's Phis at both the
header and the exit seemed to produce a frame variable somewhere in
the asm-level SSA round trip, and a register-pinned local lost its Y
placement for one of its versions. Rotating after register
allocation sidesteps both. That attempt also exposed two bugs that
are fixed: `from_ssa` placed the copies for both successors of a
conditional block in front of its jump (wrong for a do/while's
bottom test, which already had this shape), and a soft-stack
prologue overwrote a `register` parameter arriving in A.

## Catch a sim program that grows into the soft stack

The simulator loads code and data at `$0800` (`sim/harness.py`,
`DEFAULT_ORIGIN`), and the soft stack grows down from `$7FFF`
(`sim/runtime.py`, `SSP_INIT`). Nothing checks that the image ends
below the stack, so a program big enough to cross into it runs with
its own code or data being overwritten by stack frames, and the
results come back wrong with no error. An unoptimized program with
148 of the 64-bit shift and multiply functions in
`tests/test_const_shift_mul_sim.py` reached `$BF0E`, and its 64-bit
multiplies returned 0. That test now splits its functions into
programs of 40 to stay clear of it.

`build_sim` could raise when the image's end address is above some
margin below `SSP_INIT`, and the runtime could also trap a store
below the image's end through `SSP` or `FP`. The first check is
cheap and would have pointed at the problem immediately.

## Return a byte in X or Y when every caller moves it there

A function returning one byte leaves it in A. When the value is
already in X at the return and every caller either moves it to X
with `TAX` or ignores it, returning it in X would remove the `TXA`
before each `RTS` and the `TAX` after each call. Y works the same
way. In Zork's interpreter (`examples/zork/save.c`),
`copy_data_to_buff` and `copy_data_from_buff` end in `TXA; RTS`, and
two of each one's five calls are followed by `TAX`, so this would
save 6 bytes there.

The compiler could do it in a pass over the whole unit after the
peepholes, for a function that has internal linkage, whose address
isn't taken, and which returns one byte, since every call to it is
then in the unit. The pass checks that after each call A is dead or
is moved to X (or Y) and then dead, switches the function's returns
to leave the value in X, and deletes the `TAX` at each call. The asm
`Ret` already has a `save_x` flag that marks X live at a return, for
the high byte of a 2-byte value, so the liveness passes would keep X
live there. A function called from other units would need the choice
recorded in the link metadata, and the linker would have to rewrite
callers it can't see when compiling the callee, so that case is
probably not worth it.

## Make Zork's input.c smaller

`examples/zork/input.c` is 1,110 bytes, where Zork's
code for it is 1,018. `match_dictionary_word` is 181 bytes against
Zork's 190. The separator tests, `is_dict_separator`,
`is_std_separator`, and `is_separator`, are 52 bytes against Zork's
44:

- Zork's `is_dict_separator` branches to `is_std_separator`'s `SEC;
  RTS` and `CLC; RTS`. A branch from one C function into another
  works only if the linker keeps the two functions together and
  within a branch's reach, and `link_layout.fall_through` can move a
  function. That would save about 2 bytes here. Each function
  branches over its `SEC; RTS`, and one of those branches could go to
  the other function's copy. Each `CLC; RTS` ends its function, where
  a branch to another copy saves nothing.
- `get_dictionary_addr` returns the dictionary's address in A and X,
  and `is_dict_separator` and `match_dictionary_word` each store it
  into a zero-page pair to use as a `(zp),Y` base, 4 bytes in each.
  Zork's leaves it in `SCRATCH2`.
- `is_dict_separator` tests `c` for 0 before its loop. Zork's pushes
  `c` around the call, and the `BEQ` at the top of its loop tests the
  `PLA` the first time and the `DEX` after that.

## Make Zork's numbers.c smaller

`examples/zork/numbers.c` is 475 bytes, where Zork's code
for it is 436. `divu16` (62 against 78) and `mulu16` (42 against 69)
are already smaller. What's left:

- `instr_mul` is 72 bytes against 53 and `instr_div` 75 against 50.
  They test `b` as Zork's do, and the `* 4` path shares the `* 2`
  path's shift, but with a `JMP` where Zork's falls into it, since
  `cross_jump` doesn't move blocks.
- `check_sign`, `store_with_sign`, and `print_number` each negate
  inline, 10 to 12 bytes, where Zork's call one `negate`. A C
  `negate` called by all three came out 9 to 22 bytes longer. The
  function is 15 bytes, and each caller copies its value into
  `negate`'s slot or out of the returned A and X. `check_sign` gains
  12 bytes as a tail call, `return negate(a);`, with `a` in `negate`'s
  slot (`slot_sharing`). But once `store_with_sign` calls it, its
  parameter shares a slot, which `slot_coalescing` leaves alone, so
  `instr_mul` and `instr_div` copy their result into it, 4 bytes
  each. Zork's `negate` works on `SCRATCH2` in place, where every
  caller already keeps its number. Letting `slot_coalescing` merge a
  local into a callee's slot that a member share holds, when the
  local is that callee's argument, would recover those 8 bytes, which
  still leaves the shared `negate` about even with the inline ones.
- `instr_mul` copies `b` from `check_sign`'s slot into `mulu16`'s, 8
  bytes, and `instr_div` and `instr_mod` copy it into `divu16`'s.
  Zork's code leaves it in `SCRATCH1` for both. `slot_sharing` keeps
  a local passed to two callees in the first one's bytes only, and
  `mulu16` assigns its `b`, which `_param_slots` keeps out of every
  share. Keeping `b` in one set of bytes for `check_sign`, `mulu16`,
  and `divu16` would take a member whose owner writes it: a share
  member's owner would become one of the share's writers, which no
  holder may keep its value across, in place of the rule that it
  never assigns the parameter. `check_shares` and
  `check_linked_shares` enforce that rule now, and the cross-unit
  `const` promise rests on it. The one local that `instr_mul`'s
  result of `check_sign` and its product share is the right
  allocation: the `* 2` and `* 4` paths shift it in place and pass it
  on, so a local of its own for the product would need a copy there.
- `print_number` is 82 bytes against 65. Zork pushes the digits on
  the 6502's stack, which C can't, and a recursive digit printer
  marked `[[c6502::save_hardware_stack]]` would push them but
  take its parameter on the soft stack. Each pass through the loop
  also copies `n` into `divu16`'s dividend slot and its quotient back
  (12 bytes), and stores the divisor 10 again (8), since nothing
  tells the caller that `divu16` leaves its divisor alone. Zork's
  `divu16` works on `SCRATCH1` and `SCRATCH2` in place.

## Keep hot branches from crossing pages

A taken branch that crosses a page takes a cycle more, so where the
linker places a hot function changes its cycle count. Shrinking
`objects.c` in Zork's interpreter moved `branch` and `print_zstring`,
which the linker places after functions of `objects.c` that fall into
them, and that cost 16,534 cycles in the first test session, more
than the change had saved. The cycles don't show in a function's own
code. The linker could place hot functions, or pad in front of a hot
loop, so that their branches stay within a page, at the cost of a
few bytes. It would need to know which branches are hot, from a
profile or from loop depth.

## Check where `[[fallthrough]];` is

`parser._Builder.attribute_declaration` rejects `[[fallthrough]];` at
file scope by testing `len(_CTX.scopes) == 1`. The lexer runs a token
ahead of the reductions, so when the declaration is the last thing in
a function body it has already read the `}` and closed the body's
scope. This function then gets "belongs in a switch statement, not at
file scope":

```c
void h(int x) {
    x++;
    [[fallthrough]];
}
```

The code is invalid anyway, because C23 §6.7.13.6 lets the attribute
appear only in a switch, right before a `case` or `default` label,
but the message names the wrong place. Nothing checks the rule itself
either: `[[fallthrough]];` anywhere inside a function is accepted,
including as the last statement of a switch's block. The check belongs
where the parser knows which statement follows, as `var_decl` does for
`[[c6502::save_hardware_stack]]` (it returns a marker that `decl_item`
acts on and `start` rejects). Then report a `[[fallthrough]];` that
isn't followed by a label in the same switch.

## Step 9 of the C++ port

Step 9 of docs/cpp_port.md ("Order") moves the rest of the
repository's Python to C++, so that no Python is left. A C++ 6502
simulator replaces py65 and `sim/`, including the Python hooks that
stand in for the runtime helpers with no 6502 code yet. It's checked
against py65 instruction by instruction (registers, flags, memory
written, and cycles) on the corpus's programs and the games' scripted
runs. The test suite and the fuzzers with their reducer move to C++
programs and doctest.

Step 8 finished on 25 Nov 2026, and step 9 has no survey or plan of
groups yet, so the first piece of work is the survey and the plan, as
step 8 had. The root CLAUDE.md lists the Python that is left, and
"What step 9's Python keeps", under step 8, says what reads each
module, though tac_sim.py and fp_arith.py, which it lists, went in
group R, since the TAC interpreter is in C++. Once the suite is in
C++, compile.py, nm.py, and objdump.py stop being shims, and the
messages that still say `compile.py:` can change.
