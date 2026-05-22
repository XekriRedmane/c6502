"""LLVM PatternMatch.h-style pattern combinators for TAC instructions.

Usage::

    from passes.optimization.framework.patterns import (
        match, m_Binary, m_Var, m_Constant, m_Any, m_OneOf,
        m_JumpIfTrue, m_JumpIfFalse, m_Unary, m_Specific,
    )

    result = match(
        m_Binary(op=tac_ast.Add, src1=m_Var(capture='x')),
        instr,
    )
    if result is not None:
        x = result.bindings['x']

Top-level entry point: ``match(pattern, node) -> MatchResult | None``.
Returns a ``MatchResult`` whose ``.bindings`` dict contains any
captured values, or ``None`` on failure.

Compound patterns (``m_Binary``, ``m_Unary``, etc.) snapshot/restore
the ``MatchResult`` around child-pattern calls so a partial sub-match
doesn't corrupt the bindings dict on overall failure.
"""
from __future__ import annotations
import abc
from dataclasses import dataclass, field
import tac_ast


@dataclass
class MatchResult:
    """Bindings dict built up as a Pattern tree is walked. Each
    Pattern.match call may add to bindings on success; on failure,
    the COMPOUND pattern that wraps it is responsible for restoring
    via snapshot/restore."""
    bindings: dict[str, object] = field(default_factory=dict)

    def snapshot(self) -> dict[str, object]:
        return dict(self.bindings)

    def restore(self, snap: dict[str, object]) -> None:
        self.bindings = snap


class Pattern(abc.ABC):
    """Returns True on match (any captures already applied to m);
    False on failure. The CALLER is responsible for snapshot/restore
    around child-pattern calls in compound patterns."""
    @abc.abstractmethod
    def match(self, node: object, m: MatchResult) -> bool: ...


def match(pattern: Pattern, node: object) -> MatchResult | None:
    """Top-level entry point. Returns MatchResult with bindings on
    match, None on failure."""
    m = MatchResult()
    if pattern.match(node, m):
        return m
    return None


# -- Operand patterns ----------------------------------------------

@dataclass
class m_Any(Pattern):
    """Wildcard. Matches anything. Captures the matched node when
    `capture` is set."""
    capture: str | None = None

    def match(self, node, m):
        if self.capture:
            m.bindings[self.capture] = node
        return True


@dataclass
class m_Constant(Pattern):
    """Match a tac_ast.Constant. `value` constrains to a specific
    underlying value; `variant` constrains to a specific const class
    (e.g. tac_ast.ConstInt). `capture` binds the whole Constant
    node."""
    value: int | float | None = None
    variant: type | tuple[type, ...] | None = None
    capture: str | None = None

    def match(self, node, m):
        if not isinstance(node, tac_ast.Constant):
            return False
        if self.variant is not None:
            variants = (
                self.variant
                if isinstance(self.variant, tuple)
                else (self.variant,)
            )
            if not isinstance(node.const, variants):
                return False
        if self.value is not None and node.const.value != self.value:
            return False
        if self.capture:
            m.bindings[self.capture] = node
        return True


@dataclass
class m_Var(Pattern):
    """Match a tac_ast.Var. `name` constrains to a specific name
    (rarely needed — backrefs use m_Specific instead). `capture`
    binds the whole Var."""
    name: str | None = None
    capture: str | None = None

    def match(self, node, m):
        if not isinstance(node, tac_ast.Var):
            return False
        if self.name is not None and node.name != self.name:
            return False
        if self.capture:
            m.bindings[self.capture] = node
        return True


@dataclass
class m_Specific(Pattern):
    """Backreference: match a node that is STRUCTURALLY EQUAL to the
    node previously bound to `name`. Relies on tac_ast dataclasses'
    __eq__. Fails (returns False) if `name` wasn't bound earlier."""
    name: str

    def match(self, node, m):
        bound = m.bindings.get(self.name)
        if bound is None:
            return False
        return node == bound


# -- Instruction patterns ------------------------------------------

@dataclass
class m_Binary(Pattern):
    """Match a tac_ast.Binary. `op` constrains to a specific op
    class (or tuple). `src1`/`src2`/`dst` are child patterns
    (default m_Any). `capture` binds the whole Binary node."""
    op: type | tuple[type, ...] | None = None
    src1: Pattern = field(default_factory=m_Any)
    src2: Pattern = field(default_factory=m_Any)
    dst: Pattern = field(default_factory=m_Any)
    capture: str | None = None

    def match(self, node, m):
        if not isinstance(node, tac_ast.Binary):
            return False
        if self.op is not None:
            ops = self.op if isinstance(self.op, tuple) else (self.op,)
            if not isinstance(node.op, ops):
                return False
        snap = m.snapshot()
        if not (
            self.src1.match(node.src1, m)
            and self.src2.match(node.src2, m)
            and self.dst.match(node.dst, m)
        ):
            m.restore(snap)
            return False
        if self.capture:
            m.bindings[self.capture] = node
        return True


