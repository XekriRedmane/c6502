"""Writes records of Python's `re` for `c6502cpp check-regex`, which
compares py_regex (cpp/src/py_regex.*) with them.

Each record is one line of JSON: a pattern, what `re.compile` raised
for it, and for a pattern that compiled, the end of `.match` on each of
a set of subjects (cpp/src/py_regex_cases.h describes the fields). The
patterns are the kinds a user would give `--passthru-includes` and the
common errors, random patterns built from the syntax py_regex takes,
random strings of the characters that are special in a pattern, most of
which are invalid, and random patterns under IGNORECASE of the
characters whose cases `re` folds in unusual ways. The subjects are
include names and random strings over the characters the patterns use.

    uv run python tools/py_regex_records.py <out.jsonl> [--count N] [--seed S]
    cpp/build/bin/c6502cpp check-regex <out.jsonl>

A pattern that makes Python print a warning is recorded too, since the
C++ reports it as unsupported, and `check-regex` counts those apart.
"""

from __future__ import annotations

import argparse
import json
import random
import re
import warnings

# Patterns a user might write for the names of includes.
_INCLUDE_PATTERNS = [
    r".*", r"<.*>", r'".*"', r"<std.*\.h>", r'"foo\.h"', r'"(foo|bar)\.h"',
    r"<(?:stdio|stdlib|string)\.h>", r"(?i)<STD", r"(?i:<std)io\.h>",
    r"^<sys/", r'.*config\.h"$', r"(?!<std)", r'(?!<std).*\.h[">]$',
    r'"[a-z_]+\.h"', r'"[^/]+"', r"<[\w/]+\.h>", r"<\w+>", r"\S+",
    r'"\w*"', r".*\binc\b.*", r".*/(?=x)", r".*(?<=\.h)>", r"(?<!x)\.h",
    r"<.{3,5}>", r"<.{,4}>", r"<.{2}", r'"a+?"', r'"a*+"', r'"(?>a*)a"',
    r"(?x) < std  # comment\n .* ", r"(?s).*", r"(?m)^<", r"(?a)\w+",
    r"(?u)\w+", r"\A<", r"\Z", r'"\d+\.h"', r"[<\"]", r'[^"]*', r"<a|b>",
    r"(<)(a)", r"(?P<name>a)", r"(?#a comment)<", r'"[\x41-\x5A]"',
    r"A", r"\101", r"\0", r"[\d\s]", r"[\D]", r"\.", r"\\", r"\n",
    r"(?i)[a-z]+", r"(?i)K", r"(?i)k", r"(?i)[k]", r"(?i)[^k]",
    r"(?i)s", r"(?i)ſ", r"(?i)[Ā-ſ]", r"(?ai)k",
    r"(?i)\U00010400", r"(?i)[\U00010400x]", r"(?i)\U00010400|x",
    r"(?i)[\U00010400-\U00010410]", r"(?:a|ab){2}+", r"(?:a?){2}b",
    r"(?:a?)*?b", r"(a|)*b", r"(?:)*", r"(?:a|)+?a", r"(?:|a)*a",
    # The common errors.
    r"(", r")", r"a)", r"(a", r"*", r"a**", r"\q", r"[a", r"[b-a]",
    r"[\d-z]", r"\x4", r"a{3,2}", r"(?<a>x)", r"(?P<1>x)", r"(?P=x)",
    r"\1", r"(a)\2", r"(?i", r"(?z)", r"a(?i)", r"(?-a:x)", r"(?L)",
    r"(?au)", r"(?a)(?u)", r"a{4294967295}", r"(?<=a*)", r"\N", r"\N{",
    r"(?", r"(?P", r"(?Px)", r"(?<x)", r"[", r"\\\\\\", "\\", r"(?#x",
    r"(?(1)a)", r"(?(x)a)", r"(?(0)a)", r"x(?t)", r"(?t)a*", r"(?t)a",
    r"(?i-i:a)", r"(?-:a)", r"(?i-:a)", r"(?x:a b)", r"(?-x:a)",
    r"\8", r"[\8]", r"\777", r"[\777]", r"\U00110000", "a\n(", "a\n)\nb",
    r"[[a]", r"[a--b]", r"[a&&b]", r"a{,}", r"a{", r"a{1", r"a{1,", r"{",
    r"{}", r"x{}", r"a{1}{2}", r"a*?+", r"(?>a)*", r"(?=a)*", r"\b*",
]

