# Direct member access

`DirectMemberAccess` in
[passes/optimization/direct_member_access.py](../../passes/optimization/direct_member_access.py)
turns a load or store through `&obj + k` into a direct access to
byte `k` of `obj`. It runs in the TAC fixed-point loop, after the two
reassociation passes, so that the offset is already one constant. It
doesn't run in the unoptimized pipeline.

`c99_to_tac` lowers a struct member, and an array element at a
constant index, as pointer arithmetic on the object's address. For
`player.y = y` that's a `GetAddress`, an `Add` of 1, and a `Store`
through the result, and `tac_to_asm` builds the address in a
zero-page pair and writes through it with `STA (ptr),Y`. The pass
replaces the access with an `IndexedLoad` or `IndexedSymbolStore` at
a constant index, which `tac_to_asm` lowers to `STA player+1` for a
static object, or to the local's own byte for an automatic one. The
`GetAddress` and the `Add` stay in place, and dead-store elimination
drops them once nothing reads the address. For the example below the
function body goes from 58 bytes to 20.

A local struct or array whose accesses all go this way no longer has
its address taken anywhere. The asm register allocator can then give
its bytes zero-page slots in place of a soft-stack frame.

## Example

```c
#include <stdint.h>
struct obj { uint8_t x, y; uint16_t hp; };
struct obj player;
void place(uint8_t x, uint8_t y) {
    player.x = x;
    player.y = y;
    player.hp = 100;
}
```

### Before

```
GetAddress(player, %0.1)
Store(@0.x, %0.1)
GetAddress(player, %1.1)
Binary(+, %1.1, 1, %2.1)
Store(@1.y, %2.1)
GetAddress(player, %4.1)
Binary(+, %4.1, 2, %5.1)
Store(100u, %5.1)
Ret(None)
```

### After

```
GetAddress(player, %0.1)
IndexedSymbolStore(player, 0uc, @0.x)
GetAddress(player, %1.1)
Binary(+, %1.1, 1, %2.1)
IndexedSymbolStore(player, 1uc, @1.y)
GetAddress(player, %4.1)
Binary(+, %4.1, 2, %5.1)
IndexedSymbolStore(player, 2uc, 100u)
Ret(None)
```

The final assembly stores each byte with an absolute `STA`, from
`STA player` to `STA player+3`. With the pass switched off, the same
function reloads `#<player` and `#>player` into a zero-page pair for
each member and adds 2 to it for `hp`.

## When it applies

The pass follows each SSA pointer from a `GetAddress(obj)` through
any `Add` of an integer constant, and keeps the running offset. It
rewrites four kinds of access through such a pointer.

- `Load(&obj + k, dst)` becomes `IndexedLoad(obj, k, dst)`.
- `Store(src, &obj + k)` becomes `IndexedSymbolStore(obj, k, src)`.
- `IndirectIndexedLoad(&obj + k, j, dst)` with a constant `j`
  becomes `IndexedLoad(obj, k + j, dst)`.
- `IndirectIndexedStore(&obj + k, j, src)` with a constant `j`
  becomes `IndexedSymbolStore(obj, k + j, src)`.

All of these must also be true.

- `obj` has static storage, or it's an automatic struct, union, or
  array that isn't a parameter.
- The byte offset is between 0 and 255, since the index is one byte.
- The value loaded or stored is a scalar.

The volatile bit of the access carries over to the new instruction.
An access outside the object through `&obj + k` is undefined
behavior in C, so the pass doesn't check `k` against the object's
size.

## What it leaves alone

- An offset above 255. `big[300] = 1` on a 400-byte global array
  still stores through a pointer.
- An automatic scalar whose address is taken, such as `v` in
  `uint8_t *p = &v; *p = 1;`. Once its address is gone, SSA
  construction could rename the scalar, and the rewritten access
  would name the old variable. Aggregates are never renamed, so the
  restriction doesn't apply to them.
- A parameter, even an aggregate one.
- Access through a pointer that doesn't come from `GetAddress`, such
  as `o->y` on a `struct obj *o` parameter. The
  [indirect-indexed](recognize_indirect_indexed.md) recognizer turns
  that one into `LDY #$01; STA (__zpabi_viaptr__o_0),Y` later.
- A member that is itself a struct, copied as a whole. That copy
  still goes through the pointer.

## Related

[Combining added constants](reassoc_const.md) and
[offset reassociation](reassoc_offset.md) run first and leave the
offset as one constant.
