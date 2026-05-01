"""End-to-end smoke tests for tac_sim.

Drives short C programs through the real pipeline (parse → resolve →
type-check → c99_to_tac) and runs the resulting TAC in `tac_sim`,
asserting the simulator's return value matches what C would produce.
This pins TAC behavior independently of the (still-landing) 6502
runtime helpers.
"""
from __future__ import annotations

import unittest

import fp_arith
from c99_to_tac import translate_program as translate_to_tac
from parser import parse
from passes.identifier_resolution import resolve_program as resolve_identifiers
from passes.label_resolution import resolve_program as resolve_labels
from passes.loop_labeling import label_program as label_loops
from passes.string_lifting import lift_program as lift_strings
from passes.type_checking import check_program as type_check_program
from tac_sim import Simulator


def _compile_to_tac(source: str):
    resolved = label_loops(resolve_labels(lift_strings(
        resolve_identifiers(parse(source)),
    )))
    prog, symbols, types = type_check_program(resolved)
    tac = translate_to_tac(prog, symbols, types)
    return tac, symbols, types


def _run(source: str, fn: str = "main", args: list[int] | None = None) -> int:
    tac, symbols, types = _compile_to_tac(source)
    sim = Simulator(tac, symbols, types)
    return sim.call(fn, args or [])


class TestTacSim(unittest.TestCase):
    def test_arithmetic(self):
        self.assertEqual(_run("int main(void) { return 1 + 2 * 3; }"), 7)

    def test_signed_div_truncates_toward_zero(self):
        # C99 §6.5.5.6: -7 / 2 == -3 (truncate, not floor).
        self.assertEqual(_run("int main(void) { return -7 / 2; }"), -3)
        self.assertEqual(_run("int main(void) { return -7 % 2; }"), -1)

    def test_long_overflow_wraps(self):
        # Long is 2 bytes signed, range -32768..32767. 30000 + 30000
        # overflows to -5536 in two's complement.
        self.assertEqual(
            _run("long main(void) { long a = 30000; return a + a; }"),
            -5536,
        )

    def test_unsigned_compare(self):
        # 0xFF as unsigned char would compare > 1; our 1-byte unsigned
        # is UInt. Make the comparison explicit.
        src = """
        int main(void) {
            unsigned int a = 200u;
            unsigned int b = 1u;
            return a > b;
        }
        """
        self.assertEqual(_run(src), 1)

    def test_signed_compare(self):
        # As `int` (signed 1-byte), 200 wraps to -56, which is < 1.
        src = """
        int main(void) {
            int a = 200;
            int b = 1;
            return a > b;
        }
        """
        self.assertEqual(_run(src), 0)

    def test_if_else(self):
        src = """
        int main(void) {
            long x = 5;
            if (x > 3) return 11; else return 22;
        }
        """
        self.assertEqual(_run(src), 11)

    def test_while_loop(self):
        src = """
        int main(void) {
            long sum = 0;
            long i = 0;
            while (i < 10) { sum = sum + i; i = i + 1; }
            return sum;
        }
        """
        self.assertEqual(_run(src), 45)

    def test_recursive_factorial(self):
        src = """
        long fact(long n) {
            if (n < 2) return 1;
            return n * fact(n - 1);
        }
        long main(void) { return fact(7); }
        """
        # 7! = 5040, fits in Long.
        self.assertEqual(_run(src), 5040)

    def test_call_with_args(self):
        src = """
        long add(long a, long b) { return a + b; }
        int main(void) { return add(40, 2); }
        """
        self.assertEqual(_run(src), 42)

    def test_continue_and_break(self):
        # Sum 1..10 skipping evens, stop at 7.
        src = """
        int main(void) {
            long sum = 0;
            long i = 0;
            while (i < 100) {
                i = i + 1;
                if (i > 7) break;
                if ((i & 1) == 0) continue;
                sum = sum + i;
            }
            return sum;
        }
        """
        # 1 + 3 + 5 + 7 = 16
        self.assertEqual(_run(src), 16)

    def test_signed_to_unsigned_widening(self):
        # int -1 -> SignExtend to 0xFFFF, reinterpreted as ULong is
        # 65535. main returns long (signed 2-byte), so 0xFFFF
        # reinterpreted as signed is -1.
        src = """
        long main(void) {
            int x = -1;
            unsigned long y = (unsigned long) x;
            return y;
        }
        """
        self.assertEqual(_run(src), -1)

    def test_truncate(self):
        # Long 4660 (0x1234) truncated to int = 0x34 = 52.
        src = """
        int main(void) {
            long x = 4660;
            return (int) x;
        }
        """
        self.assertEqual(_run(src), 0x34)