# The atoms of the random patterns, with a weight toward literals.
_ATOMS = [
    "a", "a", "b", "b", "<", ">", '"', "\\.", ".", "\\d", "\\w", "\\s",
    "\\W", "\\S", "\\D", "[ab]", "[^a]", "[a-c]", "[\\w.]", "[^\\s]", "_",
    "A", "K", "\\u212a", "x", "h", "/", "\\b", "\\B", "^", "$", "\\A",
    "\\Z", "\\n", "\\x41", "é", "[é-ê]", "[^>]",
]
_REPEATS = ["*", "+", "?", "{2}", "{1,3}", "{,2}", "{2,}", "*?", "+?",
            "??", "{1,2}?", "*+", "++", "?+", "{1,2}+"]
_POSSESSIVE = {"*+", "++", "?+", "{1,2}+"}
_FLAGS = ["i", "m", "s", "x", "a", "u", "im", "is", "ix"]


def _random_pattern(rng: random.Random, depth: int = 0) -> str:
    """A random pattern of the syntax py_regex takes, with groups,
    alternations, repeats, lookarounds, and flags."""
    parts = []
    for _ in range(rng.randint(1, 4)):
        r = rng.random()
        if r < 0.55 or depth > 2:
            atom = rng.choice(_ATOMS)
        elif r < 0.65:
            atom = "(?:" + _random_pattern(rng, depth + 1) + ")"
        elif r < 0.72:
            atom = "(" + _random_pattern(rng, depth + 1) + ")"
        elif r < 0.78:
            atom = ("(?" + rng.choice(["=", "!"]) + _random_pattern(rng, depth + 1)
                    + ")")
        elif r < 0.83:
            # A lookbehind of a fixed width.
            inner = "".join(rng.choice(["a", "b", ".", "\\w", "[ab]"])
                            for _ in range(rng.randint(1, 2)))
            atom = "(?<" + rng.choice(["=", "!"]) + inner + ")"
        elif r < 0.88:
            atom = "(?>" + _random_pattern(rng, depth + 1) + ")"
        elif r < 0.93:
            flags = rng.choice(_FLAGS)
            if rng.random() < 0.3:
                flags += "-" + rng.choice(["i", "m", "s", "x"])
                if flags[0] == flags[-1]:
                    flags = flags[:-2]
            atom = "(?" + flags + ":" + _random_pattern(rng, depth + 1) + ")"
        else:
            atom = "(?P<g" + str(rng.randint(0, 9)) + ">" + \
                _random_pattern(rng, depth + 1) + ")"
        if rng.random() < 0.35 and atom not in ("^", "$", "\\b", "\\B", "\\A",
                                                "\\Z"):
            rep = rng.choice(_REPEATS)
            # py_regex doesn't take a possessive repeat of a group, so
            # most groups get the greedy form instead.
            if (atom.startswith("(") and rep in _POSSESSIVE
                    and rng.random() < 0.9):
                rep = rep[:-1]
            atom += rep
        parts.append(atom)
    pattern = "".join(parts)
    if rng.random() < 0.2:
        pattern += "|" + _random_pattern(rng, depth + 1)
    if depth == 0 and rng.random() < 0.1:
        pattern = "(?" + rng.choice(_FLAGS) + ")" + pattern
    return pattern


# Characters whose cases `re` folds in unusual ways: the letters
# `_EXTRA_CASES` joins, the Kelvin sign, the title-case digraphs, and
# letters outside the Basic Multilingual Plane.
_CASE_CHARS = (
    "aAkKsSiIıİſKµΜμͅιΙ"
    "ιßẞàÀσςΣǄǅǆ"
    "\U00010400\U00010428\U0001e900\U0001e922_1 "
)


