# Stale MSBuild builds

- One build directory reused across `git checkout`s of a series made `c6502_tests` fail or segfault on 21 Nov 2026 at commits that pass from a fresh build, even with CMake run again after each checkout. Give each commit a fresh worktree and build directory at a short path such as `/c/w/<first 7 characters of the hash>`, and treat a failure as unconfirmed until a fresh build reproduces it.
- A file edited while MSVC is building can keep the object compiled from its old text, and the link then fails with an unresolved symbol. Touch the edited files and build again.
- The source globs in cpp/CMakeLists.txt use CONFIGURE_DEPENDS, which the ZERO_CHECK target checks at the start of a full build. A .cpp added while a build runs is missing from that build. `cmake --build ... --target c6502_tests` skips ZERO_CHECK, so a new file under cpp/tests isn't compiled and its cases don't run. After adding a file, run `cmake -S cpp -B cpp/build`, and count the cases with `c6502_tests -tc="<pattern>"`.
- A wait loop that greps a build log for `error` matches `test_parser_errors.cpp` and ends early. Grep for `error C`, `error LNK`, or the last target's `->` line.
- Adding a language to `project()` in cpp/CMakeLists.txt makes MSBuild rebuild every target from scratch.
- A fresh MSVC Release build takes about 7 minutes with `/MP` (897f9a5f), against about 6 for GCC 12.2 in WSL with 16 jobs. Without `/MP`, cl.exe compiled each target's files one at a time and a build took about 23 minutes.

Related: [[codebase/worktree-guard]], [[codebase/long-runs-and-scratch-scripts]], [[principles/make-checks-show-their-coverage]], [[principles/fix-root-causes]]
