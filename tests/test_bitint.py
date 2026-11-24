"""Tests for C23's bit-precise integer types, `_BitInt(N)` and
`unsigned _BitInt(N)` (N3220 §6.2.5p5 and p8, §6.2.6.2, §6.3.1.1,
§6.3.1.3, §6.3.1.8, §6.4.4.2, §6.7.3.1, §5.2.5.3.2; bit_precise.py),
in the front end and the translation to TAC. tests/test_bitint_sim.py
runs every operator on every width in the simulator.

Coverage:

  * `_BitInt(N)`, `signed _BitInt(N)`, and `unsigned _BitInt(N)` parse
    with N any integer constant expression, and the errors for N
    below 2 (signed) or 1 (unsigned), above BITINT_MAXWIDTH, not
    constant, or not an integer, and for another type specifier with
    `_BitInt`.
  * `wb` and `uwb` constants, in every letter case and order, with
    digit separators and every base, take the least width that holds
    them (with the sign bit for `wb`), and `#if` reads them as
    `intmax_t` and `uintmax_t`.
  * The type rules: no integer promotion, the rank by width with a
    standard type above a bit-precise one of the same width, the usual
    arithmetic conversions for mixed operands, the results of unary
    operators, shifts, comparisons, and `?:`, and `sizeof`.
  * Constant expressions wrap an unsigned type at N bits, and static
    initializers convert to N bits.
  * BITINT_MAXWIDTH is 64 in <limits.h>.
  * The TAC restores the padding bits after `+` on a type with padding
    and leaves a type without padding alone, and the back end sees
    only whole bytes.
  * The type-generic <stdbit.h> functions take `unsigned _BitInt(N)` of
    8, 16, 32, and 64 bits, and the packed BCD builtins reject a
    bit-precise operand.
  * A program with switch, arrays, struct members, pointers, static
    initializers, and calls on bit-precise values runs to the same
    result in both pipelines.
"""
from __future__ import annotations

import shutil
import unittest

import c99_ast
from parser import ParserError, parse
from compile import _resolved
from passes.type_checking import TypeCheckError, check_program
from preprocessor import preprocess
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None


def _check(src: str):
    """The type checker's result for `src`, after every pass before it
    (`compile._resolved`)."""
    return check_program(_resolved(preprocess(src), optimize=False))


def _type_of(decls: str, exp: str):
    """The type the type checker gives `exp` in a function after
    `decls`."""
    prog, _, _ = _check(f"{decls}\nvoid probe(void) {{ {exp}; }}\n")
    fn = prog.declaration[-1].function_decl
    return fn.body.block_item[0].statement.exp.data_type


def _constant(text: str):
    prog = parse(f"int main(void) {{ return {text}; }}")
    ret = prog.declaration[0].function_decl.body.block_item[0].statement
    return ret.exp.const


def _static_value(src: str, name: str) -> int:
    _, symbols, _ = _check(src)
    return symbols[name].attrs.initial_value.value