def _random_case_pattern(rng: random.Random) -> str:
    """A pattern under IGNORECASE of literals and classes of the
    characters `re` folds in unusual ways."""
    parts = []
    for _ in range(rng.randint(1, 3)):
        r = rng.random()
        if r < 0.4:
            parts.append(re.escape(rng.choice(_CASE_CHARS)))
        elif r < 0.85:
            items = []
            for _ in range(rng.randint(1, 3)):
                a, b = sorted(rng.choice(_CASE_CHARS) for _ in range(2))
                items.append(re.escape(a) if rng.random() < 0.5
                             else f"{re.escape(a)}-{re.escape(b)}")
            if rng.random() < 0.2:
                items.append(rng.choice(["\\w", "\\W", "\\d", "\\s"]))
            parts.append("[" + ("^" if rng.random() < 0.3 else "")
                         + "".join(items) + "]")
        else:
            parts.append(re.escape(rng.choice(_CASE_CHARS)) + "|"
                         + re.escape(rng.choice(_CASE_CHARS)))
        if rng.random() < 0.2:
            parts[-1] = "(?:" + parts[-1] + ")" + rng.choice(["*", "+", "?"])
    flags = rng.choice(["(?i)", "(?i)", "(?ai)", "(?iu)"])
    return flags + "".join(parts)


def _case_subjects(rng: random.Random) -> list[str]:
    return ["".join(rng.choice(_CASE_CHARS) for _ in range(rng.randint(0, 4)))
            for _ in range(40)]


_PATTERN_CHARS ="ab()[]{}*+?|^$\\.-,:0123=!<>P#xiamsuLtN-&~"


def _random_text(rng: random.Random) -> str:
    return "".join(rng.choice(_PATTERN_CHARS) for _ in range(rng.randint(1, 8)))


_SUBJECT_CHARS = "ab<>\".hx_ /\nABKk1éKſs"
_SUBJECTS = [
    "", '"foo.h"', '"bar.h"', "<stdio.h>", "<stdlib.h>", "<sys/types.h>",
    '"config.h"', '"a.h"', "<STDIO.H>", '"inc/x.h"', "<a>", "<b>", "a",
    "aa", "ab", "aab", "abab", "b", "\n", "a\n", "K", "k", "K", "s",
    "S", "ſ", "\U00010400", "\U00010428", "x", "_", "A", "é",
]


def _subjects(rng: random.Random) -> list[str]:
    out = list(_SUBJECTS)
    for _ in range(16):
        out.append("".join(rng.choice(_SUBJECT_CHARS)
                           for _ in range(rng.randint(0, 9))))
    return out


def record(pattern: str, subjects: list[str]) -> dict:
    """Python's `re.compile(pattern)`, and `.match` on each subject."""
    rec: dict = {"pattern": pattern, "error": None}
    with warnings.catch_warnings(record=True) as caught:
        warnings.simplefilter("always")
        try:
            compiled = re.compile(pattern)
        except (re.error, OverflowError, ValueError) as e:
            rec["error"] = [type(e).__name__, str(e)]
            compiled = None
    if caught:
        rec["warning"] = str(caught[0].message)
    if compiled is not None:
        rec["subjects"] = []
        rec["ends"] = []
        for s in subjects:
            try:
                m = compiled.match(s)
            except SystemError:
                # Python 3.11's matcher reports "The span of capturing
                # group is wrong" for some possessive repeats of groups,
                # which is a bug of its own, so the subject is left out.
                continue
            rec["subjects"].append(s)
            rec["ends"].append(m.end() if m else -1)
    return rec


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("out")
    ap.add_argument("--count", type=int, default=5000,
                    help="random patterns of each kind")
    ap.add_argument("--seed", type=int, default=1)
    args = ap.parse_args()
    rng = random.Random(args.seed)
    patterns = list(_INCLUDE_PATTERNS)
    patterns += [_random_pattern(rng) for _ in range(args.count)]
    patterns += [_random_text(rng) for _ in range(args.count)]
    case_patterns = {_random_case_pattern(rng) for _ in range(args.count)}
    seen = set()
    re.purge()
    with open(args.out, "w", encoding="utf-8", newline="\n") as f:
        for p in patterns + sorted(case_patterns):
            if p in seen:
                continue
            seen.add(p)
            subjects = (_case_subjects(rng) if p in case_patterns
                        else _subjects(rng))
            rec = record(p, subjects)
            f.write(json.dumps(rec, ensure_ascii=False) + "\n")
    print(f"wrote {len(seen)} records to {args.out}")


if __name__ == "__main__":
    main()
