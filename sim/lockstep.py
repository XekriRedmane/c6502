"""Runs py65 and the C++ CPU (cpp/src/sim/cpu.h) in lockstep.

With `C6502_SIM_LOCKSTEP=1`, `Simulation.run` (sim/harness.py) runs the
program on py65 as it always does, and records each segment between hook
calls: the registers and the hook's writes it starts from, a digest of
the state after each instruction (`cpp_backend.cpu_run` gives the
layout), the bytes written, and why it ended. Then one `cpu-run` request
runs the same segments on the C++ CPU, from the same image, and the two
are compared. The first difference raises `LockstepError`, which gives
py65's and the C++ CPU's state after the first instruction they differ
on, found by replaying py65 from the start to it. The run's result is
py65's, so a program runs as it would without the mode.

With `C6502_SIM_LOCKSTEP_LOG` naming a directory too, each run appends
`<instructions> <segments> <how it ended>` to `lockstep-<pid>.txt` there.
"""
from __future__ import annotations

import array
import os
import struct
import zlib
from typing import NamedTuple

from sim.harness import hook_and_rts

_RECORD = struct.Struct("<HBBBBBQ").pack
_WRITE = struct.Struct("<HB").pack


class LockstepError(AssertionError):
    """py65 and the C++ CPU differ."""


def enabled() -> bool:
    return os.environ.get("C6502_SIM_LOCKSTEP") == "1"


class _RecordingMemory(bytearray):
    def __init__(self, data) -> None:
        super().__init__(data)
        self.log: list[tuple[int, int]] = []

    def __setitem__(self, key, value) -> None:
        bytearray.__setitem__(self, key, value)
        if isinstance(key, slice):
            for addr in range(*key.indices(len(self))):
                self.log.append((addr, bytearray.__getitem__(self, addr)))
        else:
            self.log.append((key % len(self), value))


def _state(mpu) -> dict:
    return {"pc": mpu.pc, "a": mpu.a, "x": mpu.x, "y": mpu.y, "sp": mpu.sp,
            "p": mpu.p, "cycles": mpu.processorCycles}


class _Found(NamedTuple):
    before: dict
    after: dict
    writes: list[tuple[int, int]]
    start_image: bytes


class _Trace:
    def __init__(self) -> None:
        self.segments: list[dict] = []
        self.digests: list[array.array] = []
        self.written: list[dict[int, int]] = []
        self.stops: list[str] = []
        self.ends: list[tuple] = []
        self.error: IndexError | None = None


def _drive(mpu, hooks, max_cycles: int, trace: _Trace) -> None:
    mem = mpu.memory
    log = mem.log
    patches: list[list[int]] = []
    while True:
        segment = {**_state(mpu), "patches": patches}
        digests = array.array("I")
        written: dict[int, int] = {}
        stop = None
        while True:
            if mpu.processorCycles >= max_cycles:
                stop = "cycles"
                break
            pc = mpu.pc
            if pc in hooks:
                stop = "stop"
                break
            if mem[pc] == 0x00:
                stop = "brk"
                break
            log.clear()
            try:
                mpu.step()
            except IndexError as e:
                trace.error = e
                stop = "error"
                break
            record = _RECORD(mpu.pc, mpu.a, mpu.x, mpu.y, mpu.sp, mpu.p,
                             mpu.processorCycles)
            if log:
                record += b"".join(_WRITE(a, v) for a, v in log)
                written.update(log)
            digests.append(zlib.crc32(record))
        trace.segments.append(segment)
        trace.digests.append(digests)
        trace.written.append(written)
        trace.stops.append(stop)
        trace.ends.append(tuple(_state(mpu).values()))
        if stop != "stop":
            return
        log.clear()
        hook_and_rts(mpu, hooks)
        patches = [[a, v] for a, v in log]


