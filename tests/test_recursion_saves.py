"""Tests for `#pragma c6502 call save(hardware_stack)`: a recursive
function whose recursive calls are marked gets a private zero-page
pool, and saves what's live across each marked call on the hardware
stack (`passes/recursion_saves.py`)."""
from __future__ import annotations

import unittest

import c99_ast
import tac_ast
from compile import _run_stage
from parser import ParserError, parse
from passes.abi_selection import SoftStackLayout
from passes.recursion_saves import RecursionSaveError, hw_stack_cycles
from passes.zp_local_allocation import allocate_function_locals
from preprocessor import preprocess
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac

_PRAGMA = "#pragma c6502 call save(hardware_stack)\n"


def _codegen(src: str, optimize: bool = True) -> str:
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _function(asm: str, name: str) -> str:
    """The text of function `name` in `asm`, up to the next one."""
    lines = asm.splitlines()
    start = lines.index(f"{name}:")
    end = start + 1
    while end < len(lines) and not (
        lines[end] and not lines[end][0].isspace()
        and lines[end].endswith(":") and not lines[end].startswith(".")
    ):
        end += 1
    return "\n".join(lines[start:end])


def _calls(node) -> list[c99_ast.FunctionCall]:
    """Every c99 FunctionCall in `node`."""
    import dataclasses
    out = []
    if isinstance(node, list):
        for x in node:
            out += _calls(x)
    elif dataclasses.is_dataclass(node):
        if isinstance(node, c99_ast.FunctionCall):
            out.append(node)
        for f in dataclasses.fields(node):
            out += _calls(getattr(node, f.name))
    return out


_DESCEND = """
unsigned char level;
unsigned char deepest;
unsigned char remaining;

void descend(void) {
    unsigned char here = level;
    if (here > deepest) deepest = here;
    if (remaining != 0) {
        remaining = remaining - 1;
        level = here + 1;
PRAGMA        descend();
        level = here;
    }
}

int main(void) {
    remaining = 5;
    descend();
    return deepest * 10 + level;
}
"""


class TestSyntax(unittest.TestCase):

    def test_preprocessor_sentinel(self) -> None:
        out = preprocess("void f(void) {\n" + _PRAGMA + "f();\n}\n")
        self.assertIn("__c6502_pragma_hw_stack_save__", out)

    def test_marks_the_calls_after_it(self) -> None:
        for stmt in ("x = f(1) + g();", "return f(1) + g();",
                     "int x = f(1) + g();"):
            with self.subTest(stmt):
                src = ("int f(int); int g(void);\nint h(void) {\n"
                       "int x = 0;\n" + _PRAGMA + stmt
                       + "\nreturn f(2);\n}\n")
                calls = _calls(parse(preprocess(src)))
                marked = [(c.name, c.hw_stack_save) for c in calls]
                self.assertEqual(
                    marked, [("f", True), ("g", True), ("f", False)],
                )

    def test_rejected_positions(self) -> None:
        for stmt, needle in (
            ("if (f(1)) x = 1;", "must come before an expression"),
            ("{ f(1); }", "must come before an expression"),
            ("x = 1;", "calls a function"),
            ("int y = 2;", "calls a function"),
        ):
            with self.subTest(stmt):
                src = ("int f(int);\nvoid h(void) {\nint x;\n"
                       + _PRAGMA + stmt + "\n}\n")
                with self.assertRaises(ParserError) as cm:
                    parse(preprocess(src))
                self.assertIn(needle, str(cm.exception))

    def test_call_that_cant_recurse(self) -> None:
        src = ("int g(int x) { return x; }\nint f(int x) {\n"
               + _PRAGMA + "return g(x);\n}\n")
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                with self.assertRaises(RecursionSaveError) as cm:
                    _codegen(src, optimize)
                self.assertIn("'g' can't call 'f' again", str(cm.exception))

    def test_call_through_a_pointer(self) -> None:
        src = ("int f(int x) {\nint (*p)(int) = f;\nif (x == 0) return 0;\n"
               + _PRAGMA + "return p(x - 1);\n}\n")
        with self.assertRaises(RecursionSaveError) as cm:
            _codegen(src)
        self.assertIn("function pointer", str(cm.exception))


def _fn(name: str, *calls: tuple[str, bool]) -> tac_ast.Function:
    return tac_ast.Function(
        name=name, is_global=True, params=[],
        instructions=[
            tac_ast.FunctionCall(name=c, args=[], dst=None, hw_stack_save=m)
            for c, m in calls
        ],
    )


