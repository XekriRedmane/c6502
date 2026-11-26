# Serialize Shared-State Mutations

When concurrent actors share mutable state, enforce serialization structurally, with sequential phases or exclusive ownership. Instructions and conventions are insufficient for concurrency safety.

**Why:** Concurrent writes to shared state produce failures that are intermittent, hard to reproduce, and expensive to debug.

**Pattern:**
1. **Identify shared mutable state**: build directories, worktrees, branches, the session's scratchpad, and files that two agents edit.
2. **If shared state exists, serialize access**, for example by running one port group at a time.
3. **If serialization is impractical, eliminate the sharing**: a fresh worktree and build directory for each run, and a subdirectory of your own in the scratchpad.

**Evidence:**
- A second copy of a check script built the same worktree in the same build directory, and MSBuild failed on locked `.obj` files ([[codebase/long-runs-and-scratch-scripts]]).
- A log left in the shared scratchpad by an earlier agent was read as a run of the current tree ([[codebase/long-runs-and-scratch-scripts]]).
- One build directory reused across checkouts made tests fail at commits that pass from a fresh build ([[codebase/msbuild-stale-builds]]).
- Port groups run in parallel conflicted in shared files and hit the API's rate limit, so they run one at a time (the main session's memory).
