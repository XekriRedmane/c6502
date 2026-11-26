"""Tests for the runtime library (cpp/src/passes/runtime_library.h), the
6502 helpers the linker adds to a program that calls them.

Each helper is reached from C the way the compiler reaches it (`*`,
`/`, `%`, `<<`, and `>>` on the types that call it), linked with no
`--defsym`, and run on the simulator with its own helpers removed, so
that only the copies the link put in the program can run. The results
are checked against Python on edge cases and random values. The other
tests check that a program gets only the helpers it calls, that
`--defsym` takes precedence over the library, that a linker script
places the helpers, that the helpers use a HARGS block the program
moved, and that a helper with no 6502 code still names itself in the
link error. The tests of `closure` and of the helpers' sizes are
doctest cases of the C++ port, in cpp/tests/test_runtime_layout.cpp
(docs/cpp_port.md, step 8, group G6).
"""
from __future__ import annotations

import io
import random
import tempfile
import unittest
from dataclasses import dataclass
from pathlib import Path
from unittest.mock import patch

from binutils import elf as E
from rtlib.helpers import all_helper_functions
from tests.cpp_build import needs_c6502cpp

# The name the runtime library's object goes by in a link.
LIBRARY_NAME = "runtime.o"


def helper_names() -> frozenset[str]:
    """The names of the helpers the runtime library has, the functions
    of rtlib/helpers.py, which the simulator keeps."""
    return frozenset(fn.name for fn in all_helper_functions())



@dataclass
class _Type:
    """A C integer type: its spelling, its width in bytes, and whether
    it's signed."""
    c: str
    size: int
    signed: bool

    @property
    def bits(self) -> int:
        return 8 * self.size

    def wrap(self, v: int) -> int:
        """`v` converted to this type."""
        v &= (1 << self.bits) - 1
        if self.signed and v >> (self.bits - 1):
            v -= 1 << self.bits
        return v

    def literal(self, v: int) -> str:
        """`v` as a C expression of a value of this type."""
        if v == -(1 << (self.bits - 1)):
            return f"({v + 1} - 1)"
        suffix = "UL" if not self.signed and self.size == 4 else ""
        return f"({v}{suffix})" if v < 0 else f"{v}{suffix}"

    def values(self, rng: random.Random, n: int) -> list[int]:
        """Edge cases (0, 1, the largest and smallest values, and the
        values around the sign bit and each byte boundary), then
        random values, `n` in all."""
        top = (1 << self.bits) - 1
        raw = [0, 1, 2, 3, top, top - 1, 1 << (self.bits - 1),
               (1 << (self.bits - 1)) - 1, (1 << (self.bits - 1)) + 1,
               0x55, 0xAA, 0x7F, 0x80, 0xFF]
        for k in range(1, self.size):
            raw += [1 << (8 * k), (1 << (8 * k)) - 1]
        out = list(dict.fromkeys(self.wrap(v) for v in raw))
        while len(out) < n:
            out.append(self.wrap(rng.getrandbits(self.bits)))
        return out[:n]


U8 = _Type("uint8_t", 1, False)
COUNT = U8
B8 = _Type("_BitInt(8)", 1, True)
UB8 = _Type("unsigned _BitInt(8)", 1, False)
U16 = _Type("uint16_t", 2, False)
S16 = _Type("int16_t", 2, True)
U32 = _Type("uint32_t", 4, False)
S32 = _Type("int32_t", 4, True)


def _trunc_div(a: int, b: int) -> int:
    q = abs(a) // abs(b)
    return -q if (a < 0) != (b < 0) else q