class TestCycles(unittest.TestCase):

    def test_every_call_on_the_cycle_must_be_marked(self) -> None:
        prog = tac_ast.Program(top_level=[
            _fn("main", ("f", False), ("p", False)),
            _fn("f", ("g", True), ("leaf", False)),
            _fn("g", ("f", True)),
            _fn("p", ("q", True)),
            _fn("q", ("p", False)),
            _fn("leaf"),
        ])
        self.assertEqual(
            hw_stack_cycles(prog),
            {"f": frozenset({"f", "g"}), "g": frozenset({"f", "g"})},
        )

    def test_pools(self) -> None:
        prog = tac_ast.Program(top_level=[
            _fn("main", ("f", False)),
            _fn("f", ("g", True), ("leaf", False)),
            _fn("g", ("f", True)),
            _fn("leaf"),
        ])
        abi = {n: SoftStackLayout() for n in ("main", "f", "g", "leaf")}
        pools = allocate_function_locals(
            prog, abi, {"main": 2, "f": 2, "g": 2, "leaf": 2},
        )
        self.assertEqual(set(pools), {"main", "f", "g", "leaf"})
        # f and g may share: every call between them saves first.
        self.assertEqual(pools["f"], pools["g"])
        for name in ("f", "g", "leaf"):
            self.assertFalse(set(pools["main"]) & set(pools[name]))
        self.assertFalse(set(pools["f"]) & set(pools["leaf"]))

    def test_unmarked_cycle_gets_no_pool(self) -> None:
        prog = tac_ast.Program(top_level=[
            _fn("main", ("f", False)),
            _fn("f", ("f", False)),
        ])
        abi = {n: SoftStackLayout() for n in ("main", "f")}
        pools = allocate_function_locals(prog, abi, {"main": 1, "f": 1})
        self.assertEqual(pools, {})


class TestCodegen(unittest.TestCase):

    def test_saves_around_the_call(self) -> None:
        body = _function(
            _codegen(_DESCEND.replace("PRAGMA", _PRAGMA)), "descend",
        )
        self.assertNotIn("prologue", body)
        self.assertIn(
            "   LDA   __local_descend__here\n   PHA\n   JSR   descend\n"
            "   PLA\n   STA   level\n",
            body,
        )

    def test_without_the_pragma(self) -> None:
        body = _function(
            _codegen(_DESCEND.replace("PRAGMA", "")), "descend",
        )
        self.assertIn("callee-saved", body)
        self.assertNotIn("PHA", body)

    def test_pushes_from_and_pulls_into_globals(self) -> None:
        src = """
unsigned char a, b, depth;
void load(void) { depth++; }
void f(void) {
    if (depth < 2) {
        unsigned char sa = a;
        unsigned char sb = b;
        load();
PRAGMA        f();
        b = sb;
        a = sa;
    }
}
int main(void) { f(); return depth; }
""".replace("PRAGMA", _PRAGMA)
        body = _function(_codegen(src), "f")
        self.assertIn(
            "   LDA   a\n   PHA\n   LDA   b\n   PHA\n   JSR   load\n"
            "   JSR   f\n   PLA\n   STA   b\n   PLA\n   STA   a\n",
            body,
        )

    def test_caller_keeps_zp_abi(self) -> None:
        # rec has a pool, so f0 can call it and read its own
        # parameter slot afterwards.
        src = _PROGRAMS["zp_abi caller"].replace("PRAGMA", _PRAGMA)
        body = _function(_codegen(src), "f0")
        self.assertNotIn("prologue", body)
        self.assertIn("JSR   rec", body)
        self.assertIn("__zpabi_f0__p0_0", body.split("JSR   rec")[1])

    def test_unmarked_call_on_the_cycle(self) -> None:
        src = """
int pong(int n);
int ping(int n) {
    int a = n * 2;
    if (n <= 0) return 0;
PRAGMA    int r = pong(n - 1);
    return r + a;
}
int pong(int n) {
    int b = n + 5;
    if (n <= 0) return 1;
    int r = ping(n - 1);
    return r * 2 + b;
}
int main(void) { return ping(5); }
""".replace("PRAGMA", _PRAGMA)
        asm = _codegen(src)
        self.assertIn("callee-saved", _function(asm, "ping"))
        self.assertNotIn("PHA\n   JSR", _function(asm, "ping"))


