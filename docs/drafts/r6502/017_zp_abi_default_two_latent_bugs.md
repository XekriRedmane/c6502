# Making zp_abi the default surfaced two latent bugs

c6502 has a per-function calling-convention attribute,
`__attribute__((zp_abi))`, that puts param bytes in pinned
zero-page slots instead of the soft data stack. The optimizer
already had everything it needed to make this the default for
every eligible function — the eligibility check (no
IndirectCall, not in a call-graph cycle, address not taken,
params fit the ZP window) was already factored out. Flipping
the default was a 30-line change in `select_abi`: unannotated
functions try zp_abi and silently fall back to soft-stack on
ineligibility.

Running the test suite turned up seven failures. Two were
test-documentation cleanups (one test asserted "params always
use Frame addressing under --optimize" — that was the old
MVP rule, now obsolete). The other five were real, and they
exposed two latent bugs that the old default had been masking.

**Bug 1: `LoadAddress` hides address-taken-ness.**

`asm_dead_store` has a Call-transparency optimization: a direct
JSR can be treated as transparent (doesn't read or write) for
a `__local_<fn>__*` slot whose address is never built in this
function, because the allocator guarantees that range is
disjoint from every transitively-reachable callee's allocation.
The helper that scans for "address taken" walks every operand
and collects any `ImmLabelLow.name` / `ImmLabelHigh.name`.

That misses `LoadAddress(src=Data(name), …)` — the compound
atom that lowers to `LDA #<name; STA dst; LDA #>name; STA dst+1`.
The inner `name` is in a `Data` operand, not an `ImmLabel*`.
`compile.py` runs a `lower_data_load_address` pass before
`asm_dead_store` that splits the compound into two
`Mov(ImmLabel*, dst)` atoms, exposing the name. But the sim
pipeline doesn't run that lowering.

Under the old default, struct-by-value args went through the
soft stack, which inserts an `AllocateStack` atom between
"compute the arg" and the JSR. `AllocateStack` is opaque to
`asm_dead_store`, so the STAs initializing the local could
never be considered dead. Removing that barrier with zp_abi
left only the JSR between the init and the call — and the
mis-detected "no address taken" let DSE drop the init.

The compiler then issued a perfectly clean
`LDA #$E8; STA __local_main__l; LDA #$03; STA __local_main__l+1; …`
to the gold asm, but the sim's identical input came out with
those eight init instructions stripped. modify_ptr returned
zero. Comparison failed. Test returned 6.

Fix is one branch in `_collect_address_taken_names`: when
walking a `LoadAddress` atom, also mark the inner `Data` name
as address-taken. The operand walk catches the rest.

**Bug 2: Struct returns prepend a hidden sret pointer.**

`c99_to_tac` translates `struct big f(void)` by prepending a
hidden first parameter — a pointer to the caller-allocated
return slot. The TAC function ends up with one more param than
the C source declared.

`select_abi._param_byte_count` reads `FunType.params` (the
explicit param list) and sums sizes. It doesn't know about the
hidden sret param. For `struct big f(void)`: byte_count = 0,
`ZpLayout.slot_symbols = []`.

Under the old default this didn't matter — unannotated
functions got soft-stack, which doesn't index a slot_symbols
list. Under the new default, the first call to `f` reached
`_emit_zp_arg_writes`, walked the prepended-sret TAC arg list
(1 arg of 2 bytes), and tried to index `slot_symbols[0]` — out
of range.

Two ways to fix: extend the sizer to add 2 bytes for sret
when the return is `Structure` / `Union`, or reject struct-
returning functions from zp_abi and let the silent-fallback
catch them. The fallback was cheaper and soft-stack already
handles sret correctly, so I rejected them.

**The meta-observation**: defaults change which code paths get
exercised. Both bugs were latent for as long as struct-by-
value calls and address-of-local + JSR sequences used soft-
stack. The address-taken detection had a gap, but
`AllocateStack`'s opacity covered it. The sret sizing had a
gap, but unannotated functions never reached the slot-symbol
indexer. Both gaps were straightforward to fix once visible.

The CFG dataflow extension in r6502/016 had the same shape —
that one surfaced a self-Mov tracker mismodel that had also
been latent in the per-block tracker era. Three latent bugs in
two consecutive sessions, all unmasked by changing defaults
that the existing code had been carefully arranged around.

Test count went from 2660 to 2661. Net example .asm sizes
unchanged — the examples were already annotated `zp_abi` on
every interesting function, so the default-zp_abi change was
a no-op for the example corpus. The win is on un-annotated
code: `int main` and helper functions in the chapter test
corpus that no one had bothered to annotate now drop their
frame setup entirely.
