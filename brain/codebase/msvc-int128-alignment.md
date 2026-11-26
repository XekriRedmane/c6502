# MSVC and the 16-byte alignment of asdl::Int

- `asdl::Int` is `std::_Signed128` under MSVC, which is aligned to 16 bytes, more than `alignof(max_align_t)`. Any IR value that can hold one, such as a `Type_operand` with its `Imm`, or a struct holding such a value, inherits that alignment.
- `std::stable_sort` on a vector of such a struct makes its temporary buffer with `std::aligned_storage`, and MSVC 14.36 stops the build with `error C2338: static_assert failed: 'You've instantiated std::aligned_storage<Len, Align> with an extended alignment ...'`. This happened on 26 Nov 2026 in place_zp_globals, after its map keyed by dump strings became a vector of structs holding the operand.
- Moving one element to the front with `std::find_if` and `std::rotate`, or sorting a vector of indexes, compiles. Sorting a vector of strings or other plainly aligned values is unaffected.
- The same compiler stopped with `fatal error C1001: Internal compiler error` on a doctest case that built a `tac_ast::Program` holding a Function, a Phi, and a PhiArg as one nested designated initializer (test_zp_globals.cpp, 26 Nov 2026). Building the value a step at a time, with `emplace_back` into each list, compiles.

Related: [[codebase/msbuild-stale-builds]]
