"""Reads the pointer again for each byte of a wide access through it.

`tac_to_asm` lowers a load or store of a value wider than a byte, at a
constant offset through a pointer, by copying the pointer into DPTR
once and then reading or writing each byte through `(DPTR),Y`. When
the pointer is in zero page, `memory_value_propagation` turns each
`(DPTR),Y` into `(p),Y` while it can show that DPTR still holds a copy
of `p`, and the copy into DPTR goes. Two things stopped that for the
second byte.

- A store through a pointer ends every fact the pass holds, since the
  byte it writes could be one of `p`'s. The first byte of
  `P->gold = 0` ended the fact that DPTR copies `P`, so the second
  byte was stored through DPTR and the copy stayed (8 bytes).
- The register allocator gave the pointer's bytes to another value
  once the copy into DPTR had read them. The low byte of `r->gold`,
  read through a pointer parameter `r`, went into `r`'s own byte, so
  the high byte could only be read through DPTR.

`restage_load` and `restage_store` tell `tac_to_asm` when to copy the
pointer into DPTR again before each byte after the first. The second
copy reads the pointer's bytes after the earlier bytes are read or
written, which keeps the pointer alive to the last byte, and each copy
gives `memory_value_propagation` a fact of its own, so every byte is
read or written through `(p),Y` and every copy into DPTR goes.

Copying again is the same as reading the pointer again. For a load
nothing is written between the copies, so the pointer has the value
it had. For a store, the earlier bytes of the same store are written
between the copies, and they could only change the pointer if the
store's bytes overlapped the object that holds it. C's effective-type
rule (C23 §6.5p7) makes that overlap undefined in the cases
`restage_store` accepts:

- The pointer is a TAC variable of pointer type, so the bytes it is
  copied from are an object of that type, or a temporary that no C
  lvalue can reach.
- The store writes the scalar that the pointer's referenced type has
  at exactly those bytes (`scalar_at`), a member, an element, or the
  whole referenced object. A union and a byte that only part of a
  scalar covers give no type, and the store is left alone.
- That scalar is no character type, so the store doesn't have the
  character type's licence to write any object, and its type isn't
  compatible with the pointer's own type. An integer or floating
  type is never compatible with a pointer type. A pointer type is
  compared with the pointer's type with every qualifier removed, and
  a pointer to an array or a function is taken to be compatible,
  which leaves `P->next = q` alone for a `struct node *P`, since a
  node that holds its own address is defined C.

A store through a pointer whose referenced type the program casts
(`((struct t *)p)->m`) is judged by the type TAC gives the pointer,
which copy propagation may make `p`'s own type. Writing `p`'s bytes
through a member of a structure that isn't there is outside what the
rule above covers, and the lowering assumes no program does it.

The copy pays only when `memory_value_propagation` can remove it,
which is possible only for a pointer in zero page: a TAC temporary or
local, which the register allocator puts in zero page under `-O`
unless it spills, a parameter's or local's zero-page slot, or a
zero-page object (`asm_aliasing.is_user_zp_global`). `pointer_in_zero_page`
checks that, and the unoptimized pipeline never copies again."""
from __future__ import annotations

import asm_ast
import c99_ast


def _unqualified(t):
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return t


def _deep_unqualified(t):
    """`t` with every `const` and `volatile` removed, through pointers
    and arrays."""
    t = _unqualified(t)
    if isinstance(t, c99_ast.Pointer):
        return c99_ast.Pointer(referenced_type=_deep_unqualified(
            t.referenced_type))
    if isinstance(t, c99_ast.Array):
        return c99_ast.Array(
            element_type=_deep_unqualified(t.element_type), size=t.size)
    return t


