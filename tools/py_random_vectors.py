"""Writes cpp/tests/frozen/py_random.txt, the results of CPython's
`random.Random` that cpp/tests/test_py_random.cpp compares the C++ port
(cpp/src/py_random.h) with. The file stays the reference once Python
is gone, so this tool runs only while it is installed, under CPython
3.11, whose `random` the fuzzers' seeds were drawn from.

Each seed's block starts with a `seed` line and lists one call per
line, with its arguments and its result:

    seed int <n> | seed str <JSON string>
    genrand <32-bit output>
    random <the double's bits, 16 hex digits>
    getrandbits <k> <value>
    randbelow <n> <value>
    randrange <stop> <value>
    randrange2 <start> <stop> <value>
    randint <a> <b> <value>
    choice <n> <index>                  (of range(n))
    shuffle <n> <the permutation of range(n)>
    choices <k> <comma-separated weights> <indices>
    choicesn <n> <k> <indices>          (no weights)
    sample <n> <k> <indices>            (of range(n))
"""
from __future__ import annotations

import json
import random
import struct
import sys
from pathlib import Path

OUT = Path(__file__).resolve().parent.parent / "cpp" / "tests" / "frozen" / "py_random.txt"

INT_SEEDS = [0, 1, 2, 23, 64, 6502, -5, 7919 + 1, 42 * 7919 + 1, 99 * 104729 + 3,
             299 * 15485863 + 5, 1000 * 1000 + 500, 2**32 - 1, 2**32, 2**64 + 3,
             2**100 + 12345, -(2**70)]
STR_SEEDS = ["", "bitint0", "bitint99", "bin16_to_bcd",
             "['asl16', 'mul16']unsigned long", "é€\U0001F600",
             "x" * 55, "x" * 111, "x" * 112, "y" * 200, "\0abc"]


def lines_for(rng: random.Random) -> list[str]:
    out = []
    for _ in range(3):
        out.append(f"genrand {rng.getrandbits(32)}")
    for _ in range(4):
        out.append(f"random {struct.pack('>d', rng.random()).hex()}")
    for k in (0, 1, 4, 8, 16, 31, 32, 33, 48, 63, 64, 65, 100, 127):
        out.append(f"getrandbits {k} {rng.getrandbits(k)}")
    for n in (1, 2, 3, 7, 100, 255, 256, 2**31, 2**32 + 1, 2**64, 2**64 + 1, 10**30):
        out.append(f"randbelow {n} {rng._randbelow(n)}")
    for stop in (1, 5, 256, 65536):
        out.append(f"randrange {stop} {rng.randrange(stop)}")
    for start, stop in ((0, 10), (-100, 100), (5, 6), (-(2**63), 2**63)):
        out.append(f"randrange2 {start} {stop} {rng.randrange(start, stop)}")
    for a, b in ((0, 0), (0, 1), (1, 6), (-128, 127), (0, 2**64 - 1), (10, 20)):
        out.append(f"randint {a} {b} {rng.randint(a, b)}")
    for n in (1, 2, 5, 17, 300):
        seq = list(range(n))
        out.append(f"choice {n} {rng.choice(seq)}")
    for n in (0, 1, 2, 5, 13):
        seq = list(range(n))
        rng.shuffle(seq)
        out.append(f"shuffle {n} {' '.join(map(str, seq)) or '-'}")
    for weights, k in (([1, 1], 3), ([5, 1, 0, 2], 6), ([0.5, 0.25, 0.125], 4),
                       ([3], 2), ([10, 20, 30, 40, 50, 60], 5)):
        got = rng.choices(range(len(weights)), weights=weights, k=k)
        out.append(f"choices {k} {','.join(map(repr, weights))} {' '.join(map(str, got))}")
    for n, k in ((1, 2), (7, 5), (100, 3)):
        got = rng.choices(range(n), k=k)
        out.append(f"choicesn {n} {k} {' '.join(map(str, got))}")
    for n, k in ((8, 1), (8, 4), (3, 0), (3, 3), (5, 2), (40, 6), (1000, 7), (30, 21)):
        got = rng.sample(range(n), k)
        out.append(f"sample {n} {k} {' '.join(map(str, got)) or '-'}")
    return out


def main() -> int:
    if sys.version_info[:2] != (3, 11):
        print("py_random_vectors.py needs CPython 3.11", file=sys.stderr)
        return 1
    text = []
    for s in INT_SEEDS:
        text.append(f"seed int {s}")
        text += lines_for(random.Random(s))
    for s in STR_SEEDS:
        text.append(f"seed str {json.dumps(s)}")
        text += lines_for(random.Random(s))
    with open(OUT, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(text) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
