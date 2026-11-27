# Porting tests with agents in parallel worktrees

- G1 of step 9 (26 Nov 2026) ported 495 Python tests to doctest with five agents at once, each in a worktree of its own (`isolation: "worktree"`), after the lead had committed the shared library and one ported module as the model. Each agent took about an hour, and together they ran in about 90 minutes, where one agent would have taken five hours or more.
- A worktree that `isolation` makes starts at main, not at the lead's branch. The agents were told to move to the lead's commit first. `git reset --hard <hash>` was refused by the permission classifier for one of them, and `git merge --ff-only <hash>` reached the same state, since the lead's commit descends from main.
- The brief was one file (C:/w/g1/brief.md) that every agent read, with a batch file listing each agent's modules and their tests, and a short prompt per agent with that batch's special cases. The agents were forbidden to edit the shared library, and each put its helpers in its own files, so the lead could merge them with `git cherry-pick <base>..<branch>` per agent. Over 95 commits, one conflict came up, in a module docstring that two batches edited.
- Each agent made a full build of its own worktree (about 9 minutes) and then incremental ones, all through tools/run_exclusive.sh. The five builds and test runs queued on the lock, and a lead's build waited up to 12 minutes for it.
- A static count of test methods and `setattr` shards matched pytest's count: 1,325 before, and 766 left after 495 ported and 64 dropped, as pytest reported. Count the methods at the base commit and at the end with an AST walk to get the tally by module.
- Agents ran the reflect skill on their own, before their reports, and committed notes to brain/ in their branches, which merged cleanly.

Related: [[codebase/worktree-guard]], [[codebase/msbuild-stale-builds]], [[codebase/rewriting-tests-with-scripts]], [[principles/cost-aware-delegation]]
