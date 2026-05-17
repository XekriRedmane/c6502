"""Behavioral tests for `passes.zp_link_metadata`.

Coverage:
  - `build_metadata` extracts the right defs / externs / calls
    from a small TAC program.
  - `format_metadata` + `parse_metadata` round-trip a
    representative LinkMetadata.
  - `parse_metadata` returns an empty LinkMetadata when the
    asm has no metadata block.
  - Malformed blocks raise ValueError with a useful message.
"""
from __future__ import annotations

import unittest

import tac_ast
from passes.abi_selection import SoftStackLayout, ZpLayout
from passes.zp_link_metadata import (
    ExternMeta,
    FunctionMeta,
    LinkMetadata,
    build_metadata,
    format_metadata,
    parse_metadata,
)


def _fn(name: str, *instrs) -> tac_ast.Function:
    return tac_ast.Function(
        name=name, is_global=True, params=[],
        instructions=list(instrs),
    )


def _call(name: str) -> tac_ast.FunctionCall:
    return tac_ast.FunctionCall(name=name, args=[], dst=None)


def _icall(ptr: tac_ast.Type_val) -> tac_ast.IndirectCall:
    return tac_ast.IndirectCall(ptr=ptr, args=[], dst=None)


def _prog(*tls) -> tac_ast.Program:
    return tac_ast.Program(top_level=list(tls))


def _zp(*addrs: int) -> ZpLayout:
    return ZpLayout(
        slot_symbols=[f"__zpabi_p{k}" for k in range(len(addrs))],
        addrs=list(addrs),
    )


class TestBuildMetadata(unittest.TestCase):
    def test_simple_program(self) -> None:
        prog = _prog(
            _fn("caller", _call("callee")),
            _fn("callee"),
        )
        abi = {
            "caller": _zp(0x80, 0x81),
            "callee": _zp(0x82, 0x83),
        }
        local_pools = {"caller": [0x84, 0x85], "callee": []}
        meta = build_metadata(prog, abi, local_pools)
        # Two defs, sorted by name (alphabetical).
        self.assertEqual(
            [d.name for d in meta.defs],
            ["callee", "caller"],
        )
        caller = next(d for d in meta.defs if d.name == "caller")
        self.assertEqual(caller.param_bytes, 2)
        self.assertEqual(caller.local_bytes, 2)
        self.assertFalse(caller.indirect)
        self.assertFalse(caller.in_cycle)
        # One call edge.
        self.assertEqual(meta.calls, [("caller", "callee")])
        # No externs.
        self.assertEqual(meta.externs, [])

    def test_extern_zp_abi(self) -> None:
        prog = _prog(_fn("main", _call("helper")))
        abi = {
            "main": SoftStackLayout(),
            "helper": _zp(0x80, 0x81),
        }
        local_pools = {}
        meta = build_metadata(prog, abi, local_pools)
        # helper is extern (in abi but no Function in prog).
        self.assertEqual(
            [e.name for e in meta.externs], ["helper"],
        )
        self.assertEqual(meta.externs[0].param_bytes, 2)

    def test_indirect_flag(self) -> None:
        ptr = tac_ast.Var(name="fp")
        prog = _prog(_fn("f", _icall(ptr)))
        meta = build_metadata(prog, {}, {})
        f = next(d for d in meta.defs if d.name == "f")
        self.assertTrue(f.indirect)

    def test_cycle_flag_self_recursion(self) -> None:
        prog = _prog(_fn("rec", _call("rec")))
        meta = build_metadata(prog, {}, {})
        rec = next(d for d in meta.defs if d.name == "rec")
        self.assertTrue(rec.in_cycle)

    def test_cycle_flag_mutual(self) -> None:
        prog = _prog(
            _fn("a", _call("b")),
            _fn("b", _call("a")),
        )
        meta = build_metadata(prog, {}, {})
        for name in ("a", "b"):
            d = next(d for d in meta.defs if d.name == name)
            self.assertTrue(d.in_cycle, f"{name} should be in cycle")


class TestRoundTrip(unittest.TestCase):
    def test_format_then_parse_matches(self) -> None:
        meta = LinkMetadata(
            defs=[
                FunctionMeta(
                    name="caller",
                    params=["__zpabi_caller__x_0", "__zpabi_caller__x_1"],
                    locals=[
                        "__local_caller__sprite_x",
                        "__local_caller__sprite_y",
                        "__local_caller__0",
                    ],
                    indirect=False, in_cycle=False,
                ),
                FunctionMeta(
                    name="other", params=[], locals=[],
                    indirect=True, in_cycle=False,
                ),
            ],
            externs=[ExternMeta(
                name="helper",
                params=[
                    "__zpabi_helper__a_0", "__zpabi_helper__a_1",
                    "__zpabi_helper__b_0", "__zpabi_helper__b_1",
                ],
            )],
            calls=[("caller", "helper"), ("other", "caller")],
        )
        text = "\n".join(format_metadata(meta))
        parsed = parse_metadata(text)
        self.assertEqual(parsed, meta)

    def test_empty_input(self) -> None:
        # No metadata block → empty LinkMetadata.
        parsed = parse_metadata("clear_page1:\n   RTS\n")
        self.assertEqual(parsed, LinkMetadata())

    def test_unknown_record_raises(self) -> None:
        text = (
            "; @zp-link-meta-begin\n"
            "; mystery something\n"
            "; @zp-link-meta-end\n"
        )
        with self.assertRaises(ValueError) as cm:
            parse_metadata(text)
        self.assertIn("unknown record kind", str(cm.exception))

    def test_unmatched_begin(self) -> None:
        with self.assertRaises(ValueError):
            parse_metadata("; @zp-link-meta-begin\n")

    def test_unmatched_end(self) -> None:
        with self.assertRaises(ValueError):
            parse_metadata("; @zp-link-meta-end\n")


if __name__ == "__main__":
    unittest.main()
