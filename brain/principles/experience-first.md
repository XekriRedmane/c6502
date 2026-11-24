# Experience First

The people who use c6502 read the assembly it writes, the errors it reports, and the documents that explain both. Every technical decision serves or hinders that experience, and when a tradeoff sets implementation convenience against it, choose the experience.

- Every feature, flag, and attribute has to earn its place, since each one is one more thing for a user to learn.
- A program that breaks a rule gets an error that names the rule and says how to fix the program. The compiler doesn't fall back silently or miscompile.
- The generated assembly stays readable, with labels a reader can follow back to the source.
- A few features done well serve users better than many done roughly.

Foundations serve the experience. Foundational thinking governs the *sequence* of work, and this principle governs the *target*.