def _bi(width: int, signed: bool = True) -> c99_ast.BitInt:
    return c99_ast.BitInt(width=width, is_signed=signed)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestSpecifier(unittest.TestCase):
    def test_forms(self) -> None:
        cases = {
            "_BitInt(5) x;": _bi(5),
            "signed _BitInt(7) x;": _bi(7),
            "unsigned _BitInt(1) x;": _bi(1, False),
            "_BitInt(64) x;": _bi(64),
            "unsigned _BitInt(64) x;": _bi(64, False),
            "_BitInt(2 * 3 + 1) x;": _bi(7),
            "_BitInt(sizeof(long)) x;": _bi(4),
            "enum { W = 12 }; _BitInt(W) x;": _bi(12),
            "typedef unsigned _BitInt(24) u24; u24 x;": _bi(24, False),
        }
        for src, want in cases.items():
            with self.subTest(src=src):
                prog = parse(src)
                self.assertEqual(prog.declaration[-1].var_decl.data_type, want)

    def test_qualified_and_derived(self) -> None:
        prog = parse("const _BitInt(9) *p; _BitInt(3) a[4];")
        self.assertEqual(
            prog.declaration[0].var_decl.data_type,
            c99_ast.Pointer(referenced_type=c99_ast.Const(
                referenced_type=_bi(9))),
        )
        self.assertEqual(
            prog.declaration[1].var_decl.data_type,
            c99_ast.Array(element_type=_bi(3), size=4),
        )

    def test_width_errors(self) -> None:
        cases = {
            "_BitInt(1) x;": "must be at least 2, and it's 1",
            "signed _BitInt(0) x;": "must be at least 2",
            "unsigned _BitInt(0) x;": "must be at least 1, and it's 0",
            "_BitInt(65) x;": "at most BITINT_MAXWIDTH, 64, and it's 65",
            "unsigned _BitInt(100) x;": "at most BITINT_MAXWIDTH",
            "int n; _BitInt(n) x;": "must be an integer constant expression",
            "_BitInt(2.0) x;": "must be an integer constant expression",
        }
        for src, message in cases.items():
            with self.subTest(src=src):
                with self.assertRaises(ParserError) as cm:
                    parse(src)
                self.assertIn(message, str(cm.exception))

    def test_other_specifiers_are_errors(self) -> None:
        for src in ["long _BitInt(8) x;", "_BitInt(8) int x;",
                    "unsigned signed _BitInt(8) x;",
                    "_BitInt(8) _BitInt(8) x;", "char _BitInt(4) x;"]:
            with self.subTest(src=src):
                with self.assertRaises(ParserError) as cm:
                    parse(src)
                self.assertIn("combines only with one 'signed'",
                              str(cm.exception))

    def test_cast_and_sizeof(self) -> None:
        for n in [1, 7, 8, 9, 16, 17, 24, 25, 40, 41, 48, 56, 57, 64]:
            with self.subTest(n=n):
                self.assertEqual(
                    _static_value(
                        f"unsigned long s = sizeof(unsigned _BitInt({n}));",
                        "s",
                    ),
                    (n + 7) // 8,
                )
        self.assertEqual(
            _static_value("int s = sizeof((_BitInt(20))1);", "s"), 3,
        )

    def test_bitint_is_a_keyword(self) -> None:
        with self.assertRaises(Exception):
            parse("int _BitInt = 1;")


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestConstants(unittest.TestCase):
    def test_least_width(self) -> None:
        cases = {
            "0wb": (0, 2, True), "1wb": (1, 2, True), "2wb": (2, 3, True),
            "3wb": (3, 3, True), "4wb": (4, 4, True),
            "0uwb": (0, 1, False), "1uwb": (1, 1, False),
            "3uwb": (3, 2, False), "4uwb": (4, 3, False),
            "0xFFwb": (255, 9, True), "0xFFuwb": (255, 8, False),
            "0b101wb": (5, 4, True), "017wb": (15, 5, True),
            "1'000'000wb": (1000000, 21, True),
            "0x7FFF'FFFF'FFFF'FFFFwb": ((1 << 63) - 1, 64, True),
            "18446744073709551615uwb": ((1 << 64) - 1, 64, False),
        }
        for text, (value, width, signed) in cases.items():
            with self.subTest(text=text):
                self.assertEqual(
                    _constant(text),
                    c99_ast.ConstBitInt(value=value, width=width,
                                        is_signed=signed),
                )

    def test_letter_case_and_order(self) -> None:
        for text in ["5WB", "5wb"]:
            with self.subTest(text=text):
                self.assertTrue(_constant(text).is_signed)
        for text in ["5uwb", "5Uwb", "5uWB", "5UWB", "5wbu", "5wbU",
                     "5WBu", "5WBU"]:
            with self.subTest(text=text):
                self.assertEqual(
                    _constant(text),
                    c99_ast.ConstBitInt(value=5, width=3, is_signed=False),
                )
        for text in ["5wB", "5Wb", "5uwbu"]:
            with self.subTest(text=text):
                with self.assertRaises(Exception):
                    parse(f"int x = {text};")

    def test_negated_constant(self) -> None:
        # `-3wb` is a `_BitInt(3)` negated, and `-3uwb` an
        # `unsigned _BitInt(2)` that wraps (C23 §6.4.4.2p8).
        self.assertEqual(_type_of("", "-3wb"), _bi(3))
        self.assertEqual(_static_value("int x = -3wb;", "x"), -3)
        self.assertEqual(_type_of("", "-3uwb"), _bi(2, False))
        self.assertEqual(_static_value("int x = -3uwb;", "x"), 1)

    def test_preprocessor_if(self) -> None:
        src = (
            "#if 1wb && -1wb < 0 && -1uwb > 0 && 0xFFuwb == 255\n"
            "int yes;\n#else\nint no;\n#endif\n"
        )
        self.assertIn("int yes;", preprocess(src))

    def test_preprocessor_if_too_wide(self) -> None:
        from preprocessor import PreprocessorError
        with self.assertRaises(PreprocessorError) as cm:
            preprocess("#if 18446744073709551616uwb\n#endif\n")
        self.assertIn("needs a unsigned _BitInt(65)", str(cm.exception))


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestTypeRules(unittest.TestCase):
    def test_no_integer_promotion(self) -> None:
        decls = "_BitInt(5) a, b; unsigned _BitInt(3) u;"
        self.assertEqual(_type_of(decls, "a + b"), _bi(5))
        self.assertEqual(_type_of(decls, "-a"), _bi(5))
        self.assertEqual(_type_of(decls, "~u"), _bi(3, False))
        self.assertEqual(_type_of(decls, "+u"), _bi(3, False))
        self.assertEqual(_type_of(decls, "a << 2"), _bi(5))
        self.assertEqual(_type_of(decls, "2 << a"), c99_ast.Int())
        self.assertEqual(_type_of(decls, "a < b"), c99_ast.Int())
        self.assertEqual(_type_of(decls, "!a"), c99_ast.Int())
        self.assertEqual(_type_of(decls, "a++"), _bi(5))

    def test_standard_example(self) -> None:
        # C23 §6.3.1.8p3.
        decls = "_BitInt(2) a2; _BitInt(3) a3; _BitInt(33) a33; signed char c;"
        self.assertEqual(_type_of(decls, "a2 * a3"), _bi(3))
        self.assertEqual(_type_of(decls, "a2 * c"), c99_ast.Int())
        self.assertEqual(_type_of(decls, "a33 * c"), _bi(33))

    def test_usual_arithmetic_conversions(self) -> None:
        I, U = c99_ast.Int(), c99_ast.UInt()
        cases = [
            ("_BitInt(16)", "int", I),
            ("_BitInt(17)", "int", _bi(17)),
            ("_BitInt(24)", "unsigned int", _bi(24)),
            ("unsigned _BitInt(16)", "int", U),
            ("unsigned _BitInt(15)", "int", I),
            ("_BitInt(16)", "unsigned int", U),
            ("unsigned _BitInt(7)", "_BitInt(7)", _bi(7, False)),
            ("unsigned _BitInt(5)", "_BitInt(7)", _bi(7)),
            ("unsigned _BitInt(8)", "_BitInt(8)", _bi(8, False)),
            ("unsigned _BitInt(7)", "_BitInt(8)", _bi(8)),
            ("_BitInt(32)", "unsigned long", c99_ast.ULong()),
            ("_BitInt(40)", "unsigned long", _bi(40)),
            ("_BitInt(33)", "unsigned long", _bi(33)),
            ("_BitInt(32)", "unsigned _BitInt(31)", _bi(32)),
            ("long long", "unsigned _BitInt(63)", c99_ast.LongLong()),
            ("long long", "unsigned _BitInt(64)", c99_ast.ULongLong()),
            ("_BitInt(40)", "long long", c99_ast.LongLong()),
            ("unsigned _BitInt(64)", "long long", c99_ast.ULongLong()),
            ("_BitInt(64)", "unsigned long long", c99_ast.ULongLong()),
            ("_BitInt(12)", "double", c99_ast.Double()),
            ("unsigned _BitInt(3)", "float", c99_ast.Float()),
            ("_BitInt(5)", "char", I),
            ("_BitInt(5)", "bool", I),
        ]
        for left, right, want in cases:
            with self.subTest(left=left, right=right):
                decls = f"{left} l; {right} r;"
                self.assertEqual(_type_of(decls, "l + r"), want)
                self.assertEqual(_type_of(decls, "r * l"), want)
                self.assertEqual(_type_of(decls, "1 ? l : r"), want)

    def test_conversions_are_casts(self) -> None:
        prog, _, _ = _check(
            "_BitInt(5) f(long x) { return x; }\n"
            "double g(_BitInt(5) a) { return a; }\n"
        )
        ret = prog.declaration[0].function_decl.body.block_item[0].statement
        self.assertEqual(ret.exp.data_type, _bi(5))
        self.assertIsInstance(ret.exp, c99_ast.Cast)

    def test_pointer_compatibility(self) -> None:
        # Each N is a distinct type (C23 §6.2.5p5 footnote 30).
        with self.assertRaises(TypeCheckError):
            _check("_BitInt(3) x; _BitInt(4) *p = &x;")
        _check("_BitInt(3) x; _BitInt(3) *p = &x; void *v = &x;")

    def test_switch_and_subscript(self) -> None:
        _check(
            "int t[4];\n"
            "int f(unsigned _BitInt(2) k) {\n"
            "  switch (k) { case 3uwb: return t[k]; case 5: return 1; }\n"
            "  return 0;\n}\n"
        )
        with self.assertRaises(TypeCheckError) as cm:
            _check(
                "int f(unsigned _BitInt(2) k) {\n"
                "  switch (k) { case 1: case 5: return 1; }\n"
                "  return 0;\n}\n"
            )
        self.assertIn("duplicate case value 1", str(cm.exception))


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestConstantExpressions(unittest.TestCase):
    def test_unsigned_wraps_at_n_bits(self) -> None:
        cases = {
            "(unsigned _BitInt(3))7 + 1uwb": 0,
            "(unsigned _BitInt(3))5 * 3uwb": 7,
            "(unsigned _BitInt(4))-1": 15,
            "(unsigned _BitInt(9))1 << 8": 256,
            "(_BitInt(5))-1 >> 1": -1,
            "~(unsigned _BitInt(6))0": 63,
            "(_BitInt(5))100": 4,
            "(_BitInt(5))-17": 15,
            "(_BitInt(12))0xFFF": -1,
            "(unsigned _BitInt(1))2": 0,
            "(int)(_BitInt(5))100": 4,
        }
        for exp, want in cases.items():
            with self.subTest(exp=exp):
                self.assertEqual(_static_value(f"long x = {exp};", "x"), want)

    def test_array_size_and_case(self) -> None:
        _, symbols, _ = _check(
            "char a[(unsigned _BitInt(3))5 + 4uwb];\n"
            "char b[sizeof(_BitInt(33))];\n"
        )
        self.assertEqual(symbols["a"].type.size, 1)
        self.assertEqual(symbols["b"].type.size, 5)

    def test_signed_overflow_isnt_constant(self) -> None:
        # Overflow of a signed type is undefined (C23 §6.5p5), so it
        # isn't a constant expression, as for the standard types.
        with self.assertRaises(NotImplementedError) as cm:
            _check("char a[(_BitInt(4))7 + 2wb];")
        self.assertIn("signed overflow", str(cm.exception))

    def test_static_initializers_convert(self) -> None:
        cases = {
            "_BitInt(5) x = 100;": 4,
            "unsigned _BitInt(3) x = -1;": 7,
            "_BitInt(40) x = 1.75;": 1,
            "unsigned _BitInt(20) x = 0x123456;": 0x23456,
            "_BitInt(2) x = 3wb;": -1,
        }
        for src, want in cases.items():
            with self.subTest(src=src):
                self.assertEqual(_static_value(src, "x"), want)

    def test_limits_h(self) -> None:
        out = preprocess(
            "#include <limits.h>\n"
            "#if BITINT_MAXWIDTH == 64\nint yes;\n#endif\n"
            "_BitInt(BITINT_MAXWIDTH) w;\n"
        )
        self.assertIn("int yes;", out)
        prog = parse(out)
        self.assertEqual(prog.declaration[-1].var_decl.data_type, _bi(64))


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestBuiltins(unittest.TestCase):
    def test_stdbit_generic(self) -> None:
        self.assertEqual(
            _type_of("#include <stdbit.h>\nunsigned _BitInt(16) x;",
                     "stdc_bit_floor(x)"),
            _bi(16, False),
        )
        self.assertEqual(
            _type_of("#include <stdbit.h>\nunsigned _BitInt(64) x;",
                     "stdc_count_ones(x)"),
            c99_ast.UInt(),
        )
        for t in ["unsigned _BitInt(12)", "_BitInt(16)"]:
            with self.subTest(t=t):
                with self.assertRaises(TypeCheckError) as cm:
                    _check(f"#include <stdbit.h>\n{t} x;\n"
                           f"int f(void) {{ return stdc_count_ones(x); }}")
                self.assertIn("unsigned _BitInt of 8, 16, 32, and 64 bits",
                              str(cm.exception))

    def test_bcd_rejects_bit_precise(self) -> None:
        with self.assertRaises(TypeCheckError) as cm:
            _check("unsigned _BitInt(8) x;\n"
                   "int f(void) { return __c6502_bcd_add(x, 1u); }")
        self.assertIn("bit-precise type", str(cm.exception))


