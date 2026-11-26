"""The runtime library: the 6502 helpers that compiled code calls for the
operations it doesn't do inline, such as a division by a variable. The
linker adds the ones a program calls (cpp/src/passes/runtime_library.h,
from the copy in cpp/generated/rtlib_helpers.cpp), and
the simulator runs the same code (`sim/runtime.py`). See `helpers.py`.
"""
