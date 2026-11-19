"""Values kept in a callee's parameter bytes across calls
(passes/slot_sharing.py), parameters at a fixed zero-page address
(`zeropage(addr)` on a parameter),
and register parameters, which get no slot.

`TestShares` compiles single programs and checks which values share
their callee's bytes, from the `share` link-metadata records and the
code, and runs each in both pipelines. `TestAcrossUnits` links two
units, where a `const` declaration lets a caller keep a value in the
bytes of a function defined in the other, and checks the linker's
errors when the promise doesn't hold. `TestPinnedParams` and
`TestRegisterParams` cover the other two.

Under `-O`, a function whose declaration names no register takes its
leading parameters in A and X (`abi_selection._default_param_registers`),
and a parameter in a register has no bytes to share. So each function
here whose parameters are to arrive in slots names a register for one
parameter, such as `[[c6502::in_a]]` on a byte, and the default is off
for the rest of its parameters.
"""
from __future__ import annotations

import io
import re
import shutil
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from compile import _run_stage, main as compile_main
from parser import ParserError, parse
from passes.abi_selection import AbiSelectionError
from preprocessor import preprocess
from sim.harness import build_sim
from tests.no_inlining import no_inlining

_HAVE_PCPP = shutil.which("pcpp") is not None


def _codegen(src: str, optimize: bool = True) -> str:
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _shares(asm: str) -> list[str]:
    return [line[len("; share "):] for line in asm.splitlines()
            if line.startswith("; share ")]


def _body(asm: str, fn: str) -> list[str]:
    """The instruction lines of `fn`'s body."""
    text = asm.split(f"\n{fn}:\n")[1]
    out = []
    for line in text.splitlines():
        if line and not line.startswith((" ", "\t", ".")):
            break
        if line.startswith("   ") and line.strip() != "SUBROUTINE":
            out.append(" ".join(line.split()))
    return out


def _equs(asm: str) -> dict[str, int]:
    return {m.group(1): int(m.group(2), 16)
            for m in re.finditer(r"^(\S+)\tEQU\t\$([0-9A-F]+)$", asm, re.M)}


def _run(src: str, optimize: bool) -> int:
    return build_sim(src, optimize=optimize).run().return_int()


_PAGES = """
#include <stdint.h>
uint8_t buf[600];
uint16_t sum;
static void add_page(uint8_t i [[c6502::in_a]], const uint8_t *p) {
    do { sum += p[i]; i++; } while (i != 0);
}
static void forward(uint8_t i [[c6502::in_a]], const uint8_t *p) {
    add_page(i, p);
}
int main(void) {
    for (uint16_t k = 0; k < 600; k++) buf[k] = (uint8_t)(k * 7);
    const uint8_t *p = buf;
    uint8_t pages = 2;
    do {
        forward(0, p);
        p += 0x100;
    } while (--pages != 0);
    return sum;
}
"""
_PAGES_SUM = sum((k * 7) & 0xFF for k in range(512)) & 0xFFFF