@dataclass
class m_Unary(Pattern):
    """Match a tac_ast.Unary. `op` constrains to a specific op class
    (or tuple). `src`/`dst` are child patterns (default m_Any).
    `capture` binds the whole Unary node."""
    op: type | tuple[type, ...] | None = None
    src: Pattern = field(default_factory=m_Any)
    dst: Pattern = field(default_factory=m_Any)
    capture: str | None = None

    def match(self, node, m):
        if not isinstance(node, tac_ast.Unary):
            return False
        if self.op is not None:
            ops = self.op if isinstance(self.op, tuple) else (self.op,)
            if not isinstance(node.op, ops):
                return False
        snap = m.snapshot()
        if not (self.src.match(node.src, m) and self.dst.match(node.dst, m)):
            m.restore(snap)
            return False
        if self.capture:
            m.bindings[self.capture] = node
        return True


@dataclass
class m_Copy(Pattern):
    """Match a tac_ast.Copy. `src`/`dst` are child patterns (default
    m_Any). `capture` binds the whole Copy node."""
    src: Pattern = field(default_factory=m_Any)
    dst: Pattern = field(default_factory=m_Any)
    capture: str | None = None

    def match(self, node, m):
        if not isinstance(node, tac_ast.Copy):
            return False
        snap = m.snapshot()
        if not (self.src.match(node.src, m) and self.dst.match(node.dst, m)):
            m.restore(snap)
            return False
        if self.capture:
            m.bindings[self.capture] = node
        return True


@dataclass
class m_Cast(Pattern):
    """Match any of SignExtend / ZeroExtend / Truncate / IntToFloat /
    IntToDouble / FloatToInt / DoubleToInt / FloatToDouble /
    DoubleToFloat. Constrain with `kind` (single class or tuple)."""
    kind: type | tuple[type, ...] | None = None
    src: Pattern = field(default_factory=m_Any)
    dst: Pattern = field(default_factory=m_Any)
    capture: str | None = None
    _CAST_TYPES = None  # populated below; tuple of all cast classes

    def match(self, node, m):
        if not isinstance(node, m_Cast._CAST_TYPES):
            return False
        if self.kind is not None:
            kinds = self.kind if isinstance(self.kind, tuple) else (self.kind,)
            if not isinstance(node, kinds):
                return False
        snap = m.snapshot()
        if not (self.src.match(node.src, m) and self.dst.match(node.dst, m)):
            m.restore(snap)
            return False
        if self.capture:
            m.bindings[self.capture] = node
        return True


m_Cast._CAST_TYPES = (
    tac_ast.SignExtend, tac_ast.ZeroExtend, tac_ast.Truncate,
    tac_ast.IntToFloat, tac_ast.IntToDouble,
    tac_ast.FloatToInt, tac_ast.DoubleToInt,
    tac_ast.FloatToDouble, tac_ast.DoubleToFloat,
)


@dataclass
class m_JumpIfTrue(Pattern):
    """Match a tac_ast.JumpIfTrue. `condition` is a child pattern
    (default m_Any). `target` constrains the label string. `capture`
    binds the whole JumpIfTrue node."""
    condition: Pattern = field(default_factory=m_Any)
    target: str | None = None
    capture: str | None = None

    def match(self, node, m):
        if not isinstance(node, tac_ast.JumpIfTrue):
            return False
        if self.target is not None and node.target != self.target:
            return False
        snap = m.snapshot()
        if not self.condition.match(node.condition, m):
            m.restore(snap)
            return False
        if self.capture:
            m.bindings[self.capture] = node
        return True


@dataclass
class m_JumpIfFalse(Pattern):
    """Match a tac_ast.JumpIfFalse. `condition` is a child pattern
    (default m_Any). `target` constrains the label string. `capture`
    binds the whole JumpIfFalse node."""
    condition: Pattern = field(default_factory=m_Any)
    target: str | None = None
    capture: str | None = None

    def match(self, node, m):
        if not isinstance(node, tac_ast.JumpIfFalse):
            return False
        if self.target is not None and node.target != self.target:
            return False
        snap = m.snapshot()
        if not self.condition.match(node.condition, m):
            m.restore(snap)
            return False
        if self.capture:
            m.bindings[self.capture] = node
        return True


# -- Combinators ----------------------------------------------------

class m_OneOf(Pattern):
    """Try each alternative; first match wins. Bindings from failed
    alternatives are cleared via snapshot/restore."""

    def __init__(self, *alternatives: Pattern):
        self.alternatives = alternatives

    def match(self, node, m):
        snap = m.snapshot()
        for alt in self.alternatives:
            if alt.match(node, m):
                return True
            m.restore(snap)
        return False


@dataclass
class m_Commutative(Pattern):
    """Match a Binary where (lhs, rhs) matches src1/src2 OR src2/src1.
    Tries the direct order first; falls back to swapped on failure."""
    op: type | tuple[type, ...]
    lhs: Pattern
    rhs: Pattern
    dst: Pattern = field(default_factory=m_Any)
    capture: str | None = None

    def match(self, node, m):
        if not isinstance(node, tac_ast.Binary):
            return False
        ops = self.op if isinstance(self.op, tuple) else (self.op,)
        if not isinstance(node.op, ops):
            return False
        snap = m.snapshot()
        if (
            self.lhs.match(node.src1, m)
            and self.rhs.match(node.src2, m)
            and self.dst.match(node.dst, m)
        ):
            if self.capture:
                m.bindings[self.capture] = node
            return True
        m.restore(snap)
        if (
            self.lhs.match(node.src2, m)
            and self.rhs.match(node.src1, m)
            and self.dst.match(node.dst, m)
        ):
            if self.capture:
                m.bindings[self.capture] = node
            return True
        m.restore(snap)
        return False
