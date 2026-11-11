"""Random call-graph fuzzer, aimed at the calling conventions.

Each program has three to six functions f0..fN arranged as a DAG (fi
only calls fj with j > i), plus extras chosen per seed:
  * a recursive function `rec`, which can't get zp_abi and so stays
    on the soft stack,
  * the last function called through a function pointer, which also
    puts it (and every function making that indirect call) on the
    soft stack,
  * `register` parameters (A / X) and a `register unsigned char`
    local (Y), on functions that will get zp_abi,
  * pointer parameters that receive the address of a caller's local,
  * calls inside loops, and globals updated across calls,
  * functions marked `__attribute__((return_bool_in_carry))`, which
    return `e != 0` in the carry flag, called for their value or as
    an `if` condition.
Function bodies compute random integer expressions over parameters,
locals, and call results, and keep earlier locals live across the
calls. Every bug this fuzzer found involved a value live across a
call between two different calling conventions.

The TAC interpreter is the reference, which is fair for backend bugs
because zp_abi, frames, and register placement are all decided after
TAC. Without --optimize every function uses the soft stack; with it,
the eligible ones get zp_abi.

Undefined behavior is avoided by construction. Divisors are `(e | 1)`
and shift counts are `(e & 7)`. Signed overflow does occur, and every
path wraps it the same way (two's complement), so it doesn't cause
disagreements.

    uv run python -m fuzz.calls --start 0 --count 300
"""

from __future__ import annotations

import random
import sys

from fuzz.runner import fuzz_main

TYPES = ["signed char", "unsigned char", "int", "unsigned int",
         "long", "unsigned long"]
SIZE = {"signed char": 1, "unsigned char": 1, "int": 2,
        "unsigned int": 2, "long": 4, "unsigned long": 4}

# Recursion forces the soft stack (a zp_abi function can't be on a
# call-graph cycle).
_REC = ("int rec(int d, int x) { if (d <= 0) return x & 255; "
        "return rec(d - 1, (x * 3 + d) & 1023) + 1; }")


def _expr(rng: random.Random, vars_: list[str], depth: int = 2) -> str:
    """A random integer expression over `vars_` with no undefined
    division or shift."""
    if depth == 0 or rng.random() < 0.3:
        if vars_ and rng.random() < 0.8:
            return rng.choice(vars_)
        return str(rng.choice([0, 1, 2, 3, 7, 100, 255, 1000, 32767]))
    k = rng.random()
    a = _expr(rng, vars_, depth - 1)
    if k < 0.1:
        return f"(({rng.choice(TYPES)})({a}))"
    if k < 0.17:
        return f"({rng.choice(['-', '~', '!'])}{a})"
    b = _expr(rng, vars_, depth - 1)
    op = rng.choice(["+", "-", "*", "/", "%", "&", "|", "^", "<<", ">>",
                     "<", ">", "<=", ">=", "==", "!=", "&&", "||"])
    if op in ("/", "%"):
        return f"(({a}) {op} (({b}) | 1))"
    if op in ("<<", ">>"):
        return f"(({a}) {op} (({b}) & 7))"
    return f"(({a}) {op} ({b}))"