_CHARACTER_TYPES = (c99_ast.Char, c99_ast.SChar, c99_ast.UChar, c99_ast.Bool)
_SCALAR_SIZES = (
    ((c99_ast.Char, c99_ast.SChar, c99_ast.UChar, c99_ast.Bool), 1),
    ((c99_ast.Int, c99_ast.UInt, c99_ast.Short, c99_ast.UShort,
      c99_ast.Pointer), 2),
    ((c99_ast.Long, c99_ast.ULong, c99_ast.Float), 4),
    ((c99_ast.LongLong, c99_ast.ULongLong, c99_ast.Double), 8),
)


def _size(t, types) -> int | None:
    """The size of `t` in bytes, or None when it isn't known."""
    t = _unqualified(t)
    for classes, size in _SCALAR_SIZES:
        if isinstance(t, classes):
            return size
    if isinstance(t, c99_ast.BitInt):
        import bit_precise
        return bit_precise.storage_bytes(t.width)
    if isinstance(t, c99_ast.Array):
        e = _size(t.element_type, types)
        return None if e is None else e * t.size
    if isinstance(t, (c99_ast.Structure, c99_ast.Union)) and types:
        layout = types.get(t.tag)
        if layout is not None and layout.complete:
            return layout.size
    return None


def scalar_at(t, k: int, n: int, types):
    """The scalar type that an object of type `t` has at exactly bytes
    `k` to `k + n - 1`: a member of a structure, an element of an
    array, or `t` itself. None when no one scalar covers exactly those
    bytes, or when they are in a union, whose members overlap."""
    t = _unqualified(t)
    if isinstance(t, c99_ast.Array):
        e = _size(t.element_type, types)
        if not e or k < 0 or k >= e * t.size:
            return None
        return scalar_at(t.element_type, k % e, n, types)
    if isinstance(t, c99_ast.Structure):
        layout = types.get(t.tag) if types else None
        if layout is None or not layout.complete:
            return None
        for m in layout.members:
            size = _size(m.type, types)
            if size is not None and m.byte_offset <= k < m.byte_offset + size:
                return scalar_at(m.type, k - m.byte_offset, n, types)
        return None
    if isinstance(t, (c99_ast.Union, c99_ast.Void, c99_ast.FunType)):
        return None
    if k == 0 and _size(t, types) == n:
        return t
    return None


def store_keeps_pointer(ptr_type, stored, ) -> bool:
    """Whether a store of the scalar type `stored` is undefined when it
    overlaps an object of the pointer type `ptr_type` (C23 §6.5p7)."""
    stored = _unqualified(stored)
    if isinstance(stored, _CHARACTER_TYPES):
        return False
    if not isinstance(stored, c99_ast.Pointer):
        return True
    a = _deep_unqualified(stored).referenced_type
    b = _deep_unqualified(ptr_type).referenced_type
    if isinstance(a, (c99_ast.Array, c99_ast.FunType)) or isinstance(
            b, (c99_ast.Array, c99_ast.FunType)):
        return False
    return a != b


def pointer_in_zero_page(op: asm_ast.Type_operand) -> bool:
    """Whether the pointer operand `op` is, or will be under `-O`, a
    pair of zero-page bytes that `(zp),Y` can read through."""
    from passes.asm_aliasing import is_user_zp_global
    if isinstance(op, asm_ast.Pseudo):
        return True
    if isinstance(op, asm_ast.Data):
        return (op.name.startswith(("__zpabi_", "__local_"))
                or is_user_zp_global(op.name))
    return False


def restage_store(ptr_type, offset: int, n: int, types) -> bool:
    """Whether a store of `n` bytes at `offset` through a pointer of
    type `ptr_type` may read the pointer again for each byte (see the
    module docstring)."""
    if n < 2 or ptr_type is None:
        return False
    ptr_type = _unqualified(ptr_type)
    if not isinstance(ptr_type, c99_ast.Pointer):
        return False
    stored = scalar_at(ptr_type.referenced_type, offset, n, types)
    if stored is None:
        return False
    return store_keeps_pointer(ptr_type, stored)
