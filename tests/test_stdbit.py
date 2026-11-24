"""Tests for <stdbit.h>, C23's bit and byte utilities (C23 §7.18;
builtin_functions.py, docs/stdbit.md).

`builtin_functions.stdbit` is the model every other check compares
with, so it is checked first against a second model written from the
standard's definitions, which walks the bits by index. Then the front
end is checked for the types it gives and the errors it reports, the
folding of constant arguments in both pipelines and in constant
expressions, the header's macros and declarations, the functions a
program reaches through their addresses, the code a few calls compile
to, and the simulator's results in both pipelines: all 256 values of
an `unsigned char`, and a sweep of edge and random values of the
wider types, for every function in its type-generic form and its form
with a suffix.
"""
from __future__ import annotations

import random
import re
import shutil
import unittest
from pathlib import Path

import builtin_functions as bf
import tac_ast
from preprocessor import preprocess
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None

_TYPES = {
    "uc": ("unsigned char", 1),
    "us": ("unsigned short", 2),
    "ui": ("unsigned int", 2),
    "ul": ("unsigned long", 4),
    "ull": ("unsigned long long", 8),
}


def _codegen(src: str, optimize: bool) -> str:
    from compile import _run_stage
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _tac(src: str, optimize: bool) -> tac_ast.Program:
    """The TAC that `compile.py --tac` prints for `src`, read back into
    `tac_ast` objects. Its `pretty` text is the constructors' calls with
    their fields as keywords, so Python can evaluate it."""
    from compile import _run_stage
    return eval(_run_stage("tac", preprocess(src), optimize=optimize),
                vars(tac_ast))


def _body(asm: str, name: str) -> list[str]:
    """The instruction lines of function `name`, without the
    `SUBROUTINE` line, blank lines, or comments."""
    text = asm.split(f"\n{name}:\n")[1]
    out = []
    for line in text.splitlines():
        line = line.strip()
        if not line or line.startswith(";") or line == "SUBROUTINE":
            continue
        if line.endswith(":") and not line.startswith("."):
            break
        out.append(" ".join(line.split()))
    return out


def _result_type(operation: str, value_type: str) -> str:
    if operation in ("bit_floor", "bit_ceil"):
        return value_type
    if operation == "has_single_bit":
        return "bool"
    return "unsigned int"


def _by_index(operation: str, value: int, width: int) -> int:
    """`operation` computed from C23 §7.18's wording, one bit at a time.
    `msi(k)` is the bit at most significant index k (§7.18.1p3) and
    `lsi(k)` the bit at least significant index k (§7.18.1p4)."""
    def msi(k):
        return (value >> (width - 1 - k)) & 1

    def lsi(k):
        return (value >> k) & 1

    def run(bit, want):
        n = 0
        while n < width and bit(n) == want:
            n += 1
        return n

    def first(bit, want):
        for k in range(width):
            if bit(k) == want:
                return k + 1
        return 0

    ones = sum(lsi(k) for k in range(width))
    powers = [1 << k for k in range(width)]
    match operation:
        case "leading_zeros":
            return run(msi, 0)
        case "leading_ones":
            return run(msi, 1)
        case "trailing_zeros":
            return run(lsi, 0)
        case "trailing_ones":
            return run(lsi, 1)
        case "first_leading_zero":
            return first(msi, 0)
        case "first_leading_one":
            return first(msi, 1)
        case "first_trailing_zero":
            return first(lsi, 0)
        case "first_trailing_one":
            return first(lsi, 1)
        case "count_zeros":
            return width - ones
        case "count_ones":
            return ones
        case "has_single_bit":
            return int(ones == 1)
        case "bit_width":
            # "the smallest number of bits needed to store value"
            return next(n for n in range(width + 1) if value < 1 << n)
        case "bit_floor":
            return max((p for p in powers if p <= value), default=0)
        case "bit_ceil":
            return min((p for p in powers if p >= value), default=0)
    raise ValueError(operation)


def _sweep(width: int) -> list[int]:
    """Edge values of a `width`-bit unsigned type and a few random
    ones: 0, 1, the all-ones value, each power of 2 and its neighbors
    for every third exponent, and the top bit's neighbors."""
    bits = 8 * width
    top = 1 << (bits - 1)
    values = {0, 1, 2, 3, (1 << bits) - 1, (1 << bits) - 2, top,
              top - 1, top + 1}
    for k in range(0, bits, 3):
        values |= {1 << k, ((1 << k) - 1), ((1 << k) + 1) & ((1 << bits) - 1)}
    rnd = random.Random(width)
    values |= {rnd.getrandbits(bits) for _ in range(6)}
    return sorted(values)


