# Checking an expression twice in the type checker

- The type checker stamps the program in place, and checking an expression a second time doesn't give the first result. The first check wraps an array operand in the `AddressOf` that stands for its decay (`decay_if_array`), and a second check reads that node as a real `&`, so the operand's type becomes a pointer to the whole array.
- `check_address_of` checked the operand of `&*e` and, when it wasn't a pointer, fell through to check all of `*e` again. `int a[2][3]; int *q = &*a[1];` was rejected as converting `int (*)[3]` to `int *` until 0161dc9e on 26 Nov 2026. A path that falls through to a general check after a partial one is the shape to look for.
- Comment Sicko's flags need checking against the pipeline before acting on them. It took the `Pseudo` case of `sub1_test_zero_peephole` for dead, since the peephole fixed point runs after `replace_pseudoregisters`, but `optimize_function` (optimization_asm/optimizer.cpp) runs the same pass before SSA construction, when every operand is still a `Pseudo`. Grep for every caller of a pass before calling a case of it unreachable.
- Several fixes that touch one file can still be separate commits. The scratch scripts of slice 5 staged chosen hunks with `git apply --cached` and a test file holding only some of its `TEST_CASE`s with `git hash-object -w` and `git update-index --cacheinfo`.

Related: [[codebase/comment-cleanup]], [[principles/prove-it-works]]
