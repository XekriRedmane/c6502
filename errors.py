"""The exception classes of the compiler's errors.

The compiler is written in C++ (cpp/), and `cpp_backend._exception`
raises the error that a pass of a compile reports as the Python
exception of the class it names, with its message, so that a caller of
compile.py's shims catches it by class. This module holds those
classes, which were defined beside the Python passes that raised them
until the Python compiler was removed. `TypeCheckError` is in
passes/type_check_error.py, which passes/runtime_layout.py derives
`RuntimeLayoutError` from, and `ElfError` is in binutils/elf.py, and
this module takes both from there. The others are the classes of
`cpp_backend._ERROR_MODULES`.

The C++ parser's syntax errors keep the names of Lark's, which the
Python parser was built on, and so their hierarchy too: `UnexpectedToken`,
`UnexpectedCharacters`, and `UnexpectedEOF` are `UnexpectedInput`s, and
those and `VisitError` are `LarkError`s. Lark's classes take other
arguments than a message, and these take the message alone.
"""
from __future__ import annotations

from binutils.elf import ElfError
from passes.type_check_error import TypeCheckError

__all__ = [
    "AbiSelectionError", "AssemblerError", "ConstantExpressionError",
    "ElfError", "FrameSizeError", "IdentifierResolutionError",
    "LabelResolutionError", "LarkError", "LayoutError", "LexError",
    "LinkError", "LoopLabelingError", "NumberError", "ParserError",
    "PreprocessorError", "RecursionSaveError", "RegallocError",
    "ScriptError", "TypeCheckError", "UnexpectedCharacters",
    "UnexpectedEOF", "UnexpectedInput", "UnexpectedToken", "UnrollError",
    "VisitError", "ZpGlobalError", "ZpLocalAllocationError",
    "ZpSlotAllocationError",
]


# --- Lark's syntax errors -------------------------------------------------


class LarkError(Exception):
    """The base of the parser's syntax errors, as Lark's is."""


class UnexpectedInput(LarkError):
    """A syntax error at a place in the source."""


class UnexpectedToken(UnexpectedInput):
    """A token the grammar doesn't take where it is."""


class UnexpectedCharacters(UnexpectedInput):
    """Characters that no terminal of the grammar matches."""


class UnexpectedEOF(UnexpectedInput):
    """The end of the source where the grammar takes more."""


class VisitError(LarkError):
    """An error that Lark raised from a transformer's callback, which a
    few tests still accept beside the error itself."""


# --- The front end ----------------------------------------------------------


class PreprocessorError(Exception):
    """A `#pragma c6502 ...` line, or a number in an `#if` that isn't
    an integer constant. c6502's directives are C23 statement
    attributes now, and the message for a pragma names the one to
    write instead. Each message names the line in its source file."""


class NumberError(ValueError):
    """A pp-number that isn't a valid constant. The message names the
    spelling and what's wrong with it."""


class LexError(Exception):
    """A token the lexer can't read. The message starts with the
    token's `line:col`."""


class ParserError(Exception):
    """A declaration that the grammar accepts but C forbids, such as one
    with several type specifiers or several storage classes, or one
    with no type specifier (C99 dropped C89's implicit int)."""


class UnrollError(Exception):
    """A loop marked `[[c6502::unroll]]` did not match the canonical
    shape the unroller can handle."""


class IdentifierResolutionError(Exception):
    """A duplicate declaration, or a use of an undeclared name."""


class LabelResolutionError(Exception):
    """A duplicate label, or a `goto` to a label that isn't defined."""


class LoopLabelingError(Exception):
    """A `break` or `continue` outside the statement it needs, a `case`
    or `default` outside any switch, or a second `default` in one
    switch."""


class ConstantExpressionError(Exception):
    """An expression that must be a constant expression (C99 §6.6)
    and doesn't meet that section's constraints."""


# --- Code generation --------------------------------------------------------


class AbiSelectionError(Exception):
    """A `[[c6502::zp_abi]]` annotation that can't be honored: the
    function makes an indirect call, sits on a cycle in the direct call
    graph (recursion), has its address taken, or has too many parameter
    bytes."""


class RecursionSaveError(Exception):
    """`[[c6502::save_hardware_stack]]` on a statement it can't apply
    to."""


class RegallocError(Exception):
    """A hard constraint the byte-granular register allocator can't
    honor, such as a `register` local whose uses don't fit the register
    it needs."""


class ZpGlobalError(TypeCheckError):
    """A zero-page object that can't be placed."""


class ZpSlotAllocationError(Exception):
    """A zp_abi function whose slots can't be placed, because no
    contiguous range of the size they take fits in zero page or the
    spill region, or because the call graph has a cycle."""


class ZpLocalAllocationError(Exception):
    """An eligible function's local pool that can't be placed, because
    no contiguous gap of the size it takes is free in the zero-page
    window or the spill region."""


class FrameSizeError(ValueError):
    """A function's soft-stack frame that is too large for `LDY #offset`
    to reach. It's a limit of the program being compiled, so the driver
    reports it as a compile error."""


# --- The object-file tools --------------------------------------------------


class AssemblerError(Exception):
    """A line the assembler can't assemble."""


class LinkError(Exception):
    """A program that can't be linked."""


class ScriptError(Exception):
    """A linker script that isn't in the supported subset."""


class LayoutError(Exception):
    """A program the linker script can't lay out."""
