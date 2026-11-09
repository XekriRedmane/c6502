"""Simulates every constant shift count, and multiplies by constants of
the form 2^a + 2^b and 2^a - 2^b, at each integer width.

`tac_to_asm` inlines a constant shift `8q + r` as byte moves and `r`
one-bit steps when `passes.shift_cost.inline_shift_ok` accepts it, and
calls the `asl*` / `lsr*` / `asr*` helper otherwise. Under --optimize,
`strength_reduction` rewrites the multiplies into shifts and an Add or
Subtract. Sweeping every count from 1 to width-1 runs both the inline
and the helper paths, and the boundary between them, so each result
is compared with the value Python computes at the same width.

Each operation is its own function, which keeps every unoptimized
frame small and keeps the argument a runtime value. The functions are
split across programs of at most `_CHUNK` functions. The sim loads
code at $0800 and the soft stack grows down from $7FFF, and an
unoptimized program holding all 148 of the 64-bit functions reaches
the stack. Nothing checks for that, and the results come back wrong."""

from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim

# (C type, bytes)
_TYPES = [
    ("int", 2),
    ("unsigned int", 2),
    ("long", 4),
    ("unsigned long", 4),
    ("long long", 8),
    ("unsigned long long", 8),
]

_MULTIPLIERS = [3, 5, 6, 7, 9, 10, 12, 15, 17, 24, 31, 33, 40, 48, 63,
                65, 96, 127, 129, 192, 255, 257]

# Bit patterns, truncated to each width: one with the sign bit set and
# one without, both with bits set in every byte.
_INPUTS = [0xF3A5C3961E7D4B82, 0x5A3C69E1B2D4870F]

_CHUNK = 40


def _signed(v: int, size: int) -> int:
    bits = size * 8
    v &= (1 << bits) - 1
    return v - (1 << bits) if v >> (bits - 1) else v


def _programs(ctype: str, size: int, x: int):
    """Yields `(source, expected)` pairs. Each source has one function
    per shift or product, storing its result into a global, and
    `expected` maps each global to the value it should hold afterwards
    (as an unsigned bit pattern)."""
    bits = size * 8
    mask = (1 << bits) - 1
    signed = not ctype.startswith("unsigned")
    exprs: dict[str, str] = {}
    expected: dict[str, int] = {}
    for k in range(1, bits):
        exprs[f"l{k}"] = f"x << {k}"
        expected[f"l{k}"] = (x << k) & mask
        exprs[f"r{k}"] = f"x >> {k}"
        sx = _signed(x, size) if signed else x
        expected[f"r{k}"] = (sx >> k) & mask
    for c in _MULTIPLIERS:
        exprs[f"m{c}"] = f"x * {c}"
        expected[f"m{c}"] = (x * c) & mask
    lit = f"{x}ULL" if size == 8 else f"{x}UL"
    names = list(exprs)
    for start in range(0, len(names), _CHUNK):
        chunk = names[start:start + _CHUNK]
        src = "".join(
            f"{ctype} {name};\n"
            f"void f_{name}({ctype} x) {{ {name} = {exprs[name]}; }}\n"
            for name in chunk
        )
        calls = "".join(f"    f_{name}(v);\n" for name in chunk)
        src += (
            f"int main(void) {{\n    {ctype} v = ({ctype}){lit};\n"
            + calls + "    return 0;\n}\n"
        )
        yield src, {name: expected[name] for name in chunk}


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestConstShiftMul(unittest.TestCase):

    def _check(self, optimize: bool) -> None:
        for ctype, size in _TYPES:
            for raw in _INPUTS:
                x = raw & ((1 << (size * 8)) - 1)
                for src, expected in _programs(ctype, size, x):
                    first = next(iter(expected))
                    with self.subTest(type=ctype, x=hex(x), first=first):
                        self._run(src, expected, size, optimize)

    def _run(self, src, expected, size, optimize) -> None:
        sim = build_sim(src, optimize=optimize)
        sim.run()
        mem = sim.mpu.memory
        for name, want in expected.items():
            addr = sim.symbols[name]
            got = int.from_bytes(bytes(mem[addr:addr + size]), "little")
            self.assertEqual(
                got, want, msg=f"{name}: got {got:#x}, want {want:#x}",
            )

    def test_unoptimized(self) -> None:
        self._check(optimize=False)

    def test_optimized(self) -> None:
        self._check(optimize=True)



@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestByteShift(unittest.TestCase):
    """`(uint8_t)(x << k)` narrows to a 1-byte shift, which runs in A."""

    def test_narrowed_left_shift(self):
        xs = [0x00, 0x01, 0x5A, 0x80, 0xA5, 0xFF]
        src = "unsigned char r[56];\n"
        src += "unsigned char xs[6] = {" + ", ".join(map(str, xs)) + "};\n"
        for k in range(1, 8):
            src += (
                f"unsigned char s{k}(unsigned char x) "
                f"{{ return (unsigned char)(x << {k}); }}\n"
            )
        src += "int main(void) {\n    for (unsigned char i = 0; i < 6; i++) {\n"
        for k in range(1, 8):
            src += f"        r[i * 7 + {k - 1}] = s{k}(xs[i]);\n"
        src += "    }\n    return 0;\n}\n"
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                sim.run()
                base = sim.symbols["r"]
                for i, x in enumerate(xs):
                    for k in range(1, 8):
                        got = sim.mpu.memory[base + i * 7 + k - 1]
                        self.assertEqual(
                            got, (x << k) & 0xFF, msg=f"{x:#x} << {k}",
                        )


if __name__ == "__main__":
    unittest.main()