_PROGRAM = r"""
#include <stdbit.h>
typedef unsigned _BitInt(24) u24;
typedef _BitInt(12) s12;

struct rec { unsigned char tag; s12 x; u24 big; _BitInt(5) small; };
struct rec recs[3] = {{1, -5, 0xABCDEF, -16}, {2, 2047, 7, 15}};
static const _BitInt(40) table[4] = {1, -2, 0x7FFFFFFFFFwb, -0x8000000000wb};
u24 counter = 0xFFFFFE;
_BitInt(5) small = 100;
long results[16];

u24 add24(u24 a, u24 b) { return a + b; }
s12 twice(s12 *p) { *p *= 2; return *p; }
_BitInt(40) pick(_BitInt(3) i) { return table[i & 3]; }

int classify(_BitInt(5) v) {
    switch (v) {
    case -16: return 1;
    case 15: return 2;
    case 0: return 3;
    default: return 4;
    }
}

void more(long *r);

int main(void) {
    long *r = results;
    s12 x = recs[0].x;
    *r++ = add24(counter, 3);
    counter++;
    *r++ = counter;
    *r++ = twice(&x);
    *r++ = twice(&recs[1].x);
    *r++ = pick(2);
    *r++ = (long)(pick(3) >> 8);
    *r++ = classify(recs[0].small) * 10 + classify(recs[1].small);
    *r++ = classify(small) * 10 + classify(small - 4);
    more(r);
    return 0;
}

void more(long *r) {
    *r++ = recs[0].big >> 4;
    *r++ = (unsigned _BitInt(4))(recs[0].tag + 20);
    *r++ = stdc_bit_width((unsigned _BitInt(16))1000);
    *r++ = sizeof(struct rec);
    *r++ = (u24)-1 > 0;
    *r++ = (s12)-1 < 0;
    *r++ = (_BitInt(20))(double)-123456.75;
    *r++ = (long)(float)(u24)0x123456;
}
"""

_EXPECTED = [
    1, 0xFFFFFF, -10, -2, 0x7FFFFFFFFF & 0xFFFFFFFF,
    (-0x8000000000 >> 8) & 0xFFFFFFFF, 12, 43, 0xABCDE, 5, 10, 7, 1, 1,
    -123456, 0x123456,
]


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestProgram(unittest.TestCase):
    def test_program(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(_PROGRAM, optimize=optimize)
                sim.run()
                addr = sim.symbols["results"]
                mem = sim.mpu.memory
                got = [
                    int.from_bytes(bytes(mem[addr + 4 * k:addr + 4 * k + 4]),
                                   "little", signed=True)
                    for k in range(len(_EXPECTED))
                ]
                want = [v - (1 << 32) if v >= 1 << 31 else v
                        for v in (e & 0xFFFFFFFF for e in _EXPECTED)]
                self.assertEqual(got, want)


if __name__ == "__main__":
    unittest.main()