def make_program(seed: int) -> tuple[str, None]:
    """The program for `seed`. The expected return is None, meaning
    the TAC result is the reference."""
    rng = random.Random(seed)
    # The carry-flag choices come from a stream of their own, so the
    # rest of each seed's program is what it was before they existed.
    crng = random.Random(seed * 7919 + 1)
    n = rng.randint(3, 6)
    out = []
    globals_ = []
    for i in range(rng.randint(1, 3)):
        globals_.append(f"g{i}")
        out.append(f"{rng.choice(TYPES)} g{i} = {rng.randint(0, 200)};")
    use_rec = rng.random() < 0.4
    use_ptr_fn = rng.random() < 0.4

    sigs = {}
    for i in range(n):
        ret = rng.choice(TYPES)
        params = [(rng.choice(TYPES), f"p{k}")
                  for k in range(rng.randint(0, 4))]
        if rng.random() < 0.25:
            # Receives the address of a local in each caller.
            params.append(("int *", "pp"))
        sigs[i] = (ret, params)
    calls = {}
    for i in range(n):
        callees = list(range(i + 1, n))
        rng.shuffle(callees)
        calls[i] = callees[:rng.randint(0, 2)]
    calls_rec = {i: use_rec and rng.random() < 0.5 for i in range(n)}
    # A carry-flag function returns `bool` (`_Bool`), and its address
    # can't be taken, so the pointer-called one is left out.
    carry = {
        i for i in range(n)
        if crng.random() < 0.3 and not (use_ptr_fn and i == n - 1)
    }
    for i in carry:
        sigs[i] = ("_Bool", sigs[i][1])

    # Which functions end up on the soft stack under --optimize: the
    # pointer-called one, each caller through the pointer, each
    # caller of `rec`, and every caller of one of those (a zp_abi
    # function can only call zp_abi functions). `register` is only
    # legal on the rest, since it needs zp_abi.
    soft = set()
    if use_ptr_fn:
        soft.add(n - 1)
        soft |= {i for i in range(n) if n - 1 in calls[i]}
    soft |= {i for i in range(n) if calls_rec[i]}
    for i in reversed(range(n)):
        if any(j in soft for j in calls[i]):
            soft.add(i)
    reg_ok = set(range(n)) - soft

    lines = [_REC] if use_rec else []
    for i in reversed(range(n)):
        ret, params = sigs[i]
        decl = []
        reg_bytes = 0
        for t, name in params:
            q = ""
            # At most two register arg-bytes (A, then X), and never a
            # pointer.
            if (i in reg_ok and t != "int *" and reg_bytes + SIZE[t] <= 2
                    and rng.random() < 0.4):
                q = "register "
                reg_bytes += SIZE[t]
            decl.append(f"{q}{t} {name}")
        body = []
        vars_ = [name for t, name in params if t != "int *"] + globals_
        for k in range(rng.randint(1, 4)):
            body.append(f"    {rng.choice(TYPES)} l{k} = {_expr(rng, vars_)};")
            vars_.append(f"l{k}")
        if i in reg_ok and rng.random() < 0.3:
            body.append("    register unsigned char ry = (unsigned char)(l0);")
            vars_.append("ry")
        if any(t == "int *" for t, _ in params):
            body.append(f"    *pp += (int)({_expr(rng, vars_, 1)});")
            vars_.append("(*pp)")
        for j in calls[i]:
            cret, cparams = sigs[j]
            needs_box = any(t == "int *" for t, _ in cparams)
            if needs_box:
                body.append(f"    int box{j} = (int)(l0);")
            args = [f"&box{j}" if t == "int *" else _expr(rng, vars_, 1)
                    for t, _ in cparams]
            call = f"f{j}({', '.join(args)})"
            if use_ptr_fn and j == n - 1:
                ptype = f"{cret} (*fp)({', '.join(t for t, _ in cparams)})"
                body.append(f"    {ptype} = f{j};")
                call = f"fp({', '.join(args)})"
            dst = rng.choice([v for v in vars_ if v.startswith("l")])
            if j in carry and crng.random() < 0.5:
                # Branch on the carry flag instead of reading a value.
                body.append(f"    if ({call}) {dst} += 3; else {dst} ^= 5;")
            elif rng.random() < 0.3:
                body.append(f"    for (int it = 0; it < 3; it++) "
                            f"{dst} += ({call}) + it;")
            else:
                body.append(f"    {dst} ^= {call};")
            if needs_box:
                body.append(f"    {dst} += box{j};")
            if globals_ and rng.random() < 0.5:
                body.append(f"    {rng.choice(globals_)} += {dst};")
        if calls_rec[i]:
            dst = rng.choice([v for v in vars_ if v.startswith("l")])
            body.append(f"    {dst} += rec(3, (int)({_expr(rng, vars_, 1)}));")
        body.append(f"    return {_expr(rng, vars_)};")
        attr = ("__attribute__((return_bool_in_carry)) "
                if i in carry else "")
        ret = sigs[i][0]
        lines.append(f"{attr}{ret} f{i}({', '.join(decl) or 'void'}) {{")
        lines.extend(body)
        lines.append("}")

    _, params0 = sigs[0]
    pre = []
    margs = []
    for t, _ in params0:
        if t == "int *":
            pre.append("    int mbox = 5;")
            margs.append("&mbox")
        else:
            margs.append(str(rng.randint(0, 300)))
    mix = " ^ ".join(f"(long)g{k}" for k in range(len(globals_)))
    main = ["int main(void) {"] + pre + [
        f"    long r = (long)f0({', '.join(margs)});",
        f"    r ^= {mix};",
        "    return (int)(r ^ (r >> 16));",
        "}",
    ]
    return "\n".join(out + lines + main) + "\n", None


if __name__ == "__main__":
    sys.exit(fuzz_main(make_program, "calls", sys.argv[1:]))