def _source(t: _Type, u: _Type, xs: list[int], ys: list[int],
            outs: list[str]) -> str:
    """A program that applies each expression of `outs` (in `x` and
    `y`) to every pair of `xs` (of type `t`) and `ys` (of type `u`),
    storing the results in `out0`, `out1`, and so on, of type `t`."""
    n = len(xs)
    lines = [
        "#include <stdint.h>",
        f"{t.c} xs[{n}] = {{{', '.join(t.literal(v) for v in xs)}}};",
        f"{u.c} ys[{n}] = {{{', '.join(u.literal(v) for v in ys)}}};",
    ]
    lines += [f"{t.c} out{k}[{n}];" for k in range(len(outs))]
    body = "".join(
        f"out{k}[i] = ({t.c})({e});" for k, e in enumerate(outs)
    )
    lines += [
        "int main(void) {",
        f"    for (uint8_t i = 0; i < {n}; i++) {{",
        f"        {t.c} x = xs[i]; {u.c} y = ys[i];",
        f"        {body}",
        "    }",
        "    return 0;",
        "}",
    ]
    return "\n".join(lines) + "\n"


def _compile(source: str, path: Path) -> None:
    from tests.link_helpers import compile_object
    assert compile_object(source, path) == 0


def _link(paths, out: Path, *extra: str) -> tuple[int, str]:
    """Links `paths` into `out` with no `--defsym` but those in `extra`.
    Returns the exit code and what the link printed to stderr."""
    from compile import main as compile_main
    argv = ["compile.py", *map(str, paths), "-o", str(out), *extra]
    with patch("sys.stderr", io.StringIO()) as err:
        rc = compile_main(argv)
        return rc, err.getvalue()


def _build(source: str, *extra: str, script: str | None = None):
    """The executable that `source` compiles and links to, as an
    `ElfFile`, linked by `script` when one is given."""
    with tempfile.TemporaryDirectory() as tmp:
        obj = Path(tmp) / "a.o"
        _compile(source, obj)
        args = list(extra)
        if script is not None:
            ld = Path(tmp) / "prog.ld"
            ld.write_text(script)
            args = ["-T", str(ld), *args]
        out = Path(tmp) / "prog"
        rc, err = _link([obj], out, *args)
        if rc != 0:
            raise AssertionError(err)
        return E.read_file(out)


def _own_helpers(exe: E.ElfFile) -> dict[str, int]:
    """The helpers the executable holds in its own sections, by
    address."""
    return {
        s.name: s.value for s in exe.symbols
        if s.name in helper_names() and s.defined
        and s.section != E.SHN_ABS
    }


def _run(exe: E.ElfFile) -> tuple[bytearray, int, dict[str, int]]:
    """Runs `exe` on the simulator with the simulator's own helpers
    removed: no Python hook and no copy of the 6502 helpers at $E000.
    Returns the memory at the end and `main`'s result, as a signed
    `int`, with the executable's symbols."""
    from sim.linked import build_linked_sim
    sim = build_linked_sim(exe)
    rt = sim.runtime
    sim.mpu.memory[rt.helper_origin:rt.helper_origin
                   + len(rt.helper_image)] = bytes(len(rt.helper_image))
    rt.hooks.clear()
    res = sim.run()
    assert not res.timed_out
    return res.memory, res.return_int_signed(), sim.symbols


def _read(run, name: str, t: _Type, n: int) -> list[int]:
    mem, _, symbols = run
    base = symbols[name]
    return [
        t.wrap(int.from_bytes(bytes(mem[base + k * t.size:
                                        base + (k + 1) * t.size]),
                              "little"))
        for k in range(n)
    ]


