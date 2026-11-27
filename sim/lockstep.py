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


def _hook_and_rts(mpu, hooks) -> None:
    hooks[mpu.pc](mpu.memory)
    sp = mpu.sp
    lo = mpu.memory[0x0100 + ((sp + 1) & 0xFF)]
    hi = mpu.memory[0x0100 + ((sp + 2) & 0xFF)]
    mpu.sp = (sp + 2) & 0xFF
    mpu.pc = (((hi << 8) | lo) + 1) & 0xFFFF


class _Trace:
    def __init__(self) -> None:
        self.segments: list[dict] = []
        self.digests: list[array.array] = []
        self.written: list[dict[int, int]] = []
        self.stops: list[str] = []
        self.ends: list[tuple] = []
        self.error: BaseException | None = None
        self.found: tuple | None = None


def _drive(mpu, hooks, max_cycles: int, until: tuple[int, int] | None = None) -> _Trace:
    mem = mpu.memory
    log = mem.log
    trace = _Trace()
    patches: list[list[int]] = []
    while True:
        k = len(trace.segments)
        trace.segments.append({**_state(mpu), "patches": patches})
        start_image = bytes(mem) if until is not None and until[0] == k else None
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
            before = _state(mpu)
            log.clear()
            try:
                mpu.step()
            except Exception as e:
                trace.error = e
                stop = "error"
                break
            record = _RECORD(mpu.pc, mpu.a, mpu.x, mpu.y, mpu.sp, mpu.p,
                             mpu.processorCycles)
            if log:
                record += b"".join(_WRITE(a, v) for a, v in log)
                written.update(log)
            digests.append(zlib.crc32(record))
            if until == (k, len(digests) - 1):
                trace.found = (before, _state(mpu), list(log), start_image)
                return trace
        trace.digests.append(digests)
        trace.written.append(written)
        trace.stops.append(stop)
        trace.ends.append(tuple(_state(mpu).values()))
        if stop != "stop":
            return trace
        log.clear()
        try:
            _hook_and_rts(mpu, hooks)
        except Exception as e:
            trace.error = e
            return trace
        patches = [[a, v] for a, v in log]


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
    import cpp_backend
    from py65.devices.mpu6502 import MPU

    mpu = sim.mpu
    hooks = sim.runtime.hooks
    original = mpu.memory
    image = bytes(original)
    start = _state(mpu)
    mem = _RecordingMemory(original)
    mpu.memory = mem
    try:
        trace = _drive(mpu, hooks, max_cycles)
    finally:
        original[:] = mem
        mpu.memory = original
    results = cpp_backend.cpu_run(
        image, trace.segments, stops=hooks.keys(), stop_at_brk=True,
        max_cycles=max_cycles,
    )

    def replay(k: int, i: int) -> tuple:
        fresh = MPU()
        fresh.memory = _RecordingMemory(image)
        fresh.pc, fresh.a, fresh.x, fresh.y = start["pc"], start["a"], start["x"], start["y"]
        fresh.sp, fresh.p, fresh.processorCycles = start["sp"], start["p"], start["cycles"]
        return _drive(fresh, hooks, max_cycles, until=(k, i)).found

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
            before, after, writes, seg_image = replay(k, i)
            seg = dict(trace.segments[k])
            seg["patches"] = []
            cpp = cpp_backend.cpu_run(
                seg_image, [seg], stops=hooks.keys(), stop_at_brk=True,
                max_cycles=max_cycles, max_instructions=i + 1,
            )[0]
            raise LockstepError(
                f"segment {k}, instruction {i}, from {_fmt(before)}: py65 gives "
                f"{_fmt(after)} and writes {_fmt_writes(writes)}, and the C++ CPU "
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
    if log_dir:
        total = sum(len(d) for d in trace.digests)
        with open(os.path.join(log_dir, f"lockstep-{os.getpid()}.txt"), "a",
                  encoding="utf-8") as f:
            f.write(f"{total} {len(trace.digests)} {trace.stops[-1]}\n")
    if trace.error is not None:
        raise trace.error
    return trace.stops[-1] == "cycles"