def _replay(mpu, hooks, segment_lengths: list[int], k: int, i: int) -> _Found:
    log = mpu.memory.log
    for length in segment_lengths[:k]:
        for _ in range(length):
            mpu.step()
        hook_and_rts(mpu, hooks)
    start_image = bytes(mpu.memory)
    for _ in range(i):
        mpu.step()
    before = _state(mpu)
    log.clear()
    mpu.step()
    return _Found(before, _state(mpu), list(log), start_image)


def _fmt(state: dict | tuple) -> str:
    v = tuple(state.values()) if isinstance(state, dict) else state
    return (f"PC=${v[0]:04X} A=${v[1]:02X} X=${v[2]:02X} Y=${v[3]:02X} "
            f"SP=${v[4]:02X} P=${v[5]:02X} cycles={v[6]}")


def _fmt_writes(writes) -> str:
    return " ".join(f"${a:04X}=${v:02X}" for a, v in writes) or "none"


def run(sim, max_cycles: int) -> bool:
    """Runs `sim` (a `Simulation`) to its BRK or `max_cycles` on py65
    and compares the C++ CPU with it. Returns whether the run timed out,
    and leaves py65's state in `sim.mpu`. Raises `LockstepError` at the
    first difference, and then what py65 or a hook raised."""
    mpu = sim.mpu
    hooks = sim.runtime.hooks
    original = mpu.memory
    image = bytes(original)
    start = _state(mpu)
    mem = _RecordingMemory(original)
    mpu.memory = mem
    trace = _Trace()
    try:
        _drive(mpu, hooks, max_cycles, trace)
    finally:
        original[:] = mem
        mpu.memory = original
        _check(trace, image, start, hooks, max_cycles)
    if trace.error is not None:
        raise trace.error
    return trace.stops[-1] == "cycles"


def _check(trace: _Trace, image: bytes, start: dict, hooks, max_cycles: int) -> None:
    import cpp_backend
    from py65.devices.mpu6502 import MPU

    results = cpp_backend.cpu_run(
        image, trace.segments, stops=hooks.keys(), stop_at_brk=True,
        max_cycles=max_cycles,
    )

    def replay(k: int, i: int) -> _Found:
        fresh = MPU()
        fresh.memory = _RecordingMemory(image)
        fresh.pc, fresh.a, fresh.x, fresh.y = start["pc"], start["a"], start["x"], start["y"]
        fresh.sp, fresh.p, fresh.processorCycles = start["sp"], start["p"], start["cycles"]
        return _replay(fresh, hooks, [len(d) for d in trace.digests], k, i)

    for k, want in enumerate(trace.digests):
        if k >= len(results):
            raise LockstepError(f"the C++ CPU ran {len(results)} segments, and py65 {len(trace.digests)}")
        got = results[k]
        if got["digests"] != want:
            n = min(len(got["digests"]), len(want))
            i = next((j for j in range(n) if got["digests"][j] != want[j]), n)
            if i == len(want):
                raise LockstepError(
                    f"segment {k}: py65 ended with {trace.stops[k]} after {len(want)} "
                    f"instructions at {_fmt(trace.ends[k])}, and the C++ CPU went on "
                    f"to {got['instructions']} and {got['stop']}")
            found = replay(k, i)
            seg = dict(trace.segments[k])
            seg["patches"] = []
            cpp = cpp_backend.cpu_run(
                found.start_image, [seg], stops=hooks.keys(), stop_at_brk=True,
                max_cycles=max_cycles, max_instructions=i + 1,
            )[0]
            raise LockstepError(
                f"segment {k}, instruction {i}, from {_fmt(found.before)}: py65 gives "
                f"{_fmt(found.after)} and writes {_fmt_writes(found.writes)}, and the C++ CPU "
                f"gives {_fmt(cpp['state'])} and writes "
                f"{_fmt_writes(cpp['last_writes']) if cpp['instructions'] == i + 1 else cpp['stop']}")
        if got["stop"] != trace.stops[k] or got["instructions"] != len(want):
            raise LockstepError(
                f"segment {k}: py65 ended with {trace.stops[k]} after {len(want)} "
                f"instructions, and the C++ CPU with {got['stop']} after {got['instructions']}")
        if got["stop"] != "error" and got["state"] != trace.ends[k]:
            raise LockstepError(
                f"segment {k} ends at {_fmt(trace.ends[k])} in py65 and at "
                f"{_fmt(got['state'])} in the C++ CPU")
        if got["writes"] != trace.written[k]:
            diff = sorted(set(got["writes"].items()) ^ set(trace.written[k].items()))
            raise LockstepError(f"segment {k} writes differ at {_fmt_writes(diff[:8])}")
    if len(results) != len(trace.digests):
        raise LockstepError(f"the C++ CPU ran {len(results)} segments, and py65 {len(trace.digests)}")

    log_dir = os.environ.get("C6502_SIM_LOCKSTEP_LOG")
    if log_dir and trace.stops:
        total = sum(len(d) for d in trace.digests)
        with open(os.path.join(log_dir, f"lockstep-{os.getpid()}.txt"), "a",
                  encoding="utf-8") as f:
            f.write(f"{total} {len(trace.digests)} {trace.stops[-1]}\n")


