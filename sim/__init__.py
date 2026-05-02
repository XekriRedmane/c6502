"""6502 simulator harness for testing the TAC→asm pipeline end-to-end.

Public entry point: `sim.harness.run_c_program(source)` compiles a C
source string through the full pipeline, assembles to bytes via
`sim.assembler`, links a runtime stub (`sim.runtime`) that initializes
SSP and traps unimplemented helpers in Python, and runs the result on
py65's MPU until BRK or a cycle cap.
"""