_PROGRAMS = {
    "globals": _DESCEND,
    "locals and parameters": """
unsigned char depth;
unsigned char out[16];
unsigned char n;
void walk(unsigned char k) {
    unsigned char a = k * 3;
    unsigned char b = depth;
    unsigned int w = k * 100;
    out[n++] = a;
    if (k > 0) {
        depth = depth + 1;
PRAGMA        walk(k - 1);
        depth = b;
    }
    out[n++] = a + (unsigned char)w + b;
}
int fact(int x) {
    int y = x;
    if (x <= 1) return 1;
PRAGMA    y = y * fact(x - 1);
    return y;
}
int main(void) {
    walk(4);
    int s = 0;
    for (unsigned char i = 0; i < n; i++) s = s * 3 + out[i];
    return (s ^ fact(6)) & 0x7FFF;
}
""",
    "return and declaration": """
unsigned char weights[8] = {3, 1, 4, 1, 5, 9, 2, 6};
unsigned int sum_from(unsigned char i) {
    if (i >= 8) return 0;
    unsigned char w = weights[i];
PRAGMA    return w + sum_from(i + 1);
}
int even(unsigned char n);
int odd(unsigned char n) {
    if (n == 0) return 0;
    unsigned char k = n * 2;
PRAGMA    int r = even(n - 1);
    return r + k;
}
int even(unsigned char n) {
    if (n == 0) return 1;
    unsigned char k = n + 7;
PRAGMA    int r = odd(n - 1);
    return r * 3 + k;
}
int main(void) { return (int)sum_from(0) * 100 + even(6); }
""",
    "in a loop": """
unsigned int count_paths(unsigned char n) {
    unsigned int total = 0;
    if (n < 2) return 1;
    for (unsigned char k = 1; k <= 2; k++) {
PRAGMA        total += count_paths(n - k);
    }
    return total;
}
int main(void) { return count_paths(9); }
""",
    "address taken": """
void bump(unsigned char *p) { *p += 1; }
unsigned char walk2(unsigned char n) {
    unsigned char local = n;
    unsigned char keep = n * 3;
    bump(&local);
    if (n == 0) return local;
PRAGMA    unsigned char r = walk2(n - 1);
    return r + local + keep;
}
int main(void) { return walk2(5); }
""",
    "long": """
long acc(unsigned char n) {
    long big = (long)n * 70000;
    if (n == 0) return 1;
PRAGMA    long r = acc(n - 1);
    return r + big;
}
int main(void) { return (int)(acc(5) & 0x7FFF); }
""",
    "carry flag": """
#include <stdbool.h>
__attribute__((return_bool_in_carry))
bool any_bit(unsigned char n, unsigned char mask) {
    unsigned char h = n & mask;
    if (n == 0) return 0;
PRAGMA    bool r = any_bit(n >> 1, mask);
    return r || h == 3;
}
int main(void) {
    int s = 0;
    for (unsigned char i = 0; i < 40; i++) s = s * 2 + any_bit(i, 3);
    return s & 0x7FFF;
}
""",
    "zp_abi caller": """
int rec(int d) {
    int k = d * 3;
    if (d <= 0) return 0;
PRAGMA    int r = rec(d - 1);
    return r + 1 + (k & 1);
}
int f0(int p0) { int l = rec(3); return p0 + l; }
int main(void) { return f0(4); }
""",
    "unmarked call on the cycle": """
int pong(int n);
int ping(int n) {
    int a = n * 2;
    if (n <= 0) return 0;
PRAGMA    int r = pong(n - 1);
    return r + a;
}
int pong(int n) {
    int b = n + 5;
    if (n <= 0) return 1;
    int r = ping(n - 1);
    return r * 2 + b;
}
int main(void) { return ping(5); }
""",
}


class TestPrograms(unittest.TestCase):

    def test_programs(self) -> None:
        for name, src in _PROGRAMS.items():
            src = src.replace("PRAGMA", _PRAGMA)
            expected = _run_tac(src) & 0xFFFF
            for optimize in (False, True):
                with self.subTest(name, optimize=optimize):
                    result = build_sim(src, optimize=optimize).run(
                        max_cycles=5_000_000,
                    )
                    self.assertFalse(result.timed_out)
                    self.assertEqual(
                        result.return_int_signed() & 0xFFFF, expected,
                    )


if __name__ == "__main__":
    unittest.main()
