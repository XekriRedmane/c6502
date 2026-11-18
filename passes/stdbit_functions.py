"""Definitions of the <stdbit.h> functions a unit refers to by name.

<stdbit.h> declares each function with a suffix, such as
`unsigned int stdc_leading_zeros_uc(unsigned char value);`, and
defines its name as a macro for the builtin
`__c6502_stdc_leading_zeros_uc` (docs/stdbit.md). A call written the
ordinary way expands to the builtin, which is lowered in place. C23
§7.1.4 also lets a program reach the function itself, by taking its
address, by calling `(stdc_leading_zeros_uc)(x)`, whose parentheses
keep the macro from expanding, or by calling it after `#undef`. c6502
has no library to hold the functions, so `add_definitions` writes a
definition of each one a unit refers to that way, whose body returns
the builtin of its parameter, and the type checker gives it internal
linkage.

A definition is added only for a name that a file-scope declaration
without `static` declares as a function, so a program that doesn't
include <stdbit.h> can still use the names for its own static
functions and objects. A name the unit defines itself gets no second
definition.

The declarations of the functions the unit doesn't refer to are
removed, all 70 of them in a unit that calls none, since the later
passes would give each one zero-page parameter slots, `EQU` lines,
and a line of link metadata.

Each unit that refers to a function has its own copy, at its own
address. C23 has one function with external linkage, so a pointer to
it from one unit compares unequal to a pointer from another, which is
the one way this falls short of the standard.
"""
from __future__ import annotations

import dataclasses

import builtin_functions
import c99_ast

def parameter(name: str) -> str:
    """The resolved name of the parameter of the definition of `name`.
    The symbol table holds every parameter by its resolved name, so
    each definition needs its own. The `@` and the letters after it
    keep it apart from every name identifier_resolution makes, which
    have digits after the `@`."""
    return f"@{name}.value"


def _referenced_names(node, names: set[str]) -> None:
    """Adds to `names` every name a `Var` or `FunctionCall` in `node`
    refers to."""
    if isinstance(node, (c99_ast.Var, c99_ast.FunctionCall)):
        names.add(node.name)
    if isinstance(node, list):
        for item in node:
            _referenced_names(item, names)
    elif dataclasses.is_dataclass(node) and not isinstance(node, type):
        for f in dataclasses.fields(node):
            value = getattr(node, f.name)
            if isinstance(value, list) or dataclasses.is_dataclass(value):
                _referenced_names(value, names)


def add_definitions(decls: list) -> list[str]:
    """Appends to `decls`, a program's file-scope declarations, a
    definition of each <stdbit.h> function the program declares with
    external linkage, refers to, and doesn't define, and returns the
    names it defined. Removes from `decls` the declarations of the
    <stdbit.h> functions the program doesn't refer to."""
    declared: dict[str, c99_ast.Type_function_decl] = {}
    defined: set[str] = set()
    for d in decls:
        if not isinstance(d, c99_ast.FunctionDecl):
            continue
        fd = d.function_decl
        if fd.name not in builtin_functions.STDBIT_FUNCTIONS:
            continue
        if fd.body is not None:
            defined.add(fd.name)
        elif not isinstance(fd.storage_class, c99_ast.Static):
            declared.setdefault(fd.name, fd)
    if not declared:
        return []
    referenced: set[str] = set()
    for d in decls:
        if isinstance(d, c99_ast.FunctionDecl):
            if d.function_decl.body is not None:
                _referenced_names(d.function_decl.body, referenced)
        else:
            _referenced_names(d, referenced)
    decls[:] = [
        d for d in decls
        if not (
            isinstance(d, c99_ast.FunctionDecl)
            and d.function_decl.body is None
            and d.function_decl.name in builtin_functions.STDBIT_FUNCTIONS
            and d.function_decl.name not in referenced
        )
    ]
    added = []
    for name, fd in declared.items():
        if name in defined or name not in referenced:
            continue
        builtin = builtin_functions.STDBIT_FUNCTIONS[name]
        param = parameter(name)
        call = c99_ast.FunctionCall(
            name=builtin, args=[c99_ast.Var(name=param)],
        )
        decls.append(c99_ast.FunctionDecl(
            function_decl=c99_ast.Type_function_decl(
                name=name, params=[param],
                body=c99_ast.Block(block_item=[
                    c99_ast.S(statement=c99_ast.Return(exp=call)),
                ]),
                data_type=fd.data_type, storage_class=None,
                abi_annotation=None,
                param_registers=[""],
                carry_return=False, address=None, preserves=[],
                section=None, noreturn=False,
            ),
        ))
        added.append(name)
    return added