# A chunk of a machine's run goes to the C++ CPU in one request once it
# holds this many instructions or segments.
_CHUNK_INSTRUCTIONS = 2_000_000
_CHUNK_SEGMENTS = 20_000


class MachineRecorder:
    """Records a game machine's run on py65 (sim/apple2.py,
    lode_runner.py, and ali_baba.py) and runs it on the C++ CPU too. The
    machine calls `before` just before it executes an instruction and
    `after` just after. Whatever the machine changes in between, with its
    hooks, its soft switches, or a delay loop taken in one step, starts a
    new segment, whose patches are the bytes the machine wrote and whose
    registers are the machine's, and each segment runs on the C++ CPU for
    exactly as many instructions as it ran on py65. The run is sent in
    chunks, each from the memory py65 had when it began, and compared as
    `run` compares a harness run. `finish` sends the last chunk and puts
    the machine's memory back."""

    def __init__(self, mpu, label: str) -> None:
        self.mpu = mpu
        self.label = label
        self.original = mpu.memory
        self.mem = _RecordingMemory(self.original)
        self.log = self.mem.log
        mpu.memory = self.mem
        self.instructions = 0
        self.segment_count = 0
        self._start_chunk()

    def _start_chunk(self) -> None:
        self.image = bytes(self.mem)
        self.segments: list[dict] = []
        self.digests: list[array.array] = []
        self.written: list[dict[int, int]] = []
        self.ends: list[tuple] = []
        self.end: tuple | None = None
        self.chunk_instructions = 0
        self.log.clear()

    def _state(self) -> tuple:
        mpu = self.mpu
        return (mpu.pc, mpu.a, mpu.x, mpu.y, mpu.sp, mpu.p,
                mpu.processorCycles)

    def before(self) -> None:
        state = self._state()
        if self.log or state != self.end:
            pc, a, x, y, sp, p, cycles = state
            self.segments.append({
                "pc": pc, "a": a, "x": x, "y": y, "sp": sp, "p": p,
                "cycles": cycles,
                "patches": [[addr, v] for addr, v in self.log],
            })
            self.digests.append(array.array("I"))
            self.written.append({})
            self.ends.append(state)
            self.log.clear()

    def after(self) -> None:
        state = self._state()
        record = _RECORD(*state)
        log = self.log
        if log:
            record += b"".join(_WRITE(a, v) for a, v in log)
            self.written[-1].update(log)
            log.clear()
        self.digests[-1].append(zlib.crc32(record))
        self.ends[-1] = state
        self.end = state
        self.chunk_instructions += 1
        if (self.chunk_instructions >= _CHUNK_INSTRUCTIONS
                or len(self.segments) >= _CHUNK_SEGMENTS):
            self._flush()

    def _requests(self, segments: list[dict], counts: list[int]) -> list[dict]:
        return [dict(seg, instructions=n) for seg, n in zip(segments, counts)]

    def _flush(self) -> None:
        if self.segments:
            self._check()
            self.instructions += self.chunk_instructions
            self.segment_count += len(self.segments)
        self._start_chunk()

    def _check(self) -> None:
        import cpp_backend
        counts = [len(d) for d in self.digests]
        results = cpp_backend.cpu_run(
            self.image, self._requests(self.segments, counts), stops=[],
            stop_at_brk=False, max_cycles=2**64 - 1)
        if len(results) != len(self.segments):
            raise LockstepError(
                f"{self.label}: the C++ CPU ran {len(results)} segments of a "
                f"chunk, and py65 {len(self.segments)}")
        for k, got in enumerate(results):
            want = self.digests[k]
            if got["digests"] != want:
                n = min(len(got["digests"]), len(want))
                i = next((j for j in range(n) if got["digests"][j] != want[j]), n)
                self._report(k, i)
            if got["stop"] != "count" or got["instructions"] != counts[k]:
                raise LockstepError(
                    f"{self.label}: segment {k} of a chunk ran {counts[k]} "
                    f"instructions on py65, and the C++ CPU ended with "
                    f"{got['stop']} after {got['instructions']}")
            if got["state"] != self.ends[k]:
                raise LockstepError(
                    f"{self.label}: segment {k} of a chunk ends at "
                    f"{_fmt(self.ends[k])} in py65 and at {_fmt(got['state'])} "
                    "in the C++ CPU")
            if got["writes"] != self.written[k]:
                diff = sorted(set(got["writes"].items())
                              ^ set(self.written[k].items()))
                raise LockstepError(
                    f"{self.label}: segment {k} of a chunk writes differ at "
                    f"{_fmt_writes(diff[:8])}")

    def _report(self, k: int, i: int) -> None:
        """Raises `LockstepError` for instruction `i` of segment `k` of the
        chunk, with py65's state before and after it, found by replaying
        the chunk's segments on a fresh py65 from the chunk's image, and
        the C++ CPU's after it."""
        import cpp_backend
        from py65.devices.mpu6502 import MPU
        fresh = MPU()
        fresh.memory = _RecordingMemory(self.image)
        counts = [len(d) for d in self.digests[:k]] + [i]
        for seg, n in zip(self.segments[:k + 1], counts):
            fresh.pc, fresh.a, fresh.x, fresh.y = seg["pc"], seg["a"], seg["x"], seg["y"]
            fresh.sp, fresh.p, fresh.processorCycles = seg["sp"], seg["p"], seg["cycles"]
            for addr, v in seg["patches"]:
                fresh.memory[addr] = v
            for _ in range(n):
                fresh.step()
        before = _state(fresh)
        fresh.memory.log.clear()
        fresh.step()
        after = _state(fresh)
        cpp = cpp_backend.cpu_run(
            self.image, self._requests(self.segments[:k + 1], counts[:k] + [i + 1]),
            stops=[], stop_at_brk=False, max_cycles=2**64 - 1)[-1]
        raise LockstepError(
            f"{self.label}: after {self.instructions} instructions, chunk "
            f"segment {k}, instruction {i}, from {_fmt(before)}: py65 gives "
            f"{_fmt(after)} and writes {_fmt_writes(fresh.memory.log)}, and the "
            f"C++ CPU gives {_fmt(cpp['state'])} and writes "
            f"{_fmt_writes(cpp['last_writes'])}")

    def finish(self) -> None:
        """Compares the rest of the run and puts the machine's memory, as
        py65 left it, back in the object it started with."""
        try:
            self._flush()
        finally:
            self.original[:] = self.mem
            self.mpu.memory = self.original
        log_dir = os.environ.get("C6502_SIM_LOCKSTEP_LOG")
        if log_dir:
            with open(os.path.join(log_dir, f"lockstep-{os.getpid()}.txt"),
                      "a", encoding="utf-8") as f:
                f.write(f"{self.instructions} {self.segment_count} "
                        f"{self.label}\n")
