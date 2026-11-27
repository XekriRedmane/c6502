// Small programs that each exercise one feature of the compiler and the
// simulator, its helper hooks among them, from tests/test_sim_smoke.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("sim_smoke sim: a constant return") {
  CHECK(run_c_program("int main(void) { return 42; }").return_int() == 42);
}

TEST_CASE("sim_smoke sim: a return of zero") {
  CHECK(run_c_program("int main(void) { return 0; }").return_int() == 0);
}

TEST_CASE("sim_smoke sim: a negative return") {
  // -1 is 0xFFFF, which is -1 read as signed.
  CHECK(run_c_program("int main(void) { return -1; }").return_int_signed() == -1);
}

TEST_CASE("sim_smoke sim: add") {
  CHECK(run_c_program("int main(void) { return 5 + 7; }").return_int() == 12);
}

TEST_CASE("sim_smoke sim: subtract") {
  CHECK(run_c_program("int main(void) { return 20 - 5; }").return_int() == 15);
}

TEST_CASE("sim_smoke sim: a multiply through the helper") {
  CHECK(run_c_program("int main(void) { return 6 * 7; }").return_int() == 42);
}

TEST_CASE("sim_smoke sim: an unsigned divide") {
  CHECK(run_c_program("int main(void) { return 100 / 7; }").return_int() == 14);
}

TEST_CASE("sim_smoke sim: an unsigned remainder") {
  CHECK(run_c_program("int main(void) { return 100 % 7; }").return_int() == 2);
}

TEST_CASE("sim_smoke sim: unary minus") {
  CHECK(run_c_program("int main(void) { int x = 7; return -x; }").return_int_signed() == -7);
}

TEST_CASE("sim_smoke sim: the bitwise operators") {
  CHECK(run_c_program("int main(void) { return 0x55 & 0x0F; }").return_int() == 0x05);
  CHECK(run_c_program("int main(void) { return 0x55 | 0xAA; }").return_int() == 0xFF);
  CHECK(run_c_program("int main(void) { return 0x55 ^ 0x0F; }").return_int() == 0x5A);
}

TEST_CASE("sim_smoke sim: shifts of an int") {
  CHECK(run_c_program("int main(void) { return 3 << 2; }").return_int() == 12);
  CHECK(run_c_program("int main(void) { return 100 >> 2; }").return_int() == 25);
}

TEST_CASE("sim_smoke sim: equality") {
  CHECK(run_c_program("int main(void) { return 5 == 5; }").return_int() == 1);
  CHECK(run_c_program("int main(void) { return 5 == 6; }").return_int() == 0);
}

TEST_CASE("sim_smoke sim: less than and greater than") {
  CHECK(run_c_program("int main(void) { return 3 < 5; }").return_int() == 1);
  CHECK(run_c_program("int main(void) { return 5 > 3; }").return_int() == 1);
}

TEST_CASE("sim_smoke sim: && doesn't evaluate its right side after a false left side") {
  // The right side would divide by zero.
  CHECK(run_c_program("int main(void) { return 0 && (1 / 0); }").return_int() == 0);
}

TEST_CASE("sim_smoke sim: || doesn't evaluate its right side after a true left side") {
  CHECK(run_c_program("int main(void) { return 1 || (1 / 0); }").return_int() == 1);
}

TEST_CASE("sim_smoke sim: if and else") {
  CHECK(run_c_program(R"c(
            int main(void) {
                int x = 10;
                if (x > 5) return 1;
                else return 0;
            }
        )c").return_int() == 1);
}

TEST_CASE("sim_smoke sim: a while loop") {
  CHECK(run_c_program(R"c(
            int main(void) {
                int s = 0; int i = 1;
                while (i <= 10) { s = s + i; i = i + 1; }
                return s;
            }
        )c").return_int() == 55);
}

TEST_CASE("sim_smoke sim: a for loop") {
  CHECK(run_c_program(R"c(
            int main(void) {
                int s = 0;
                for (int i = 1; i <= 10; i++) s += i;
                return s;
            }
        )c").return_int() == 55);
}

TEST_CASE("sim_smoke sim: break and continue") {
  // 1 + 2 + 3 + 4 + 6 + 7 = 23.
  CHECK(run_c_program(R"c(
            int main(void) {
                int s = 0;
                for (int i = 1; i <= 10; i++) {
                    if (i == 5) continue;
                    if (i == 8) break;
                    s += i;
                }
                return s;
            }
        )c").return_int() == 23);
}

TEST_CASE("sim_smoke sim: goto") {
  CHECK(run_c_program(R"c(
            int main(void) {
                int x = 0;
                goto end;
                x = 1;
              end:
                return x;
            }
        )c").return_int() == 0);
}

TEST_CASE("sim_smoke sim: a simple call") {
  CHECK(run_c_program(R"c(
            int add(int a, int b) { return a + b; }
            int main(void) { return add(3, 4); }
        )c").return_int() == 7);
}

TEST_CASE("sim_smoke sim: recursion") {
  CHECK(run_c_program(R"c(
            int fib(int n) {
                if (n < 2) return n;
                return fib(n - 1) + fib(n - 2);
            }
            int main(void) { return fib(10); }
        )c").return_int() == 55);
}

TEST_CASE("sim_smoke sim: SSP is back at $7FFF after main returns") {
  // The boot stub sets SSP to $7FFF before its JSR, so an epilogue that
  // doesn't rewind the soft stack shows here even when A is right.
  sim::Simulation s = build_sim("int main(void) { int x = 1; int y = 2; return x + y; }");
  const sim::SimResult r = s.run();
  CHECK(r.word(0x00) == 0x7FFF);
}

TEST_CASE("sim_smoke sim: a file-scope object") {
  CHECK(run_c_program(R"c(
            int counter = 100;
            int main(void) { counter = counter + 5; return counter; }
        )c").return_int() == 105);
}

TEST_CASE("sim_smoke sim: a write through a pointer") {
  CHECK(run_c_program(R"c(
            int main(void) {
                int x = 42;
                int *p = &x;
                *p = *p + 1;
                return x;
            }
        )c").return_int() == 43);
}

TEST_CASE("sim_smoke sim: array subscripts") {
  CHECK(run_c_program(R"c(
            int main(void) {
                int a[5] = {10, 20, 30, 40, 50};
                return a[0] + a[2] + a[4];
            }
        )c").return_int() == 90);
}

}  // namespace c6502::suite