class TestModel(unittest.TestCase):
    def test_matches_the_standard_by_index(self) -> None:
        for width in (8, 16):
            for value in range(1 << width) if width == 8 else _sweep(2):
                for op in bf.STDBIT_OPERATIONS:
                    with self.subTest(op=op, width=width, value=value):
                        self.assertEqual(
                            bf.stdbit(op, value, width),
                            _by_index(op, value, width),
                        )
        for width in (32, 64):
            for value in _sweep(width // 8):
                for op in bf.STDBIT_OPERATIONS:
                    with self.subTest(op=op, width=width, value=value):
                        self.assertEqual(
                            bf.stdbit(op, value, width),
                            _by_index(op, value, width),
                        )

    def test_edge_values(self) -> None:
        # The results for 0 and for the all-ones value, which every
        # operation defines (none of §7.18 is undefined).
        for width in (8, 16, 32, 64):
            ones = (1 << width) - 1
            want_zero = {
                "leading_zeros": width, "leading_ones": 0,
                "trailing_zeros": width, "trailing_ones": 0,
                "first_leading_zero": 1, "first_leading_one": 0,
                "first_trailing_zero": 1, "first_trailing_one": 0,
                "count_zeros": width, "count_ones": 0,
                "has_single_bit": 0, "bit_width": 0,
                "bit_floor": 0, "bit_ceil": 1,
            }
            want_ones = {
                "leading_zeros": 0, "leading_ones": width,
                "trailing_zeros": 0, "trailing_ones": width,
                "first_leading_zero": 0, "first_leading_one": 1,
                "first_trailing_zero": 0, "first_trailing_one": 1,
                "count_zeros": 0, "count_ones": width,
                "has_single_bit": 0, "bit_width": width,
                "bit_floor": 1 << (width - 1),
                # The power of 2 doesn't fit (§7.18.16p2).
                "bit_ceil": 0,
            }
            for op in bf.STDBIT_OPERATIONS:
                with self.subTest(op=op, width=width):
                    self.assertEqual(bf.stdbit(op, 0, width), want_zero[op])
                    self.assertEqual(bf.stdbit(op, ones, width), want_ones[op])

    def test_registry(self) -> None:
        self.assertEqual(len(bf.STDBIT_OPERATIONS), 14)
        self.assertEqual(len(bf.STDBIT_BUILTINS), 14 * 6)
        self.assertEqual(len(bf.STDBIT_FUNCTIONS), 14 * 5)
        for name in bf.STDBIT_BUILTINS:
            self.assertTrue(name.startswith("__c6502_stdc_"))
            self.assertEqual(bf.BUILTINS[name], 1)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestFolding(unittest.TestCase):
    def test_constant_arguments_fold_in_both_pipelines(self) -> None:
        for suffix, (tname, width) in _TYPES.items():
            value = 0x50 if width == 1 else 0x0F00 << (8 * (width - 2))
            for op in bf.STDBIT_OPERATIONS:
                rt = _result_type(op, tname)
                src = (
                    f"#include <stdbit.h>\n{rt} r;\n"
                    f"void f(void) {{ r = stdc_{op}(({tname}){value}); "
                    f"r = stdc_{op}_{suffix}({value}); }}\n"
                )
                want = bf.stdbit(op, value, 8 * width)
                for optimize in (False, True):
                    with self.subTest(op=op, type=tname, optimize=optimize):
                        tac = _tac(src, optimize)
                        fn = next(
                            t for t in tac.top_level
                            if isinstance(t, tac_ast.Function)
                        )
                        copies = [
                            i.src.const.value for i in fn.instructions
                            if isinstance(i, tac_ast.Copy)
                            and isinstance(i.src, tac_ast.Constant)
                        ]
                        self.assertFalse(any(
                            isinstance(i, tac_ast.Label)
                            for i in fn.instructions
                        ))
                        self.assertIn(want, copies)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestHeader(unittest.TestCase):
    _HEADER = Path(__file__).parent.parent / "include" / "stdbit.h"

    def test_macros(self) -> None:
        out = preprocess(
            "#include <stdbit.h>\n"
            "#if __STDC_VERSION_STDBIT_H__ == 202311L\nint version;\n#endif\n"
            "#if __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__\n"
            "int little;\n#endif\n"
            "#if __STDC_ENDIAN_LITTLE__ != __STDC_ENDIAN_BIG__\n"
            "int distinct;\n#endif\n"
        )
        for name in ("version", "little", "distinct"):
            self.assertIn(f"int {name};", out)

    def test_names_are_in_the_headers_namespace(self) -> None:
        # Every name the header declares or defines is one C23 §7.18
        # lists, `size_t` or an integer type from §7.18.1p1, a name in
        # the implementation's namespace, or a `stdc_` name (§7.33.12).
        text = self._HEADER.read_text(encoding="utf-8")
        text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
        names = set(re.findall(r"#define\s+(\w+)", text))
        names |= set(re.findall(r"typedef\s+[\w\s]+?\s(\w+);", text))
        names |= set(re.findall(r"(\w+)\(\w[\w\s]*\s(\w+)\);", text)[0])
        names |= {m for m in re.findall(r"\s(\w+)\(", text)}
        allowed = re.compile(
            r"_[A-Z_]\w*|__\w*|stdc_\w+|size_t|u?int(_least)?(8|16|32|64)_t"
        )
        bad = sorted(n for n in names if not allowed.fullmatch(n))
        self.assertEqual(bad, [])


    def test_every_function_is_declared_and_defined_as_a_macro(self) -> None:
        text = self._HEADER.read_text(encoding="utf-8")
        for op in bf.STDBIT_OPERATIONS:
            self.assertIn(f"#define stdc_{op}(x) __c6502_stdc_{op}(x)", text)
            for suffix, (tname, _) in _TYPES.items():
                rt = _result_type(op, tname)
                self.assertIn(
                    f"{rt} stdc_{op}_{suffix}({tname} __value);", text,
                )
                self.assertIn(
                    f"#define stdc_{op}_{suffix}(x) "
                    f"__c6502_stdc_{op}_{suffix}(x)", text,
                )

    def test_unused_declarations_leave_nothing_in_the_output(self) -> None:
        asm = _codegen(
            "#include <stdbit.h>\nunsigned char c; unsigned r;\n"
            "void f(void) { r = stdc_leading_zeros(c); }\n", optimize=True,
        )
        self.assertNotIn("stdc_", asm.replace(".stdbit_", ""))


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestFunctions(unittest.TestCase):
    """The functions with a suffix as functions (C23 §7.1.4)."""

    _SRC = """
#include <stdbit.h>
typedef unsigned (*count_fn)(unsigned char);
static count_fn counts[] = { &stdc_leading_zeros_uc, stdc_count_ones_uc };
unsigned char (*floor_fn)(unsigned char) = stdc_bit_floor_uc;
int main(void) {
    unsigned r = counts[0](0x10) * 100;       /* 3 */
    r += counts[1](0xF0) * 10;               /* 4 */
    r += (stdc_trailing_zeros_ul)(0x100);    /* 8 */
    r += floor_fn(100) == 64;                /* 1 */
#undef stdc_bit_width_ui
    r += stdc_bit_width_ui(0x1000) * 1000;   /* 13 */
    return r;
}
"""

    def test_runs_in_both_pipelines(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(self._SRC, optimize=optimize)
                result = sim.run(max_cycles=2_000_000)
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int(), 13349)


    def test_two_units_each_have_a_copy(self) -> None:
        import tempfile
        from binutils import elf as E
        from sim.linked import build_linked_sim
        from tests.link_helpers import compile_object, link
        a = (
            "#include <stdbit.h>\n"
            "unsigned (*pa)(unsigned char) = stdc_count_ones_uc;\n"
            "unsigned a(void) { return (stdc_count_ones_uc)(7); }\n"
        )
        b = (
            "#include <stdbit.h>\nunsigned a(void);\n"
            "unsigned (*pb)(unsigned char) = stdc_count_ones_uc;\n"
            "int main(void) { return a() * 10 + (stdc_count_ones_uc)(3); }\n"
        )
        with tempfile.TemporaryDirectory() as d:
            d = Path(d)
            for name, src in (("a", a), ("b", b)):
                self.assertEqual(compile_object(src, d / f"{name}.o"), 0)
            self.assertEqual(link([d / "a.o", d / "b.o"], d / "prog"), 0)
            exe = E.read_file(d / "prog")
            self.assertEqual(build_linked_sim(exe).run().return_int(), 32)
            copies = [
                s for s in exe.symbols if s.name.startswith("stdc_count_ones_uc")
            ]
            self.assertEqual(len(copies), 2)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestCodegen(unittest.TestCase):
    """The code a call compiles to, with -O, for operands in memory."""

    def _f(self, call: str, decls: str) -> list[str]:
        asm = _codegen(
            f"#include <stdbit.h>\n{decls}\nvoid f(void) {{ r = {call}; }}\n",
            optimize=True,
        )
        return _body(asm, "f")

    def test_leading_zeros_of_a_byte(self) -> None:
        # Shift a copy right until it's zero, counting down from 8.
        self.assertEqual(
            self._f("stdc_leading_zeros(c)", "unsigned char c; unsigned r;"),
            [
                "LDA c", "STA __local_f__1", "LDA #$08",
                "STA __local_f__0", "JMP .stdbit_test@2",
                ".stdbit_body@1:", "LSR __local_f__1", "DEC __local_f__0",
                ".stdbit_test@2:", "LDA __local_f__1",
                "BNE .stdbit_body@1", "LDA __local_f__0", "STA r",
                "LDA #$00", "STA r+1", "RTS",
            ],
        )

    def test_count_ones_of_an_int(self) -> None:
        # The low bit is tested with `LSR` of a copy in A, and then the
        # two bytes shift right in place.
        self.assertEqual(
            self._f("stdc_count_ones(u)", "unsigned u; unsigned r;"),
            [
                "LDA u", "STA __local_f__2", "LDA u+1", "STA __local_f__3",
                "LDA #$00", "STA __local_f__0", "JMP .stdbit_test@2",
                ".stdbit_body@1:", "LDA __local_f__2", "LSR",
                "BCC .stdbit_skip@3", "INC __local_f__0",
                ".stdbit_skip@3:", "LSR __local_f__3", "ROR __local_f__2",
                ".stdbit_test@2:", "LDA __local_f__2", "ORA __local_f__3",
                "BNE .stdbit_body@1", "LDA __local_f__0", "STA r",
                "LDA #$00", "STA r+1", "RTS",
            ],
        )

    def test_has_single_bit_of_a_byte(self) -> None:
        body = self._f("stdc_has_single_bit(c)", "unsigned char c; bool r;")
        self.assertIn("SBC #$01", body)
        self.assertIn("AND c", body)

    def test_no_helper_calls(self) -> None:
        # Every function is written in place; none needs a runtime
        # helper, which the repository doesn't have.
        for op in bf.STDBIT_OPERATIONS:
            for suffix, (tname, _) in _TYPES.items():
                with self.subTest(op=op, type=tname):
                    for optimize in (False, True):
                        asm = _codegen(
                            f"#include <stdbit.h>\n{tname} g; "
                            f"{_result_type(op, tname)} r;\n"
                            f"void f(void) {{ r = stdc_{op}(g); }}\n",
                            optimize=optimize,
                        )
                        self.assertNotIn("JSR", " ".join(_body(asm, "f")))


def _program(suffix: str | None, tname: str, values: list[int],
             operations) -> str:
    """A program that stores `stdc_<op>` (or its form with `suffix`) of
    each of `values` in an array `out_<op>`."""
    n = len(values)
    lines = ["#include <stdbit.h>"]
    lines.append(
        f"static const {tname} in[{n}] = "
        f"{{{', '.join(f'{v}ull' for v in values)}}};"
    )
    for op in operations:
        lines.append(f"{_result_type(op, tname)} out_{op}[{n}];")
    lines.append("int main(void) {")
    lines.append(f"    for (unsigned int i = 0; i < {n}; i++) {{")
    lines.append(f"        {tname} v = in[i];")
    for op in operations:
        fn = f"stdc_{op}_{suffix}" if suffix else f"stdc_{op}"
        lines.append(f"        out_{op}[i] = {fn}(v);")
    lines += ["    }", "    return 0;", "}"]
    return "\n".join(lines) + "\n"


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestSimulator(unittest.TestCase):
    """Every function on every value of an `unsigned char` and a sweep
    of each wider type, against `builtin_functions.stdbit`, in both
    pipelines. Each 8-byte program has half the functions, since the
    unoptimized pipeline's frame for all 14 would pass 253 bytes."""

    def _check_sweep(self, suffix: str) -> None:
        tname, width = _TYPES[suffix]
        values = list(range(256)) if width == 1 else _sweep(width)
        ops = bf.STDBIT_OPERATIONS
        groups = [ops[:7], ops[7:]] if width == 8 else [ops]
        for form in (None, suffix):
            for operations in groups:
                src = _program(form, tname, values, operations)
                for optimize in (False, True):
                    sim = build_sim(src, optimize=optimize)
                    result = sim.run(max_cycles=60_000_000)
                    self.assertFalse(result.timed_out)
                    mem = sim.mpu.memory
                    for op in operations:
                        size = {
                            "bit_floor": width, "bit_ceil": width,
                            "has_single_bit": 1,
                        }.get(op, 2)
                        base = sim.symbols[f"out_{op}"]
                        got = [
                            int.from_bytes(
                                bytes(mem[base + size * i:
                                          base + size * (i + 1)]),
                                "little",
                            )
                            for i in range(len(values))
                        ]
                        want = [bf.stdbit(op, v, 8 * width) for v in values]
                        with self.subTest(
                            op=op, form=form or "generic",
                            optimize=optimize,
                        ):
                            self.assertEqual(got, want)

    def test_unsigned_char(self) -> None:
        self._check_sweep("uc")

    def test_unsigned_short(self) -> None:
        self._check_sweep("us")

    def test_unsigned_int(self) -> None:
        self._check_sweep("ui")

    def test_unsigned_long(self) -> None:
        self._check_sweep("ul")

    def test_unsigned_long_long(self) -> None:
        self._check_sweep("ull")


if __name__ == "__main__":
    unittest.main()
