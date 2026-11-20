"""Designated initializers (C99 §6.7.8, C23 §6.7.11).

The parser leaves an item such as `[2] = v`, `.x = v`, or `.a[2].b = v`
in a braced list as a c99 `Designation`, with an array designator's
index already folded to an integer. `normalize_initializer` rewrites a
list with designations into the positional form the rest of the
compiler handles. That form has one item per subobject, in order, with
the empty initializer `{}` for each subobject that nothing initializes,
which default-initializes it (zero, or a null pointer). A union's list
holds one item, and `InitList.union_member` names the member it
initializes.

    struct p { int x; int y; };
    struct s { int n; struct p pt[2]; };
    struct s v = {.pt[1].y = 7, .n = 3};

becomes

    struct s v = {3, { {}, {{}, 7} }};

string_lifting calls this first, because it's the first pass that
dispatches each item of a list by the type it initializes, so no pass
after it sees a `Designation`. The type checker calls it too, for the
lists the parser checks while folding a `constexpr` object.

The rules come from §6.7.11p17-22.

- A designation starts at the list's own object. After it, a
  positional item initializes the subobject after the one the first
  designator names. When the designation has more than one designator,
  the item after it would continue inside a subaggregate, which takes
  brace elision. c6502 doesn't elide braces, so that's an error that
  says to brace the subaggregate.
- A later initializer of a subobject overrides an earlier one. When
  the later one is a designation that reaches inside a subaggregate,
  it overrides only the part it names, and the rest of the earlier
  initializer stays. An overridden initializer is never evaluated. C
  leaves it unspecified whether its side effects happen (§6.7.11p19),
  and c6502 rejects an overridden initializer that has any, which is
  an assignment, an increment or decrement, or a call. A read of a
  volatile object in one is dropped without an error, since the types
  of names aren't known here.
- A designation that initializes one member of a union replaces any
  earlier initializer of another member.
- An array of unknown size takes its size from the largest index
  initialized.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Callable

import c99_ast
from passes.dataclass_fields import fields_of


# `layout_of(tag)` gives the members of the struct or union `tag` as
# `(name, type)` pairs, in declaration order, or None when the tag has
# no body.
LayoutOf = Callable[[str], "list[tuple[str, c99_ast.Type_data_type]] | None"]


def has_designation(init) -> bool:
    """Returns whether the braced list `init`, or a list nested in it,
    has a designation."""
    if isinstance(init, c99_ast.Designation):
        return True
    if isinstance(init, c99_ast.InitList):
        return any(has_designation(it) for it in init.items)
    return False


def normalize_initializer(
    init, t: c99_ast.Type_data_type, layout_of: LayoutOf,
    what: str | None,
):
    """Returns the initializer `init` of an object of type `t` with
    every designation rewritten into positional form. A list with no
    designation in it comes back unchanged, as does an initializer
    that isn't a list. `what` names the object in error messages, as
    the program spells it, and is None for a compound literal, which
    the messages call `(T){...}`.

    Raises TypeCheckError for a designator that doesn't fit the type
    it applies to, an array index past the end, a positional item
    that would need brace elision, and an overridden initializer with
    side effects."""
    if not isinstance(init, c99_ast.InitList) or not has_designation(init):
        return init
    if what is None:
        what = f"({_c_type(t)}){{...}}"
    return _Normalizer(layout_of).list(init, t, what)


def _error(message: str) -> Exception:
    # Imported here because the type checker imports this module.
    from passes.type_checking import TypeCheckError
    return TypeCheckError(message)


def _c_type(t) -> str:
    from passes.type_checking import _c_type as c_type
    return c_type(t)


def _strip_quals(t):
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return t


def _is_aggregate(t) -> bool:
    return isinstance(
        _strip_quals(t),
        (c99_ast.Array, c99_ast.Structure, c99_ast.Union),
    )


def _spell(designators) -> str:
    """Returns the designators as the program writes them, such as
    `.a[2].b`."""
    return "".join(
        f"[{d.index}]" if isinstance(d, c99_ast.IndexDesignator)
        else f".{d.member}"
        for d in designators
    )


@dataclass
class _Partial:
    """An aggregate whose initializer is being put together. `slots`
    maps the index of an element or member to its initializer, which
    is an expression, an InitList, or another `_Partial`. For a union,
    `slots` has at most one entry, and that's the member initialized."""
    t: c99_ast.Type_data_type
    path: str
    slots: dict = field(default_factory=dict)


