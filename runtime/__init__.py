"""The runtime library: the 6502 helpers that compiled code calls for the
operations it doesn't do inline, such as a division by a variable. The
linker adds the ones a program calls (`passes/runtime_library.py`), and
the simulator runs the same code (`sim/runtime.py`). See `helpers.py`.
"""