_FIRST = """
#include <stdint.h>
uint8_t a[2] = {10, 20}, b[2] = {3, 4};
static uint8_t first(uint8_t k [[c6502::in_a]], const uint8_t *p) {
    return p[k];
}
"""


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
@no_inlining()
class TestShares(unittest.TestCase):

    def _check_runs(self, src: str, expected: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize), expected)

    def test_forwarded_parameter_and_held_local(self) -> None:
        asm = _codegen(_PAGES)
        shares = _shares(asm)
        self.assertEqual(len(shares), 1, shares)
        self.assertIn("member=add_page:", shares[0])
        self.assertIn("member=forward:", shares[0])
        self.assertIn("holder=main", shares[0])
        # `forward` passes its parameter on where it already is.
        self.assertEqual(_body(asm, "forward"), ["JMP add_page"])
        # The two slots are one pair of bytes.
        equs = _equs(asm)
        self.assertEqual(equs["__zpabi_forward__p_0"],
                         equs["__zpabi_add_page__p_0"])
        # `main` keeps `p` there, and its `p += 0x100` (out of SSA, a
        # copy through a temporary) updates it in place.
        loop = asm.split(".loop@2_start:")[1].split("RTS")[0]
        self.assertNotRegex(loop, r"__local_main__p\b")
        # The high byte of `p += 0x100` is an `INC`.
        self.assertIn("INC   __zpabi_forward__p_1", loop)
        self._check_runs(_PAGES, _PAGES_SUM)

    _STEPPED = """
#include <stdint.h>
uint16_t sum;
static void take(uint8_t w [[c6502::in_a]], const uint16_t s) {
    sum += s + w;
}
static void take_next(uint8_t w [[c6502::in_a]], const uint16_t s) {
    take(w, s);
}
static uint16_t first(void) { return 40; }
int main(void) {
    uint16_t s = first();
    take_next(1, ++s);
    take_next(2, ++s);
    take(3, ++s);
    take(4, ++s);
    return sum;
}
"""

    def test_stepped_counter(self) -> None:
        # Each `++s` is a new value, `t = s + 1` with `s` dead after,
        # and all of them, and `first`'s result, share one home: the
        # bytes `take_next` passes on to `take`. The call's result is
        # stored there, each step is an add in place, and nothing is
        # copied into `take`'s spelling of the bytes.
        asm = _codegen(self._STEPPED)
        shares = _shares(asm)
        self.assertEqual(len(shares), 1, shares)
        self.assertIn("member=take:", shares[0])
        self.assertIn("member=take_next:", shares[0])
        self.assertIn("holder=main", shares[0])
        self.assertEqual(_body(asm, "take_next"), ["JMP take"])
        main = _body(asm, "main")
        self.assertEqual(main[:3], [
            "JSR first", "STA __zpabi_take_next__s_0",
            "STX __zpabi_take_next__s_1",
        ])
        self.assertFalse(
            [line for line in main if "__zpabi_take__s" in line], main,
        )
        self.assertFalse(
            [line for line in main if "__local_main__" in line], main,
        )
        self._check_runs(self._STEPPED, 41 + 42 + 43 + 44 + 1 + 2 + 3 + 4)

    def test_other_argument_while_live(self) -> None:
        # `p` is still needed after `first(0, b)`, which writes the bytes.
        src = _FIRST + """
        int main(void) {
            const uint8_t *p = a;
            uint8_t s = first(0, p);
            s += first(0, b);
            return s + first(0, p);
        }
        """
        self.assertEqual(_shares(_codegen(src)), [])
        self._check_runs(src, 10 + 3 + 10)

    def test_writer_reached_while_live(self) -> None:
        # `other`, which `p` is live across, passes `first` another
        # pointer.
        src = _FIRST + """
        static uint8_t other(void) { return first(0, b); }
        int main(void) {
            const uint8_t *p = a;
            uint8_t s = first(0, p);
            s += other();
            return s + first(0, p);
        }
        """
        self.assertEqual(_shares(_codegen(src)), [])
        self._check_runs(src, 10 + 3 + 10)

    def test_global_forwarder_dropped_before_its_share(self) -> None:
        # `store` passes its parameter on to `push` with it live across
        # `next_byte`, in another unit, which might call `store` (rule
        # 3), so `store` can't keep it in `push`'s bytes. `peek` can
        # keep the value it pops there across `push(7, v)`, since `push`
        # is static. It had been dropped along with `store`, whose
        # external linkage made the share's owners global.
        src = """
        #include <stdint.h>
        uint8_t next_byte(void);
        void fail(void);
        uint16_t stack[8];
        uint8_t sp;
        static void push(uint8_t top [[c6502::in_a]], uint16_t value) {
            stack[sp++] = value;
            if (sp > top) fail();
        }
        static uint16_t pop(void) { return stack[--sp]; }
        void store(uint8_t top [[c6502::in_a]], uint16_t value) {
            if (next_byte() == 0) push(top, value);
        }
        uint16_t peek(void) { uint16_t v = pop(); push(7, v); return v; }
        """
        asm = _codegen(src)
        shares = _shares(asm)
        self.assertEqual(len(shares), 1, shares)
        self.assertIn("holder=peek", shares[0])
        self.assertNotIn("member=store:", shares[0])
        self.assertNotIn("__local_peek__", " ".join(_body(asm, "peek")))

    def test_callee_steps_a_copy(self) -> None:
        # `sum` steps `p` through the bytes, which out of SSA is a copy
        # of the parameter, so the parameter's bytes stay as they were
        # and `main` can keep its pointer there.
        src = """
        #include <stdint.h>
        uint8_t a[3] = {1, 2, 3};
        static uint8_t sum(const uint8_t *p, uint8_t n [[c6502::in_x]]) {
            uint8_t s = 0;
            do { s += *p++; } while (--n != 0);
            return s;
        }
        int main(void) {
            const uint8_t *p = a;
            uint8_t s = sum(p, 3);
            return s + sum(p, 2);
        }
        """
        self.assertTrue(any("holder=main" in s for s in _shares(_codegen(src))))
        self._check_runs(src, 6 + 3)

    def test_held_across_a_sibling_with_locals(self) -> None:
        # `p` lives in `first`'s bytes across `noise`, so `noise`'s
        # locals keep off them though `noise` and `first` are
        # siblings.
        src = _FIRST + """
        uint8_t out[8];
        static void noise(uint8_t k) {
            for (uint8_t i = 0; i < 8; i++) out[i] = (uint8_t)(i * k);
        }
        int main(void) {
            const uint8_t *p = a;
            uint8_t s = first(0, p);
            noise(5);
            s += first(0, p);
            noise(7);
            return s + first(0, p) + out[7];
        }
        """
        asm = _codegen(src)
        self.assertTrue(any("holder=main" in s for s in _shares(asm)))
        equs = _equs(asm)
        held = {equs["__zpabi_first__p_0"], equs["__zpabi_first__p_1"]}
        mine = {a for n, a in equs.items()
                if n.startswith(("__local_noise__", "__zpabi_noise__"))}
        self.assertFalse(held & mine)
        self._check_runs(src, 30 + 49)


