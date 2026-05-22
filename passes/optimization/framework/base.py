from __future__ import annotations
import abc
from dataclasses import dataclass
from typing import ClassVar
import tac_ast

@dataclass
class PassContext:
    """Threaded into every Pass.run call. Fields the pass doesn't use
    are simply ignored. Extending the context is additive — add a
    field, no driver change required.

    symbols:  the type checker's SymbolTable. None in legacy unit-test
              callers that run optimize_function on synthetic fns
              without a symbol table.
    ssa_dsts: set of SSA-renamed Var names (populated by to_ssa).
              Consumed by passes that distinguish SSA-renamed names
              from static/global/address-taken locals.
    """
    symbols: object | None = None        # SymbolTable — kept untyped to avoid import cycle
    ssa_dsts: set[str] | None = None

class Pass(abc.ABC):
    """Optimizer pass. Pure function on tac_ast.Function (per-function
    passes) or tac_ast.Program (ProgramPass)."""
    name: ClassVar[str]

    @abc.abstractmethod
    def run(self, fn: tac_ast.Function, ctx: PassContext) -> tac_ast.Function: ...
