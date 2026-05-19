# Changing a default uncovers the arrangements

c6502 has had `__attribute__((zp_abi))` since fairly early.
It's an opt-in calling-convention attribute that puts parameter
bytes in pinned zero-page slots instead of pushing them onto
the soft data stack. The eligibility check is mechanical (no
IndirectCall in the body, not on a cycle in the static call
graph, address never taken, param byte count fits the ZP
window), and the allocator that hands out non-overlapping slot
ranges has been stable for a while.

I'd been writing it on every interesting function in the
example corpus by hand. Annotating each function with
`__attribute__((zp_abi))` was repetitive, and the failure mode
of forgetting was just "soft-stack ABI" — correct, just a few
bytes and cycles per call slower than necessary.

The user asked for two changes: drop `--unroll` as a separate
flag (fold it into `--optimize`), and make every function default
to zp_abi when eligible, falling back silently to soft-stack
when not. Two pure-default changes, no semantic changes.

The `--unroll` removal was mechanical. Twenty-line edit in
`compile.py`, updates to a handful of tests that wired the flag
through, and a CLAUDE.md / pass-CLAUDE.md update to reflect that
unrolling now runs under `--optimize` rather than gated behind a
separate flag. The pragma (`#pragma c6502 loop unroll(enable)`)
still gates which loops actually unroll, so source-level opt-in
is preserved.

The zp_abi default change was 30 lines in
`passes/abi_selection.py:select_abi`. The annotated-function
path was already there; I added a parallel unannotated path that
tries `_validate_zp_abi` and silently swallows the
`AbiSelectionError` on ineligibility. Same for unannotated
externs — try zp_abi extern, silent fall back.