@needs_c6502cpp
class TestHelpers(unittest.TestCase):
    """Each helper, linked from the library and run, against Python.
    A 1- or 2-byte shift by a variable count is an inline loop, so
    those tests check the loop's results and that the shift helpers
    of those widths aren't linked."""

    N = 24

    def _check(self, helpers: set[str], t: _Type, u: _Type,
               outs: list[tuple[str, object]], nonzero: bool = False,
               counts: range | None = None,
               absent: frozenset[str] = frozenset()) -> None:
        rng = random.Random(f"{sorted(helpers)}{t.c}")
        xs = t.values(rng, self.N)
        if counts is not None:
            ys = [counts[k % len(counts)] for k in range(self.N)]
        else:
            ys = u.values(rng, self.N + 4)[::-1]
            if nonzero:
                ys = [y for y in ys if y != 0]
            ys = ys[:self.N]
            if t.signed and nonzero:
                # The quotient of the smallest value and -1 doesn't
                # fit the type, which C leaves undefined.
                low = -(1 << (t.bits - 1))
                ys = [2 if (x, y) == (low, -1) else y
                      for x, y in zip(xs, ys)]
        exe = _build(_source(t, u, xs, ys, [e for e, _ in outs]))
        self.assertTrue(helpers <= set(_own_helpers(exe)),
                        f"{helpers} not all in {sorted(_own_helpers(exe))}")
        self.assertFalse(absent & set(_own_helpers(exe)),
                         f"{absent} linked: {sorted(_own_helpers(exe))}")
        self.assertFalse(any(
            s.name in helper_names() and s.section == E.SHN_ABS
            for s in exe.symbols
        ))
        run = _run(exe)
        for k, (expr, fn) in enumerate(outs):
            got = _read(run, f"out{k}", t, self.N)
            want = [t.wrap(fn(x, y)) for x, y in zip(xs, ys)]
            self.assertEqual(
                got, want,
                f"{expr} on {t.c}: first difference at "
                f"x={xs[[g == w for g, w in zip(got, want)].index(False)]}"
                if got != want else "",
            )

    def test_mul8(self) -> None:
        self._check({"mul8"}, U8, U8, [("x * y", lambda x, y: x * y)])

    def test_udivmod8(self) -> None:
        self._check({"udivmod8"}, UB8, UB8, [
            ("x / y", lambda x, y: x // y),
            ("x % y", lambda x, y: x % y),
        ], nonzero=True)

    def test_sdivmod8(self) -> None:
        self._check({"sdivmod8", "udivmod8"}, B8, B8, [
            ("x / y", _trunc_div),
            ("x % y", lambda x, y: x - _trunc_div(x, y) * y),
        ], nonzero=True)

    def test_asl8(self) -> None:
        self._check(set(), U8, COUNT, [("x << y", lambda x, y: x << y)],
                    counts=range(8),
                    absent=frozenset({"asl8"}))

    def test_lsr8(self) -> None:
        self._check(set(), UB8, COUNT, [("x >> y", lambda x, y: x >> y)],
                    counts=range(8),
                    absent=frozenset({"lsr8"}))

    def test_asr8(self) -> None:
        self._check(set(), B8, COUNT, [("x >> y", lambda x, y: x >> y)],
                    counts=range(8),
                    absent=frozenset({"asr8"}))

    def test_mul16(self) -> None:
        self._check({"mul16"}, U16, U16, [("x * y", lambda x, y: x * y)])

    def test_udivmod16(self) -> None:
        self._check({"udivmod16"}, U16, U16, [
            ("x / y", lambda x, y: x // y),
            ("x % y", lambda x, y: x % y),
        ], nonzero=True)

    def test_sdivmod16(self) -> None:
        self._check({"sdivmod16", "udivmod16"}, S16, S16, [
            ("x / y", _trunc_div),
            ("x % y", lambda x, y: x - _trunc_div(x, y) * y),
        ], nonzero=True)

    def test_asl16(self) -> None:
        self._check(set(), U16, COUNT, [("x << y", lambda x, y: x << y)],
                    counts=range(16),
                    absent=frozenset({"asl16"}))

    def test_lsr16(self) -> None:
        self._check(set(), U16, COUNT, [("x >> y", lambda x, y: x >> y)],
                    counts=range(16),
                    absent=frozenset({"lsr16"}))

    def test_asr16(self) -> None:
        self._check(set(), S16, COUNT, [("x >> y", lambda x, y: x >> y)],
                    counts=range(16),
                    absent=frozenset({"asr16"}))

    def test_mul32(self) -> None:
        self._check({"mul32"}, U32, U32, [("x * y", lambda x, y: x * y)])

    def test_udivmod32(self) -> None:
        self._check({"udivmod32"}, U32, U32, [
            ("x / y", lambda x, y: x // y),
            ("x % y", lambda x, y: x % y),
        ], nonzero=True)

    def test_sdivmod32(self) -> None:
        self._check({"sdivmod32", "udivmod32"}, S32, S32, [
            ("x / y", _trunc_div),
            ("x % y", lambda x, y: x - _trunc_div(x, y) * y),
        ], nonzero=True)

    def test_asl32(self) -> None:
        self._check({"asl32"}, U32, COUNT, [("x << y", lambda x, y: x << y)],
                    counts=range(32))

    def test_lsr32(self) -> None:
        self._check({"lsr32"}, U32, COUNT, [("x >> y", lambda x, y: x >> y)],
                    counts=range(32))

    def test_asr32(self) -> None:
        self._check({"asr32"}, S32, COUNT, [("x >> y", lambda x, y: x >> y)],
                    counts=range(32))

    def test_bin16_to_bcd(self) -> None:
        # `__c6502_bin_to_bcd` of a variable calls the helper, and a
        # constant argument folds, so a program with only constants
        # doesn't hold it.
        from builtin_functions import bin_to_bcd
        rng = random.Random("bin16_to_bcd")
        xs = U16.values(rng, self.N)
        src = (
            "#include <stdint.h>\n"
            f"uint16_t xs[{self.N}] = {{{', '.join(map(str, xs))}}};\n"
            f"uint32_t out0[{self.N}];\n"
            "int main(void) {\n"
            f"    for (uint8_t i = 0; i < {self.N}; i++)"
            " out0[i] = __c6502_bin_to_bcd(xs[i]);\n"
            "    return 0;\n"
            "}\n"
        )
        exe = _build(src)
        self.assertEqual(set(_own_helpers(exe)), {"bin16_to_bcd"})
        got = _read(_run(exe), "out0", U32, self.N)
        self.assertEqual(got, [bin_to_bcd(x) for x in xs])
        exe = _build(
            "#include <stdint.h>\nuint32_t v;\n"
            "int main(void) { v = __c6502_bin_to_bcd(12345); return 0; }\n"
        )
        self.assertEqual(_own_helpers(exe), {})

    def test_every_helper_is_tested(self) -> None:
        tested = {name[len("test_"):] for name in dir(self)
                  if name.startswith("test_")}
        self.assertEqual(helper_names() - tested, set())


_DIVIDE = """
int a = -1234;
int b = 7;
int main(void) { return a / b; }
"""


@needs_c6502cpp
class TestSelection(unittest.TestCase):
    """Which helpers a link takes from the library."""

    def test_no_helper_called_none_linked(self) -> None:
        exe = _build("int a = 3; int main(void) { return a + 1; }")
        self.assertEqual(_own_helpers(exe), {})
        self.assertNotIn(LIBRARY_NAME, [
            s.name for s in exe.symbols if s.type == E.STT_FILE
        ])

    def test_only_called_helpers_and_their_callees(self) -> None:
        exe = _build(_DIVIDE)
        self.assertEqual(set(_own_helpers(exe)), {"sdivmod16", "udivmod16"})
        self.assertEqual(_run(exe)[1], -176)

    def test_defsym_takes_precedence(self) -> None:
        from sim.linked import build_linked_sim, defsym_args
        exe = _build(_DIVIDE, *defsym_args())
        self.assertEqual(_own_helpers(exe), {})
        self.assertEqual(build_linked_sim(exe).run().return_int_signed(), -176)

    def test_defsym_of_a_callee(self) -> None:
        # sdivmod16 comes from the library, and its call of udivmod16
        # goes to the address --defsym gives, the simulator's copy.
        from sim.linked import build_linked_sim, runtime_defsyms
        addr = runtime_defsyms()["udivmod16"]
        exe = _build(_DIVIDE, f"--defsym=udivmod16=0x{addr:04X}")
        self.assertEqual(set(_own_helpers(exe)), {"sdivmod16"})
        self.assertEqual(exe.symbol("udivmod16").value, addr)
        self.assertEqual(build_linked_sim(exe).run().return_int_signed(), -176)

    def test_a_definition_takes_precedence(self) -> None:
        # A unit that defines a helper's name gives the program that
        # function, as a C toolchain prefers an object's definition to
        # libgcc's, and the library adds nothing.
        with tempfile.TemporaryDirectory() as tmp:
            a, b = Path(tmp) / "a.o", Path(tmp) / "b.o"
            _compile(_DIVIDE.replace("int main", "int quotient"), a)
            _compile(
                "void sdivmod16(void) {}\n"
                "void udivmod16(void) {}\n"
                "int quotient(void);\n"
                "int main(void) { sdivmod16(); udivmod16();"
                " return quotient(); }\n",
                b,
            )
            out = Path(tmp) / "prog"
            rc, err = _link([a, b], out)
            self.assertEqual(rc, 0, err)
            exe = E.read_file(out)
        files = [s.name for s in exe.symbols if s.type == E.STT_FILE]
        self.assertNotIn(LIBRARY_NAME, files)
        self.assertEqual(set(_own_helpers(exe)), {"sdivmod16", "udivmod16"})

    def test_helper_without_code_is_an_error(self) -> None:
        src = "float a = 1.5f; float b = 2.0f; " \
              "int main(void) { return (int)(a + b); }"
        with tempfile.TemporaryDirectory() as tmp:
            obj = Path(tmp) / "a.o"
            _compile(src, obj)
            rc, err = _link([obj], Path(tmp) / "prog")
        self.assertEqual(rc, 1)
        self.assertIn("undefined symbol `fadd`", err)
        self.assertIn("no 6502 code in the runtime library", err)
        self.assertNotIn("isn't in the repository", err)

    def test_moved_hargs(self) -> None:
        # A fixed object in the classic block's bytes moves HARGS, and
        # the helpers read and write the program's HARGS.
        src = (
            "#include <stdint.h>\n"
            "extern uint8_t wndtop [[c6502::address(0x22)]];\n"
            "int a = -1234; int b = 7;\n"
            "int main(void) { wndtop = 3; return a / b; }\n"
        )
        exe = _build(src)
        self.assertNotEqual(exe.symbol("HARGS").value, 0x04)
        self.assertEqual(_run(exe)[1], -176)
        meta = exe.link_text()
        from passes.runtime_layout import helper_footprints
        size = int(meta.split("hargs_size=")[1].split()[0])
        self.assertGreaterEqual(size, helper_footprints()["sdivmod16"])


_SCRIPT = """
MEMORY
{
    ram (rwx) : ORIGIN = 0x0800, LENGTH = 0x1000
    lib (rx) : ORIGIN = 0x4000, LENGTH = 0x0400
}
SECTIONS
{
    .text : { a.o(.text .text.*) } > ram
    .rodata : { *(.rodata .rodata.*) } > ram
    .data : { *(.data .data.*) } > ram
    .bss : { *(.bss .bss.*) } > ram
    .runtime : { runtime.o(.text) } > lib
}
"""


@needs_c6502cpp
class TestLinkerScript(unittest.TestCase):
    """A linker script places the helpers by the library's name, or
    with a pattern that catches every unit's code."""

    def test_by_name(self) -> None:
        exe = _build(_DIVIDE, script=_SCRIPT)
        helpers = _own_helpers(exe)
        self.assertEqual(set(helpers), {"sdivmod16", "udivmod16"})
        self.assertTrue(all(0x4000 <= a < 0x4400 for a in helpers.values()))
        self.assertLess(exe.symbol("main").value, 0x4000)
        self.assertEqual(_run(exe)[1], -176)

    def test_by_wildcard(self) -> None:
        script = _SCRIPT.replace("a.o(.text .text.*)", "*(.text .text.*)")
        exe = _build(_DIVIDE, script=script)
        helpers = _own_helpers(exe)
        self.assertTrue(all(0x0800 <= a < 0x1800 for a in helpers.values()))
        self.assertEqual(_run(exe)[1], -176)

    def test_unplaced(self) -> None:
        script = _SCRIPT.replace(
            ".runtime : { runtime.o(.text) } > lib", "")
        with self.assertRaisesRegex(AssertionError,
                                    r"`udivmod16` \(section \.text, "
                                    r"runtime\.o\)"):
            _build(_DIVIDE, script=script)


if __name__ == "__main__":
    unittest.main()
