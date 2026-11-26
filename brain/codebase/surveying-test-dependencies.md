# Surveying what the tests take from the Python compiler

- A pytest plugin with `sys.setprofile` lists each call and import that code outside the compiler makes of a module group R removes (G9's was `g9_probe.py`, run with `-p` and its directory on PYTHONPATH). It made the suite take 33 minutes with `-n 12` against 6 without it, so run it once before the edits and once after.
- Under xdist the test ID that a `pytest_runtest_protocol` hookwrapper sets was wrong for a fifth of the records: calls from test_cpp_binutils.py came out under test_counter_to_index_reg.py's IDs. The caller's file in the frame was always right, so group a probe's records by the calling file, not by the test.
- An import shows up as a call of the module's `<module>` code from importlib's frames. Walk back past the frames whose file names importlib or starts with `<frozen` to find the importer.
- A static scan of the tests' `import` and `from` statements, `mock.patch` strings, and `import_module` calls finds what the profile can't, such as an import that runs only without the switch, and the profile finds calls through `sim/`, `fuzz/`, and cpp_backend.py that no import in a test shows.
- `mock.patch` used as a decorator, on a class or a method, leaves a `patchings` list on each test function, so a plugin can find the tests that `tests/no_inlining.py` decorates without a list of names. Check a decorator's effect by recording what it controls, and then run the same check with the flag forced the other way, which has to show the effect in every class.
- To run the suite at a commit while editing goes on, add a worktree at the commit and point `C6502_CPP` at a copy of the built c6502cpp. A commit with no C++ change needs no build of its own.

Related: [[codebase/rewriting-tests-with-scripts]], [[principles/make-checks-show-their-coverage]]
