# Fix Root Causes

When debugging, never paper over symptoms. Trace every problem to its root cause and fix it there.

**Why:** Symptom fixes accumulate: each workaround makes the system harder to reason about, and the real bug remains. Root-cause fixes are slower upfront but reduce total debugging time.

**Pattern:**
- Reproduce first (if you can't reproduce it, you can't verify your fix).
- Ask "why" until you hit bedrock. The pass that a tool names isn't always the cause: five times the TAC interpreter's observer named a pass, and the interpreter was at fault ([[codebase/tac-interpreter]]).
- Resist the urge to add guards. A guard that silences a failure is a symptom fix.
- Check for the pattern, not just the instance. Six miscompiles came from passes that missed the Y and flag writes of an operand's lowering, and the fix was a shared helper in `asm_liveness` ([[codebase/asm-operand-side-effects]]).
- When stuck, instrument rather than guess: run the program by hand and read the actual error.
- When a pass doesn't fire, build the smallest IR that should trigger it in a doctest case and call the pass, or its predicate, directly. If that case fires, the difference between it and the real input is where the cause is. This found the opaque `LoadAddress` that kept dead `STA DPTR`s in `asm_dead_store` (cpp/src/passes/asm_dead_store.cpp explains the fix).

**Suspect State Before Code**
Code doesn't change between runs. State does. When a failure appears at one checkout and not another, suspect stale state first: a reused build directory made `c6502_tests` fail at commits that pass from a fresh build ([[codebase/msbuild-stale-builds]]). Treat a failure as unconfirmed until a fresh build reproduces it.