class TestTacSimMemory(unittest.TestCase):
    """Memory-resident locals, statics, pointers, arrays."""

    def test_address_of_local_then_deref(self):
        src = """
        int main(void) {
            int x = 7;
            int *p = &x;
            return *p;
        }
        """
        self.assertEqual(_run(src), 7)

    def test_pointer_store_writes_through(self):
        src = """
        int main(void) {
            int x = 1;
            int *p = &x;
            *p = 42;
            return x;
        }
        """
        self.assertEqual(_run(src), 42)

    def test_array_subscript_read(self):
        src = """
        long main(void) {
            long a[4];
            a[0] = 10; a[1] = 20; a[2] = 30; a[3] = 40;
            return a[2];
        }
        """
        self.assertEqual(_run(src), 30)

    def test_array_initializer_list(self):
        src = """
        long sum(void) {
            long a[5] = {1, 2, 3, 4, 5};
            long s = 0;
            long i = 0;
            while (i < 5) { s = s + a[i]; i = i + 1; }
            return s;
        }
        long main(void) { return sum(); }
        """
        self.assertEqual(_run(src), 15)

    def test_pointer_arithmetic(self):
        # ptr + i scales by sizeof(*ptr). Long is 2 bytes — verify
        # that p+1 lands on the next element, not the next byte.
        src = """
        long main(void) {
            long a[3] = {100, 200, 300};
            long *p = a;
            return *(p + 2);
        }
        """
        self.assertEqual(_run(src), 300)

    def test_file_scope_static_initialized(self):
        src = """
        int g = 99;
        int main(void) { return g; }
        """
        tac, symbols, types = _compile_to_tac(src)
        sim = Simulator(tac, symbols, types)
        self.assertEqual(sim.call("main", []), 99)
        self.assertEqual(sim.read_static("g"), 99)

    def test_file_scope_static_tentative_zero(self):
        src = """
        long g;
        long main(void) { return g; }
        """
        self.assertEqual(_run(src), 0)

    def test_static_persists_across_calls(self):
        src = """
        int counter = 0;
        int bump(void) { counter = counter + 1; return counter; }
        int main(void) { return 0; }
        """
        tac, symbols, types = _compile_to_tac(src)
        sim = Simulator(tac, symbols, types)
        self.assertEqual(sim.call("bump", []), 1)
        self.assertEqual(sim.call("bump", []), 2)
        self.assertEqual(sim.call("bump", []), 3)
        self.assertEqual(sim.read_static("counter"), 3)

    def test_block_scope_static_keeps_value(self):
        src = """
        int next(void) {
            static int n = 10;
            n = n + 1;
            return n;
        }
        int main(void) { return 0; }
        """
        tac, symbols, types = _compile_to_tac(src)
        sim = Simulator(tac, symbols, types)
        self.assertEqual(sim.call("next", []), 11)
        self.assertEqual(sim.call("next", []), 12)

    def test_pointer_to_static_via_addressinit(self):
        # `int *q = &g;` at file scope lays down an AddressInit
        # whose 2 bytes resolve to the address of `g`.
        src = """
        int g = 77;
        int *q = &g;
        int main(void) { return *q; }
        """
        self.assertEqual(_run(src), 77)

    def test_string_literal_subscript(self):
        # String literals get lifted to file-scope static char[]
        # by passes.string_lifting; subscripting reads the bytes.
        src = """
        int main(void) {
            char *s = "hello";
            return s[1];
        }
        """
        self.assertEqual(_run(src), ord("e"))

    def test_address_taken_param(self):
        # A param whose address is taken inside the body needs to
        # be allocated in memory at frame entry (not env), with
        # its argument value laid down as bytes.
        src = """
        long deref(long x) {
            long *p = &x;
            return *p + 1;
        }
        long main(void) { return deref(41); }
        """
        self.assertEqual(_run(src), 42)


def _f32(s: str) -> int:
    return fp_arith.single_string_to_bits(s)


def _f64(s: str) -> int:
    return fp_arith.double_string_to_bits(s)