_UNIT_DEF = """
#include <stdint.h>
uint16_t total;
void add_page(uint8_t i [[c6502::in_a]], const uint8_t *const p) {
    do { total += p[i]; i++; } while (i != 0);
}
"""

_UNIT_USE = """
#include <stdint.h>
extern uint16_t total;
void add_page(uint8_t i [[c6502::in_a]], const uint8_t *const p);
uint8_t buf[512];
int main(void) {
    for (uint16_t k = 0; k < 512; k++) buf[k] = (uint8_t)(k * 7);
    const uint8_t *p = buf;
    uint8_t pages = 2;
    do {
        add_page(0, p);
        p += 0x100;
    } while (--pages != 0);
    return total;
}
"""


def _link(sources: list[str]) -> tuple[int, str, str]:
    """Compiles each source with `-O` to an object and links them.
    Returns the exit code, the linked program as text
    (`link_helpers.linked_text`), and stderr."""
    from tests.link_helpers import (
        compile_object, link_capture_stderr, linked_text,
    )
    with tempfile.TemporaryDirectory() as tmp:
        paths = []
        for k, src in enumerate(sources):
            path = Path(tmp) / f"u{k}.o"
            assert compile_object(src, path) == 0
            paths.append(path)
        out = Path(tmp) / "out"
        rc, err = link_capture_stderr(paths, out)
        text = linked_text(out) if rc == 0 else ""
    return rc, text, err


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
@no_inlining()
class TestAcrossUnits(unittest.TestCase):

    def test_const_declaration_lets_the_caller_hold(self) -> None:
        shares = _shares(_codegen(_UNIT_USE))
        self.assertEqual(len(shares), 1, shares)
        self.assertIn("member=add_page:", shares[0])
        self.assertIn("holder=main", shares[0])
        self.assertIn("live=main:add_page", shares[0])

    def test_without_const_nothing_is_held(self) -> None:
        src = _UNIT_USE.replace("*const p", "*p")
        self.assertEqual(_shares(_codegen(src)), [])

    def test_linked_program_runs(self) -> None:
        from sim.linked import build_linked_sim
        rc, text, err = _link([_UNIT_DEF, _UNIT_USE])
        self.assertEqual(rc, 0, err)
        self.assertEqual(build_linked_sim(text.exe).run().return_int(),
                         _PAGES_SUM)

    def test_checks(self) -> None:
        from passes.slot_sharing import SlotShare, check_linked_shares
        share = SlotShare(
            members=(("add_page", ("__zpabi_add_page__p_0",
                                   "__zpabi_add_page__p_1")),),
            holders=("main",), live=(("main", ("add_page",)),),
            forwards=(("main", "add_page"),), writers=("main",),
        )
        calls = [("main", "add_page"), ("add_page", "helper"),
                 ("helper", "add_page2")]
        check_linked_shares([share], calls, {"main", "add_page"}, {})
        with self.assertRaisesRegex(ValueError, "`add_page` changes it"):
            check_linked_shares([share], calls, {"main", "add_page"}, {
                "add_page": {"__zpabi_add_page__p_1"},
            })
        with self.assertRaisesRegex(ValueError, "which no unit defines"):
            check_linked_shares([share], calls, {"main"}, {})
        # `other` calls the owner with something else, and `main` holds
        # its value across a call that reaches `other`.
        with self.assertRaisesRegex(ValueError, "`other`, which `add_page`"):
            check_linked_shares(
                [share], calls + [("helper", "other"),
                                  ("other", "add_page")],
                {"main", "add_page", "helper", "other"}, {},
            )

    def test_something_it_calls_writes_the_bytes(self) -> None:
        # `add_page` passes its parameter on to `scan`, so the two
        # share bytes, and then `reset` passes `scan` another pointer
        # while `main` still holds its own there.
        src = """
        #include <stdint.h>
        uint16_t total;
        uint8_t zeros[256];
        static void scan(uint8_t i [[c6502::in_a]], const uint8_t *p) {
            do { total += p[i]; i++; } while (i != 0);
        }
        static void reset(void) { scan(0, zeros); }
        void add_page(uint8_t i [[c6502::in_a]], const uint8_t *const p) {
            scan(i, p);
            reset();
        }
        """
        rc, _, err = _link([src, _UNIT_USE])
        self.assertNotEqual(rc, 0)
        self.assertIn("`reset`", err)


