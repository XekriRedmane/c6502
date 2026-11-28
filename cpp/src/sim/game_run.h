// `c6502cpp game-run`, which plays one of the games' scripts on its
// machine (sim/games.h) and writes the run as text, so that a run can be
// looked at on its own and compared with another run's text.
//
//     c6502cpp game-run GAME SCRIPT [--exe PATH] [--events N] [--ld PATH]
//                       [--steps-after EVENT --steps N] [-o FILE]
//
// GAME is zork, lode_runner, or ali_baba, and SCRIPT one of its scripts.
// Without --exe the original runs as its test runs it. With --exe the
// linked C program at PATH runs, for Lode Runner and Ali Baba until it
// has made N events, or as many as the original makes when --events
// isn't given. Ali Baba's program is loaded with the regions of the
// linker script at --ld cleared (examples/ali_baba/ali_baba.ld by
// default). The games' files come from ZORK_DSK, LODE_RUNNER_DIR, and
// ALI_BABA_DIR.
//
// `c6502cpp game-run ali_baba coverage` plays every script on the
// original and lists, for each, how many of the original's routines it
// executes in the spans the C program replaces (`kCodeSpans`), and then
// the routines of those spans that none of the scripts reach.
//
// The text has a line per event (a call, for Zork), with the CPU's
// state digest (sim/cpu_run.h's `digest`, with no writes) as the event
// was recorded, then the machine's state at the end. With --steps-after,
// it also has a `step` line with the registers and cycles after each of
// the N instructions that follow event EVENT.
#pragma once

#include <iosfwd>
#include <string>
#include <vector>

namespace c6502::sim::game_run {

// Runs the command with `args`, the words after `game-run`, writing the
// text to `out` (or the -o file) and any error to `err`, and returns the
// exit status: 0, 1 when the run fails, and 2 for a usage error.
int main(const std::vector<std::string>& args, std::ostream& out, std::ostream& err);

}  // namespace c6502::sim::game_run
