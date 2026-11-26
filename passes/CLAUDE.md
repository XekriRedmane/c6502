# passes/CLAUDE.md

The compiler's passes are in C++, in cpp/src/passes/, and
[cpp/src/passes/CLAUDE.md](../cpp/src/passes/CLAUDE.md) describes them.
This directory keeps three Python modules that the simulator, the
tests, and errors.py still read. They stay until step 9 of the port
([docs/cpp_port.md](../docs/cpp_port.md)) moves the simulator, the
tests, and the fuzzers to C++.

- `runtime_layout.py` — where the runtime's zero-page block (SSP, FP,
  HARGS, and DPTR) goes. The simulator reads `RuntimeLayout` and
  `CLASSIC`, the classic layout (sim/harness.py, sim/linked.py,
  sim/runtime.py, and sim/ali_baba.py), and cpp_backend.py raises a
  compile's layout error as its `RuntimeLayoutError`
  (`_ERROR_MODULES`). `helper_footprints` gives each runtime helper's
  HARGS footprint, measured from rtlib/helpers.py, and
  tests/test_rtlib_table.py checks the footprints of the C++ table
  that tools/rtlib_table.py writes against it, as
  tests/test_runtime_library.py checks a linked program's HARGS size.
  tests/test_runtime_layout.py and tests/test_linker.py read
  `CLASSIC` and `RuntimeLayout`. The module also keeps
  `choose_layout`, `hargs_need`, and `check_hargs`, which nothing in
  Python calls now. The compiler runs their C++ ports
  (cpp/src/passes/runtime_layout.h and zp_globals.h).
- `dataclass_fields.py` — `fields_of` and `field_names`, which give
  what `dataclasses.fields` gives, computed once per class, since a
  dataclass's fields are fixed when the class is made.
  runtime_layout.py is the one module that uses them now.
- `type_check_error.py` — `TypeCheckError`, the error of the type
  checker, which errors.py re-exports. It has a module of its own so
  that runtime_layout.py can derive `RuntimeLayoutError` from it
  without importing errors.py.