class TestMetadata(unittest.TestCase):

    def test_share_record_round_trips(self) -> None:
        from passes.slot_sharing import SlotShare
        from passes.zp_link_metadata import (
            LinkMetadata, format_metadata, parse_metadata,
        )
        share = SlotShare(
            members=(("f", ("__zpabi_f__p_0", "__zpabi_f__p_1")),
                     ("g", ("__zpabi_g__q_0", "__zpabi_g__q_1"))),
            holders=("main",),
            live=(("main", ("f", "h")),),
            forwards=(("f", "g"), ("main", "f")),
            writers=("main",),
        )
        meta = LinkMetadata(shares=[share])
        back = parse_metadata("\n".join(format_metadata(meta)))
        self.assertEqual(back.shares, [share])


# A routine at $D000 that returns the sum of the X bytes at ($E6),Y:
# LDY #0; LDA #0; loop: CLC; ADC ($E6),Y; INY; DEX; BNE loop; RTS.
_TOTAL = """
[[c6502::zp_abi, c6502::address(0xD000)]]
unsigned char total(const unsigned char *p [[c6502::zeropage(0xE6)]],
                    register unsigned char n [[c6502::in_x]]);
"""
_TOTAL_CODE = (0xD000, bytes([
    0xA0, 0x00, 0xA9, 0x00, 0x18, 0x71, 0xE6, 0xC8, 0xCA, 0xD0, 0xF9, 0x60,
]))