class _Normalizer:
    def __init__(self, layout_of: LayoutOf) -> None:
        self._layout_of = layout_of

    def list(self, init: c99_ast.InitList, t, path: str):
        """Returns the positional form of the list `init`, which
        initializes the object `path` of type `t`."""
        t_unq = _strip_quals(t)
        if not _is_aggregate(t_unq):
            for item in init.items:
                if isinstance(item, c99_ast.Designation):
                    raise _error(
                        f"`{path}` is a scalar of type `{_c_type(t)}`, so "
                        f"its initializer can't have the designator "
                        f"`{_spell(item.designators)}`, which names an "
                        f"element of an array or a member of a struct or "
                        f"union (C23 §6.7.11p6-7)"
                    )
            # A scalar's braces hold one expression. A list that's
            # nested in them is left for the type checker to reject.
            return init
        partial = _Partial(t=t_unq, path=path)
        self._fill(partial, init)
        return self._emit(partial)

    # ------------------------------------------------------------------
    # Putting the initializer together
    # ------------------------------------------------------------------

    def _fill(self, partial: _Partial, init: c99_ast.InitList) -> None:
        """Applies the items of the list `init` to `partial`, in order."""
        pos = 0
        chain = None
        for item in init.items:
            if isinstance(item, c99_ast.Designation):
                pos = self._place(partial, item.designators, item.init) + 1
                chain = (
                    item.designators if len(item.designators) > 1 else None
                )
                continue
            if chain is not None:
                inner = _spell(chain[:1])
                raise _error(
                    f"in the initializer of `{partial.path}`, the item "
                    f"after `{_spell(chain)} = ...` would initialize the "
                    f"subobject after `{partial.path}{_spell(chain)}` "
                    f"inside `{partial.path}{inner}`, which takes brace "
                    f"elision, and c6502 doesn't elide braces. Put the "
                    f"initializers of `{partial.path}{inner}` in braces "
                    f"of their own, as in `{inner} = {{...}}`"
                )
            if isinstance(partial.t, c99_ast.Union) and pos > 0:
                raise _error(
                    f"too many initializers for union `{partial.path}`: "
                    f"a union's list initializes one member, and an item "
                    f"without a designator can't follow another item "
                    f"(C23 §6.7.11p17)"
                )
            self._check_in_range(partial, pos)
            self._set(partial, pos, item)
            pos += 1

    def _check_in_range(self, partial: _Partial, k: int) -> None:
        """Raises an error when the positional item at index `k` is
        past the end of `partial`."""
        t = partial.t
        if isinstance(t, c99_ast.Array):
            if t.size and k >= t.size:
                raise _error(
                    f"too many initializers for array `{partial.path}`: "
                    f"the item for index {k} is past the end of an array "
                    f"of {t.size} element{'s' if t.size != 1 else ''}"
                )
            return
        members = self._members(partial)
        if k >= len(members):
            raise _error(
                f"too many initializers for struct `{partial.path}`: "
                f"the struct has {len(members)} member"
                f"{'s' if len(members) != 1 else ''}, and the item after "
                f"the last one has no member to initialize"
            )

    def _members(self, partial: _Partial):
        """Returns the `(name, type)` pairs of the struct or union
        `partial`."""
        members = self._layout_of(partial.t.tag)
        if members is None:
            kw = "union" if isinstance(partial.t, c99_ast.Union) else "struct"
            raise _error(
                f"`{partial.path}` has the incomplete type `{kw} "
                f"{partial.t.tag}`, so it can't be initialized"
            )
        return members

    def _step(self, t, d, path: str):
        """Returns `(index, type, path)` of the subobject the designator
        `d` names in the object `path` of type `t`."""
        t_unq = _strip_quals(t)
        if isinstance(d, c99_ast.IndexDesignator):
            if not isinstance(t_unq, c99_ast.Array):
                raise _error(
                    f"the designator `[{d.index}]` names an element of an "
                    f"array, and `{path}` has type `{_c_type(t)}` (C23 "
                    f"§6.7.11p6)"
                )
            if t_unq.size and d.index >= t_unq.size:
                raise _error(
                    f"the designator `[{d.index}]` is past the end of "
                    f"`{path}`, an array of {t_unq.size} element"
                    f"{'s' if t_unq.size != 1 else ''} (C23 §6.7.11p6)"
                )
            return d.index, t_unq.element_type, f"{path}[{d.index}]"
        if not isinstance(t_unq, (c99_ast.Structure, c99_ast.Union)):
            raise _error(
                f"the designator `.{d.member}` names a member of a struct "
                f"or union, and `{path}` has type `{_c_type(t)}` (C23 "
                f"§6.7.11p7)"
            )
        members = self._members(_Partial(t=t_unq, path=path))
        for k, (name, m_type) in enumerate(members):
            if name == d.member:
                return k, m_type, f"{path}.{d.member}"
        raise _error(
            f"the designator `.{d.member}` names no member of `{path}`, "
            f"which has type `{_c_type(t)}` (C23 §6.7.11p7)"
        )

    def _place(self, partial: _Partial, designators, init) -> int:
        """Applies the designation `designators = init` to `partial`
        and returns the index of the subobject its first designator
        names."""
        k, sub_t, sub_path = self._step(partial.t, designators[0], partial.path)
        if len(designators) == 1:
            self._set(partial, k, init)
            return k
        # Check the next designator against the subobject first, so a
        # wrong one is reported before anything is overridden.
        self._step(sub_t, designators[1], sub_path)
        if isinstance(partial.t, c99_ast.Union):
            self._drop_other_members(partial, k)
        sub = self._as_partial(partial.slots.get(k), sub_t, sub_path)
        self._place(sub, designators[1:], init)
        partial.slots[k] = sub
        return k

    def _set(self, partial: _Partial, k: int, init) -> None:
        """Makes `init` the initializer of subobject `k` of `partial`,
        overriding whatever initialized it before."""
        if isinstance(partial.t, c99_ast.Union):
            self._drop_other_members(partial, k)
        old = partial.slots.get(k)
        if old is not None:
            self._discard(old, self._sub_path(partial, k))
        partial.slots[k] = init

    def _drop_other_members(self, partial: _Partial, k: int) -> None:
        """Discards the initializer of each member of the union
        `partial` other than member `k`, since a union holds one."""
        for j in [j for j in partial.slots if j != k]:
            self._discard(partial.slots.pop(j), self._sub_path(partial, j))

    def _sub_path(self, partial: _Partial, k: int) -> str:
        if isinstance(partial.t, c99_ast.Array):
            return f"{partial.path}[{k}]"
        return f"{partial.path}.{self._members(partial)[k][0]}"

    def _as_partial(self, old, t, path: str) -> _Partial:
        """Returns the subobject's earlier initializer `old` as a
        `_Partial` that a later designation can override part of."""
        if isinstance(old, _Partial):
            return old
        partial = _Partial(t=_strip_quals(t), path=path)
        if old is None:
            return partial
        if isinstance(old, c99_ast.InitList):
            self._fill(partial, old)
            return partial
        what = (
            "a string literal" if isinstance(old, c99_ast.String)
            else "an expression"
        )
        raise _error(
            f"a designator overrides part of `{path}`, which {what} "
            f"initializes, and c6502 can only override part of a "
            f"subobject that a braced list initializes. Initialize "
            f"`{path}` with a braced list instead"
        )

    def _discard(self, old, path: str) -> None:
        """Drops the overridden initializer `old` of `path`, which
        must have no side effects."""
        if _has_side_effects(old):
            raise _error(
                f"a later designator overrides the initializer of "
                f"`{path}`, which has side effects. C leaves it "
                f"unspecified whether they happen (C23 §6.7.11p19), and "
                f"c6502 doesn't evaluate an overridden initializer, so "
                f"it rejects one with side effects"
            )

    # ------------------------------------------------------------------
    # Writing the positional form
    # ------------------------------------------------------------------

    def _emit(self, partial: _Partial) -> c99_ast.InitList:
        """Returns the positional InitList for `partial`."""
        t = partial.t
        if isinstance(t, c99_ast.Array):
            sub_types = None
            elem = t.element_type
        else:
            members = self._members(partial)
            sub_types = [m_type for _name, m_type in members]
        if isinstance(t, c99_ast.Union):
            (k, item), = partial.slots.items()
            name, m_type = members[k]
            return c99_ast.InitList(
                items=[self._emit_item(item, m_type, f"{partial.path}.{name}")],
                union_member=name,
            )
        n = max(partial.slots) + 1 if partial.slots else 0
        items = []
        for k in range(n):
            sub_t = elem if sub_types is None else sub_types[k]
            items.append(self._emit_item(
                partial.slots.get(k), sub_t, self._sub_path(partial, k),
            ))
        return c99_ast.InitList(items=items)

    def _emit_item(self, item, t, path: str):
        if item is None:
            return c99_ast.InitList(items=[])
        if isinstance(item, _Partial):
            return self._emit(item)
        if isinstance(item, c99_ast.InitList) and has_designation(item):
            return self.list(item, t, path)
        return item


def _has_side_effects(node) -> bool:
    """Returns whether evaluating the initializer `node` assigns,
    increments, decrements, or calls a function. The operand of
    `sizeof` isn't evaluated, so it doesn't count."""
    if isinstance(node, _Partial):
        return any(_has_side_effects(v) for v in node.slots.values())
    if isinstance(node, (
        c99_ast.Assignment, c99_ast.CompoundAssignment, c99_ast.Prefix,
        c99_ast.Postfix, c99_ast.FunctionCall, c99_ast.IndirectCall,
    )):
        return True
    if isinstance(node, (c99_ast.SizeOfExp, c99_ast.SizeOfType)):
        return False
    if isinstance(node, list):
        return any(_has_side_effects(n) for n in node)
    if isinstance(node, c99_ast.Type_exp):
        return any(
            _has_side_effects(getattr(node, f.name))
            for f in fields_of(node)
        )
    return False
