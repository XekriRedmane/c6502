# The store side of my C99-to-6502 compiler was using DPTR for
# everything, and a new TAC variant fixed it

Third in a series on rounding off a 6502 compiler. The motivating
function is still

```c
static void spawn_pos_dir(uint8_t slot)
{
    entity_active[slot]    = 0x01;
    rescue_dir[slot]       = 0x01;
    entity_floor_col[slot] = 0x3E;
    /* ... */
}
```

In the two earlier rounds I made the function's parameter live in
zero page and removed dead `PHA/PLA` pairs around the index
setup. After those, each store looked like

```asm
LDA   #<entity_active
STA   __local_spawn_pos_dir_b0
LDA   #>entity_active
STA   __local_spawn_pos_dir_b0+1
LDA   #$01
LDY   __zpabi_spawn_pos_dir_p0
STA   (__local_spawn_pos_dir_b0),Y
```

— stage the array's address into a zero-page pointer, then
indirect-Y store. The address is a link-time symbol; staging it
through a runtime pointer is pure waste. The 6502 has the
addressing mode I want sitting right there:

```
STA  $XXXX,X      ; absolute,X — 3 bytes, 5 cycles
```

`STA arr,Y` works too. So why was the compiler emitting the staged
indirect-Y form?

## Asymmetric fast paths

The READ side of this code was already perfect. `floor_thresh
[rescue_floor[slot]]` lowered to

```asm
LDX   __zpabi_spawn_pos_dir_p0
LDY   rescue_floor,X        ; absolute,X — extern, sized
LDA   floor_thresh,Y        ; absolute,Y — extern, unsized
```

The fast path that produced this lives in `c99_to_tac`:
`_try_indexed_load_subscript` runs during translation, checks
eligibility (the underlying array has static storage, total byte
size ≤ 256), and directly emits a new TAC instruction
`IndexedLoad(name, index, dst)`. `tac_to_asm` lowers that as the
absolute-indexed read.

There was no mirror on the store side. `arr[i] = v` lowered as

```
%addr = GetAddress(arr)
%scaled = ZeroExtend(i)
%final = Add(%addr, %scaled)
Store(v, %final)
```

— a runtime pointer-arithmetic chain ending in an indirect Store.
`tac_to_asm.translate_store` for a Pointer-typed dst stages the
pointer into DPTR and uses `(zp),Y`. Correct, but generic.

There IS a TAC pass `recognize_indexed_store` that fuses
`ZeroExtend + Add(Constant) + Store` into `IndexedStore(int
address, ...)`, but it specifically wants a **Constant** base — it
fires for `static T * const buf = (T*)0x2000;` patterns after the
const-static fold turns `buf` into `Constant(0x2000)`. For an
unfolded array name like `entity_active`, the recognizer doesn't
match.

## The new TAC variant

I added `IndexedSymbolStore(identifier name, val index, val src,
bool is_volatile)` — the store-side mirror of `IndexedLoad` —
emitted by a new `_try_indexed_store_subscript` in `c99_to_tac`
that mirrors the load fast path's eligibility. `tac_to_asm` lowers
it as

```python
def _translate_indexed_symbol_store(self, name, index, src, vol):
    n = self._size_of(src)
    out = [
        asm_ast.Mov(src=index_op, dst=_REG_A),               # LDA idx
        asm_ast.Mov(src=_REG_A, dst=asm_ast.Reg(reg=asm_ast.X())),  # TAX
    ]
    for k in range(n):
        out.append(asm_ast.Mov(src=_byte_at(src_op, k), dst=_REG_A))  # LDA src[k]
        out.append(asm_ast.Mov(
            src=_REG_A,
            dst=asm_ast.IndexedData(name=name, offset=k, index=asm_ast.X()),
        ))  # STA name+k,X
    return out
```

The existing `direct_index_load` peephole collapses `LDA idx; TAX`
into `LDX idx` when `idx` resolves to Imm/Data/ZP, so the final
shape is `LDX idx; LDA src; STA name,X` per byte.

Final asm for `spawn_pos_dir`:

```asm
.spawn_pos_dir@asm_ssa_block@0:
   LDX   __zpabi_spawn_pos_dir_p0
   LDA   #$01
   STA   entity_active,X
   STA   rescue_dir,X
   LDA   #$3E
   STA   entity_floor_col,X
   LDA   #$00
   STA   entity_xoff_idx,X
   STA   rescue_anim,X
   LDY   rescue_floor,X
   LDA   floor_thresh,Y
   SEC
   SBC   #$07
   STA   entity_floor_pos,X
   RTS
```

26 lines, down from 147 at the start of this work. Successive
stores share the `LDX` (X doesn't move between iterations) and,
where the value repeats, share the `LDA`. The asm-level
optimization rounds picked those wins up for free once the
operand shape was right.

## The bureaucracy

The painful part of adding a TAC variant in this codebase is the
fan-out across optimization passes that match on TAC instruction
type. Eight separate files needed a new case for
`IndexedSymbolStore`:

1. `var_visit.py` — both `uses_in` and `vals_in` need cases or
   downstream DSE doesn't see the variant's operands as live.
2. `copy_propagation.py` — the substitution rewrite.
3. `ssa_construction.py` — use/def renaming.
4. `static_const_fold.py` — operand substitution again.
5. `cmp_zero_jump_fold.py` — operand visitor (lighter
   consequence).
6. `dispatch_pointer_array.py` — operand visitor (same).
7. `dead_loop_elimination.py` — the side-effecting-types tuple.
   Required for store-shaped variants or dead-loop elim
   incorrectly hoists them.
8. `tac_sim.py` — the interpreter steps for tests.

The first three I added; trial-compiled; got beautifully-wrong
output (DSE had dropped the constants because uses_in didn't
report `IndexedSymbolStore.src` as a use); patched one more pass;
trial-compiled; repeat. The lesson on the second iteration was to
`grep -rn IndexedStore passes/optimization/` first and patch them
all in one go — adding TAC variants is the kind of cross-cutting
change that benefits from up-front survey rather than iterate-on-
failures.

## Discussion

Two questions I'd put to people building similar small-target
compilers:

1. Asymmetric fast paths — load shape in c99→TAC, store shape in
   TAC optimizer — are a smell. Was it actually right to put load
   in c99→TAC, or should both fast paths live as TAC peepholes
   recognizing common shapes? The c99→TAC location avoids
   producing pointer-arithmetic TAC just to recognize and
   eliminate it, which seems good. But it's eight separate places
   the new variant has to be acknowledged.

2. The `IndexedLoad / IndexedConstLoad / IndexedStore /
   IndexedSymbolStore` naming asymmetry (load comes in
   name-keyed and address-keyed; store now does too) reflects the
   incremental order things were added. Worth a rename pass to
   make them symmetric (`IndexedSymbolLoad / IndexedConstLoad /
   IndexedSymbolStore / IndexedConstStore`)? Or live with the
   asymmetric history?

Code is at `passes/optimization/recognize_indexed_store.py` and
`c99_to_tac.py`'s `_try_indexed_store_subscript`.