# A C function that sums the n bytes at p, taking p at $30 and n in X.
_SUM = """
unsigned char sum(const unsigned char *p [[c6502::zeropage(0x30)]],
                  register unsigned char n [[c6502::in_x]]) {
    unsigned char s = 0, i = 0;
    do { s += p[i]; i++; } while (--n != 0);
    return s;
}
"""
_SUM_DECL = """
unsigned char sum(const unsigned char *p [[c6502::zeropage(0x30)]],
                  register unsigned char n [[c6502::in_x]]);
"""
_SUM_MAIN = """
unsigned char data[4] = {1, 2, 3, 4};
int main(void) { return sum(data, 4) + sum(data + 1, 2); }
"""


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestPinnedParams(unittest.TestCase):

    def test_marker(self) -> None:
        fd = parse(
            "void f(const char *s [[c6502::zeropage(0xE6)]], "
            "register unsigned char n [[c6502::in_x]]);"
        ).declaration[0].function_decl
        self.assertEqual(fd.param_registers, ["@230", "X"])

    def test_parse_errors(self) -> None:
        for src, needle in [
            ("void f(register char *s [[c6502::zeropage(0xE6)]]);",
             "arrives in a register"),
            ("void f(char *s [[c6502::zeropage]]);",
             "takes the integer zero-page address"),
            ("void f(char *s [[c6502::zeropage(0x100)]]);",
             "outside"),
        ]:
            with self.subTest(src):
                with self.assertRaisesRegex(ParserError, needle):
                    parse(src)

    def test_defined_function_reads_it_in_place(self) -> None:
        src = _SUM + _SUM_MAIN
        asm = _codegen(src)
        equs = _equs(asm)
        self.assertEqual(equs["__zpabi_sum__p_0"], 0x30)
        self.assertEqual(equs["__zpabi_sum__p_1"], 0x31)
        self.assertIn("ADC (__zpabi_sum__p_0),Y", _body(asm, "sum"))
        self.assertIn("STA   __zpabi_sum__p_0", asm)
        self.assertIn("pinned=$30,$31,", asm)
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize), 10 + 5)

    def test_copied_when_a_callee_takes_one_there(self) -> None:
        # `first`'s callers store its argument where `both` took `q`,
        # and `both` reads `q` after calling it.
        src = """
        unsigned char first(
            const unsigned char *p [[c6502::zeropage(0xE6)]]) {
            return p[0];
        }
        unsigned char both(
            const unsigned char *q [[c6502::zeropage(0xE6)]],
            const unsigned char *r) {
            unsigned char s = first(r);
            return s + q[1];
        }
        unsigned char a[2] = {10, 20}, b[2] = {3, 4};
        int main(void) { return both(a, b); }
        """
        asm = _codegen(src)
        self.assertEqual(_body(asm, "both")[:4], [
            "LDA __zpabi_both__q_0", "STA __local_both__q",
            "LDA __zpabi_both__q_1", "STA __local_both__2",
        ])
        self.assertEqual(
            [line for line in _body(asm, "first") if "__zpabi" in line],
            ["LDA (__zpabi_first__p_0),Y"],
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize), 3 + 20)

    def test_copied_when_calling_another_unit(self) -> None:
        # `elsewhere` might call a function that takes its argument at
        # $30.
        asm = _codegen("""
        void elsewhere(void);
        unsigned char f(
            const unsigned char *p [[c6502::zeropage(0x30)]]) {
            elsewhere();
            return *p;
        }
        """)
        self.assertEqual(_body(asm, "f")[0], "LDA __zpabi_f__p_0")

    def test_needs_zp_abi(self) -> None:
        src = """
        unsigned char f(const char *s [[c6502::zeropage(0xE6)]]) {
            return *s ? f(s + 1) + 1 : 0;
        }
        int main(void) { return f("abc"); }
        """
        with self.assertRaisesRegex(
            AbiSelectionError, r"declares a `zeropage\(addr\)` parameter",
        ):
            _codegen(src)
        # Without -O, the function takes it on the soft stack,
        # as its callers pass it.
        self.assertEqual(_run(src, False), 3)

    def test_declarations_agree(self) -> None:
        with self.assertRaisesRegex(AbiSelectionError, "disagree"):
            _codegen("""
            void f(const char *s [[c6502::zeropage(0xE6)]]);
            void f(const char *s) { (void)s; }
            int main(void) { f("a"); return 0; }
            """)

    def test_linked(self) -> None:
        from sim.linked import build_linked_sim
        use = _SUM_DECL + _SUM_MAIN
        rc, text, err = _link([_SUM, use])
        self.assertEqual(rc, 0, err)
        self.assertEqual(_equs(text)["__zpabi_sum__p_0"], 0x30)
        self.assertEqual(build_linked_sim(text.exe).run().return_int(), 15)
        # A unit that puts the argument somewhere else doesn't link.
        rc, _, err = _link([_SUM, use.replace("0x30", "0x40")])
        self.assertNotEqual(rc, 0)
        self.assertIn("different parameters", err)

    def test_arguments_go_to_the_address(self) -> None:
        src = _TOTAL + """
        unsigned char data[4] = {1, 2, 3, 4};
        int main(void) { return total(data, 4) + total(data + 1, 2); }
        """
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                asm = _codegen(src, optimize)
                equs = _equs(asm)
                self.assertEqual(equs["__zpabi_total__p_0"], 0xE6)
                self.assertEqual(equs["__zpabi_total__p_1"], 0xE7)
                self.assertIn("STA   __zpabi_total__p_0", asm)
                # Nothing else of the compiler's goes there.
                others = {n for n, a in equs.items()
                          if a in (0xE6, 0xE7) and "total" not in n}
                self.assertEqual(others, set())
                sim = build_sim(src, optimize=optimize)
                addr, code = _TOTAL_CODE
                sim.mpu.memory[addr:addr + len(code)] = code
                self.assertEqual(sim.run().return_int(), 10 + 5)

    def test_runtime_block_moves_off_it(self) -> None:
        src = """
        [[c6502::zp_abi, c6502::address(0xD000)]]
        void put(const unsigned char *p [[c6502::zeropage(0x04)]]);
        unsigned char data[1];
        int main(void) { put(data); return 0; }
        """
        asm = _codegen(src)
        equs = _equs(asm)
        size = int(re.search(r"hargs_size=(\d+)", asm).group(1))
        block = set(range(equs["HARGS"], equs["HARGS"] + size))
        for name in ("SSP", "FP", "DPTR"):
            block |= {equs[name], equs[name] + 1}
        self.assertFalse(block & {0x04, 0x05})

    def test_recorded_for_the_linker(self) -> None:
        asm = _codegen(_TOTAL + """
        unsigned char data[1];
        int main(void) { return total(data, 1); }
        """)
        self.assertIn("pinned=$E6,$E7,", asm)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestRegisterParams(unittest.TestCase):

    def test_no_slot(self) -> None:
        asm = _codegen("""
        unsigned char out;
        void put(register unsigned char k [[c6502::in_a]],
                 unsigned char *p) { *p = k; }
        int main(void) { put(5, &out); return out; }
        """)
        equs = _equs(asm)
        self.assertNotIn("__zpabi_put__k", equs)
        self.assertEqual(equs["__zpabi_put__p_0"], 0x80)

    def test_kept_in_its_register_sets_no_flags(self) -> None:
        # From the calls fuzzer: `p1`'s local stays in X, where it
        # arrives, and its entry copy `Mov(X, X)` was taken for an
        # `LDX`, so the `TXA` before the sign test of `p1` went and
        # the `BMI` tested `p0`, left in the flags by the caller.
        src = """
        unsigned char g0 = 104;
        unsigned long g2 = 132;
        int f0(register signed char p0, register signed char p1) {
            signed char l0 = ((p1 % (g2 | 1)) & (p0 % (g0 | 1)));
            return l0;
        }
        int main(void) {
            long r = (long)f0(215, 72);
            return (int)(r ^ (r >> 16));
        }
        """
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize), 64)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestPassedToTwoCallees(unittest.TestCase):
    """A parameter passed on to two callees shares both callees'
    bytes, so the function copies it for neither, as Zork's
    `is_separator(c)` passes `c` to `is_dict_separator` and then to
    `is_std_separator`. Each function here takes a step count in X,
    so that `c` arrives in its slot."""

    _SRC = """
    #include <stdint.h>
    #include <stdbool.h>
    uint8_t calls;
    static bool is_a(uint8_t step [[c6502::in_x]], uint8_t c) {
        calls += step;
        return c == 'a';
    }
    static bool is_b(uint8_t step [[c6502::in_x]], uint8_t c) {
        calls += step;
        return c == 'b';
    }
    static bool is_ab(uint8_t step [[c6502::in_x]], uint8_t c) {
        if (is_a(step, c)) return true;
        return is_b(step, c);
    }
    int main(void) {
        uint8_t n = 0;
        for (uint8_t c = 'a'; c != 'e'; c++)
            n = (uint8_t)(n << 1) | is_ab(1, c);
        return n * 16 + calls;
    }
    """

    def test_one_share(self):
        asm = _codegen(self._SRC)
        shares = _shares(asm)
        self.assertEqual(len(shares), 1, shares)
        for f in ("is_a", "is_b", "is_ab"):
            self.assertIn(f"member={f}:", shares[0])
        equs = _equs(asm)
        self.assertEqual(equs["__zpabi_is_a__c"], equs["__zpabi_is_b__c"])
        self.assertEqual(equs["__zpabi_is_ab__c"], equs["__zpabi_is_b__c"])
        # Nothing is copied between the calls.
        self.assertFalse(any("__zpabi_" in line
                             for line in _body(asm, "is_ab")),
                         _body(asm, "is_ab"))

    def test_runs(self):
        # 'a' and 'b' are separators, 'c' and 'd' aren't. is_ab('a')
        # calls once, the others twice.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(self._SRC, optimize),
                                 0b1100 * 16 + 7)