I asked the user upfront about the asymmetry: annotated functions
keep the strict contract (hard error on ineligibility — preserves
today's behavior for code that explicitly opted in), unannotated
ones silently fall back. The user confirmed. Also asked whether
to apply this to non-optimize builds; they said no, keep
soft-stack-only when `--optimize` is off.

Then I ran the test suite.

## Seven failures

Two of the seven were trivial: tests that asserted the old
defaults. One was named `test_optimize_param_still_uses_frame`
and read:

```python
# Per the MVP rule, params always use Frame addressing
# regardless of any color regalloc may have assigned —
# the calling convention dictates the on-entry layout.
src = "int main(int p) { return p + 1; }"
rc, out, _ = self._run(
    ["compile.py", "-", "--codegen", "--optimize"], stdin=src,
)
self.assertEqual(rc, 0)
self.assertIn("(FP),Y", out)
```

The MVP rule was exactly what the new default replaced. I split
it into two tests: one verifies an *eligible* function's param
goes into `__zpabi_main__p_0` (the new default), the other
constructs a *recursive* function and verifies the param falls
back to `(FP),Y` (the silent fallback still works for
ineligibles).

The linker test was similar — it had asserted that the linker
*rejected* a TU calling an unannotated extern, because that
extern's ABI was unknown. Now that everyone defaults to zp_abi
when eligible, the unannotated extern *also* defaults to zp_abi
at the call site. The link succeeds, with the user implicitly
on the hook for ensuring the actual definition in the other TU
uses a matching ABI. The test now asserts that the link
succeeds.

The other five failures were sim_differential failures, and they
were real.

## Bug 1: `LoadAddress` hides address-taken-ness

The chapter_14 test `static_var_indirection.c` exercises
pointers to static and automatic variables. One scenario:

```c
long long modify_ptr(long long *new_ptr) {
    static long long *p;
    if (new_ptr) p = new_ptr;
    return *p;
}

int main(void) {
    /* …earlier scenarios omitted… */
    long long l = 1000ll;
    if (modify_ptr(&l) != 1000ll) return 6;
    /* … */
    return 0;
}
```

The sim_differential test reported the optimized version
returning 6 (failure) while the unoptimized returned 0 (success).
Eight bytes off the truth, somewhere.

I dumped the post-execution memory state and the local `l` was
all zeros at the time modify_ptr was called. Then I disassembled
the compiled binary looking for `LDA #$E8` (the low byte of 1000
in two's complement little-endian — the first byte the
initializer should write). Not there. Anywhere.

Now the strange part: the `.asm` text output of `compile.py
--codegen --optimize` for the same source clearly contained:

```
.if_end@5:
   LDA   #$E8
   STA   __local_main__l
   LDA   #$03
   STA   __local_main__l+1
   LDA   #$00
   STA   __local_main__l+2
   …
   LDA   #<__local_main__l
   STA   __zpabi_modify_ptr__new_ptr_0
   LDA   #>__local_main__l
   STA   __zpabi_modify_ptr__new_ptr_1
   JSR   modify_ptr
```

The compiler emitted the right asm. But the sim was simulating
a different program.

c6502's sim uses `sim/harness.compile_to_asm` which goes through
mostly the same passes as `compile.py --codegen`, but not all of
them. Specifically, `compile.py` runs `lower_data_load_address`
after `replace_pseudoregs_bare_exit` and before the peephole
fixedpoint. The sim doesn't.

`lower_data_load_address` splits the compound
`LoadAddress(src=Data(name), dst=Y)` atom — which emits as
`LDA #<name; STA Y; LDA #>name; STA Y+1` — into two separate
`Mov(ImmLabelLow(name), Y)` / `Mov(ImmLabelHigh(name), Y+1)`
atoms. The split exposes the `name` field as an `ImmLabel*`
operand instead of hiding it inside a `Data` operand within a
compound atom.

Why does that matter? `asm_dead_store` has a Call-transparency
optimization. The allocator's invariant is that each function's
`__local_<fn>__*` private pool range is disjoint from every
transitively-reachable callee's allocation. So a direct `JSR
<callee>` can be treated as transparent for a slot in the
caller's `__local_<…>__*` namespace — the callee literally
cannot name that byte — *unless* the slot's address has been
constructed somewhere in the function (which would let a
pointer-receiving callee observe it).

The "constructed somewhere" check walks every operand of every
instruction and collects every `ImmLabelLow.name` /
`ImmLabelHigh.name`. If `__local_main__l` is in that set, the
JSR is opaque for it. If it isn't, the JSR is transparent, and
any STA to `__local_main__l` before the JSR (with no observable
read between the STA and the JSR's return) is dead.

`LoadAddress(src=Data(__local_main__l), …)` keeps the name in a
`Data` operand. The operand walk sees only the `Data`, not the
"this is going to lower into `LDA #<__local_main__l`" semantics.
With `lower_data_load_address` having run, the split atoms
expose `ImmLabelLow(name='__local_main__l')` directly and the
operand walk finds it. Without that pass, the name is invisible.

Under the old default, modify_ptr was unannotated and got
soft-stack. The arg setup goes through `(SSP),Y` writes after
an `AllocateStack(N)` atom — and `AllocateStack` is on the
opaque-atom list, so `asm_dead_store` couldn't drop *any* STA
across it. The bug was there, the operand walk had a gap, but
`AllocateStack`'s opacity covered it.

Under the new default, modify_ptr is zp_abi. The arg setup is
`STA __zpabi_modify_ptr__new_ptr_0; STA __zpabi_modify_ptr__new_ptr_1`
(two ordinary atoms), followed immediately by the JSR. No
`AllocateStack` between. The Call-transparency optimization
fires, decides "this slot's address isn't taken anywhere I can
see," and drops the eight STAs that initialized `l`. modify_ptr
later dereferences the uninitialized slot and returns garbage.

Fix is two lines in `_collect_address_taken_names`: when walking
a `LoadAddress` atom, also mark the inner `Data` name as
address-taken. The operand walk handles the rest.

## Bug 2: Struct returns prepend a hidden sret param

The chapter_18 tests pass struct-by-value parameters and return
struct types. The optimized compile blew up with `IndexError:
list index out of range` in `_emit_zp_arg_writes`.

The call-site code there walks the TAC arg list, computes a
flat byte offset, and looks up `layout.slot_symbols[flat_idx +
k]` for each byte. The IndexError means flat_idx exceeded the
length of slot_symbols.

The function in question:

```c
struct big return_in_mem(void) {
    /* … */
    return globl2;
}
```

A struct-returning function taking no explicit params. From C's
point of view, no args.

But c6502's calling convention for struct returns is sret: the
caller allocates a return slot, passes its *address* as a hidden
first arg, and the callee writes through that pointer.
`c99_to_tac` implements this by prepending the address-of-slot
to `arg_vals` at every call site and prepending a matching
`sret_param` to the TAC function's `params` list.

So the TAC function has one param (the sret pointer). The TAC
call site has one arg (the same).

`select_abi._param_byte_count` reads `fn_decl.data_type.params`
— the FunType's param list, which is the explicit declaration
in C. That list has *zero* entries for `void`. The ZpLayout
comes out with `slot_symbols=[]`. The call site walks one
2-byte arg and immediately overruns the empty slot list.

This had the same shape as bug 1: latent under the old default
(unannotated → soft-stack, which doesn't index a slot_symbols
list), uncovered when the default flipped.

Two ways to fix: extend the sizer to add 2 bytes for sret when
the return type is `Structure` / `Union`, or reject
struct-returning functions from zp_abi and let the
silent-fallback path catch them. The second was a four-line
change in `_validate_zp_abi` (and the matching extern
validator), and soft-stack already handles sret correctly. So
that's what landed. There's a future cleanup where zp_abi
properly supports struct returns by minting
`__zpabi_<fn>__sret_0` / `_1` slot symbols at the head of the
layout, but it's not load-bearing for the current corpus.

## The meta-observation

Both bugs had been latent for as long as their respective code
paths weren't exercised. The first one needed
(a) an `__local_<fn>__*` slot whose address is taken
(b) via a `LoadAddress` atom, not via explicit `ImmLabel*` Movs
(c) followed by a JSR with no `AllocateStack` between them
(d) in the sim pipeline (compile.py masks it via
`lower_data_load_address`).

Soft-stack-by-default ensured condition (c) was almost never
true — every C call to a struct-by-value or
&local-passing function went through `AllocateStack`. The fact
that the operand walk also missed (b) was a separate gap, but
(c) covered for it.

The second bug needed
(a) a struct-returning function
(b) compiled with zp_abi enabled
(c) called somewhere.

Soft-stack-by-default ensured (b) was off for any function
without the explicit annotation. Nobody annotated a struct-
returning function with `__attribute__((zp_abi))` because the
sret semantics didn't compose with zp_abi anyway. Bug stayed
dormant.

The first bug was a *real* soundness bug in `asm_dead_store`
— the operand walk should have been catching this case
regardless of which calling convention was in play. The fact
that you needed the right sequence of pass non-applications
plus the right ABI convention to expose it is just how latent
bugs are. The second bug was an arrangement of the codebase
that worked correctly for the constraints in force; the
default change broke the constraints, so the arrangement
fell over.

I think the bigger lesson is that "no semantic change" defaults
are misleading. The semantics of any *individual* call are
identical with or without zp_abi (it's a calling convention,
not a language feature). But the *set of code paths reachable*
under one default differs from the set reachable under the
other, and bugs in the difference between the two sets only
show up when you flip the bit.

Test count: 2660 → 2661. Examples unchanged (they were already
heavily annotated, so default-zp_abi was a no-op there). The
win is on un-annotated code: every `int main(int argc, …)`
helper that nobody had bothered to mark up now drops its frame
setup entirely. The sim_differential corpus passes from end to
end. Two soundness fixes saved to memory so the next default
change starts a notch less wrong.