class TestTacSimFP(unittest.TestCase):
    """Float and Double arithmetic / comparison / casts. Functions
    that return Float / Double come back as raw IEEE 754 bit
    patterns — tests compare those bits to the bit pattern of an
    expected decimal literal via fp_arith."""

    def test_float_add_exact(self):
        # 1.0 + 2.0 = 3.0 exactly in IEEE 754.
        src = "float main(void) { return 1.0f + 2.0f; }"
        self.assertEqual(_run(src), _f32("3.0"))

    def test_float_mul_half(self):
        # 3.0 * 0.5 = 1.5 exactly.
        src = "float main(void) { return 3.0f * 0.5f; }"
        self.assertEqual(_run(src), _f32("1.5"))

    def test_double_div(self):
        # 7.0 / 2.0 = 3.5 exactly.
        src = "double main(void) { return 7.0 / 2.0; }"
        self.assertEqual(_run(src), _f64("3.5"))

    def test_float_subtract(self):
        src = "float main(void) { return 5.5f - 2.25f; }"
        self.assertEqual(_run(src), _f32("3.25"))

    def test_float_negate(self):
        src = "float main(void) { float x = 1.5f; return -x; }"
        self.assertEqual(_run(src), _f32("-1.5"))

    def test_float_compare_lt(self):
        src = "int main(void) { float a = 1.5f; float b = 2.5f; return a < b; }"
        self.assertEqual(_run(src), 1)

    def test_float_compare_gt(self):
        src = "int main(void) { float a = 1.5f; float b = 2.5f; return a > b; }"
        self.assertEqual(_run(src), 0)

    def test_float_eq_zero_signs(self):
        # +0.0 == -0.0 by IEEE 754 §6.5.8.5.
        src = "int main(void) { float pz = 0.0f; float nz = -0.0f; return pz == nz; }"
        self.assertEqual(_run(src), 1)

    def test_int_to_double_cast(self):
        # (double)42 → 42.0
        src = "double main(void) { int x = 42; return (double)x; }"
        self.assertEqual(_run(src), _f64("42.0"))

    def test_double_to_int_truncates_toward_zero(self):
        # (int)3.7 → 3 (not 4 — truncates, not rounds).
        src = "int main(void) { double x = 3.7; return (int)x; }"
        self.assertEqual(_run(src), 3)
        # And -3.7 → -3 (toward zero, not -4).
        src2 = "int main(void) { double x = -3.7; return (int)x; }"
        self.assertEqual(_run(src2), -3)

    def test_float_to_double_widening(self):
        # Cast 1.5f to double — losslessly representable.
        src = "double main(void) { float x = 1.5f; return (double)x; }"
        self.assertEqual(_run(src), _f64("1.5"))

    def test_double_to_float_narrowing(self):
        # 1.5 narrows exactly. Use a value that's representable in
        # single precision so we get an exact bit match.
        src = "float main(void) { double x = 1.5; return (float)x; }"
        self.assertEqual(_run(src), _f32("1.5"))

    def test_unsigned_to_double(self):
        # 0xFFFFFFFF as ULongLong → 4294967295.0 exactly representable
        # in double (0x41EFFFFFFFE00000)... no wait, 4294967295 is not
        # exactly representable in double. Use a smaller value.
        src = """
        double main(void) {
            unsigned long x = 65535u;
            return (double)x;
        }
        """
        # 65535 is exactly representable.
        self.assertEqual(_run(src), _f64("65535.0"))

    def test_signed_negative_to_double(self):
        src = """
        double main(void) {
            int x = -1;
            return (double)x;
        }
        """
        self.assertEqual(_run(src), _f64("-1.0"))

    def test_float_truthiness_negative_zero_is_false(self):
        # IEEE 754 §6.3.1.2: -0.0 compares equal to 0, so it's falsy.
        # !x should be 1 here.
        src = "int main(void) { float x = -0.0f; return !x; }"
        self.assertEqual(_run(src), 1)

    def test_float_truthiness_nonzero_branches(self):
        # 1.5 is truthy, the if-true branch runs.
        src = """
        int main(void) {
            float x = 1.5f;
            if (x) return 11; else return 22;
        }
        """
        self.assertEqual(_run(src), 11)

    def test_static_float_persists(self):
        src = """
        float g = 2.5f;
        int main(void) { g = g + 0.5f; return 0; }
        """
        tac, symbols, types = _compile_to_tac(src)
        sim = Simulator(tac, symbols, types)
        sim.call("main", [])
        self.assertEqual(sim.read_static("g"), _f32("3.0"))

    def test_static_double_initializer(self):
        src = """
        double pi = 3.14;
        double main(void) { return pi; }
        """
        self.assertEqual(_run(src), _f64("3.14"))


