# Writing documents for c6502's users

- Say what a C programmer needs to know, in C's own terms. Robert rejected "so that every read and write in the source happens in the program" and asked for what a person would want to know: "A read of a soft switch like SPKR generally interacts with hardware, and usually its value doesn't matter and can be thrown away." Where the standard has a term, such as "undefined behavior", use it.
- Give the reason behind a rule. A rule stated with an unrelated sentence after it read as a non sequitur ("explain why a pointer can't be one").
- Pick example names that don't suggest a rule that isn't there. A `[[c6502::keep]]` example with a variable named after a register made the name look significant, so it became `i`.
- Keep a document brief and structured. Robert called a README with too much low-level detail "a wall of words".
- After inserting a section, reread the sections around it. A new section once landed in the middle of the zp_abi section.
- docs/attributes.md and docs/calling_conventions.md cover the same conventions from two sides, so a section of one links to the related section of the other.

Related: [[principles/experience-first]], [[principles/prove-it-works]]