class TestTacSimIndirectCall(unittest.TestCase):
    """Function pointers + IndirectCall. Uses explicit &fn since
    c6502 doesn't yet do bare-name function-to-pointer decay."""

    def test_call_through_pointer(self):
        src = """
        int twice(int x) { return x + x; }
        int main(void) {
            int (*fp)(int) = &twice;
            return fp(21);
        }
        """
        self.assertEqual(_run(src), 42)

    def test_pointer_dispatch_picks_target(self):
        src = """
        int add1(int x) { return x + 1; }
        int add100(int x) { return x + 100; }
        int main(void) {
            int (*fp)(int);
            fp = &add1;
            int a = fp(5);
            fp = &add100;
            int b = fp(5);
            return a + b;
        }
        """
        # 6 + 105 = 111. Out of int's signed 1-byte range — wraps
        # to 111 - 256 = -145, but 111 fits since int is -128..127
        # only at 1 byte... wait int is 1 byte signed -128..127.
        # 111 fits. But +1 produced 6, +100 produced 105; both OK.
        # Sum: 6 + 105 = 111, which fits in signed 1-byte. Final
        # return value: 111.
        self.assertEqual(_run(src), 111)

    def test_pointer_to_void_function(self):
        # Void-returning indirect call with a side effect on a
        # static variable.
        src = """
        int counter = 0;
        void bump(void) { counter = counter + 7; }
        int main(void) {
            void (*fp)(void) = &bump;
            fp();
            fp();
            return counter;
        }
        """
        self.assertEqual(_run(src), 14)

    def test_indirect_call_returning_long(self):
        src = """
        long big(long x) { return x * x; }
        long main(void) {
            long (*fp)(long) = &big;
            return fp(100);
        }
        """
        self.assertEqual(_run(src), 10000)

    def test_function_pointer_round_trip_via_static(self):
        # Pointer stored in a file-scope static, then used.
        src = """
        long square(long x) { return x * x; }
        long (*fp)(long) = &square;
        long main(void) { return fp(7); }
        """
        self.assertEqual(_run(src), 49)


class TestTacSimStructs(unittest.TestCase):
    """Struct / union pass-by-value, sret returns, member access."""

    def test_struct_sret_return(self):
        src = """
        struct point { int x; int y; };
        struct point make_point(void) {
            struct point p;
            p.x = 3;
            p.y = 4;
            return p;
        }
        int main(void) {
            struct point q = make_point();
            return q.x + q.y;
        }
        """
        self.assertEqual(_run(src), 7)

    def test_struct_pass_by_value(self):
        src = """
        struct point { int x; int y; };
        int sum_pt(struct point p) { return p.x + p.y; }
        int main(void) {
            struct point q;
            q.x = 10;
            q.y = 32;
            return sum_pt(q);
        }
        """
        self.assertEqual(_run(src), 42)

    def test_struct_pass_by_value_does_not_mutate_caller(self):
        # The callee receives its own copy — mutations don't escape.
        src = """
        struct box { long v; };
        long zero_it(struct box b) { b.v = 0; return b.v; }
        long main(void) {
            struct box k;
            k.v = 99;
            zero_it(k);
            return k.v;
        }
        """
        self.assertEqual(_run(src), 99)

    def test_struct_assignment_copies_bytes(self):
        src = """
        struct vec { long a; long b; long c; };
        long main(void) {
            struct vec u;
            u.a = 1; u.b = 2; u.c = 3;
            struct vec v;
            v = u;
            v.a = 100;
            return u.a + v.a + v.b + v.c;
        }
        """
        # u.a stays 1 (assignment was a copy), v.a became 100,
        # v.b == 2, v.c == 3. Total 1 + 100 + 2 + 3 = 106.
        self.assertEqual(_run(src), 106)

    def test_struct_with_pointer_member(self):
        src = """
        struct ref { int *p; };
        int main(void) {
            int x = 17;
            struct ref r;
            r.p = &x;
            *r.p = 99;
            return x;
        }
        """
        self.assertEqual(_run(src), 99)

    def test_nested_struct_member_access(self):
        src = """
        struct inner { int a; int b; };
        struct outer { struct inner i; int c; };
        int main(void) {
            struct outer o;
            o.i.a = 1;
            o.i.b = 2;
            o.c = 3;
            return o.i.a + o.i.b + o.c;
        }
        """
        self.assertEqual(_run(src), 6)

    def test_struct_returned_then_passed(self):
        # f() returns a struct; g(f()) passes that struct by value.
        # Tests that the return slot's bytes feed cleanly into the
        # next call's struct-arg copy.
        src = """
        struct pair { int x; int y; };
        struct pair make(int a, int b) {
            struct pair p;
            p.x = a;
            p.y = b;
            return p;
        }
        int sum(struct pair p) { return p.x + p.y; }
        int main(void) { return sum(make(20, 22)); }
        """
        self.assertEqual(_run(src), 42)

    def test_union_member_overlay(self):
        # Union members all sit at offset 0; writing one overwrites
        # the bytes of the other (within the shared width).
        src = """
        union u { int b; long w; };
        long main(void) {
            union u v;
            v.w = 0;
            v.b = 5;
            return v.w;
        }
        """
        # Writing 5 to the 1-byte int member then reading the 2-byte
        # long member: low byte is 5, high byte is whatever was
        # there (we wrote 0 first, so it's 0). Result: 5.
        self.assertEqual(_run(src), 5)


if __name__ == "__main__":
    unittest.main()
