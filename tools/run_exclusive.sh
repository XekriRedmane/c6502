#!/bin/bash
# Runs a command while holding the machine-wide lock /c/w/heavy.lock, so
# that only one build or test run uses the CPU at a time. A build already
# takes every core (MSVC's /MP and MSBuild's /m), so agents working in
# parallel run each configure, build, c6502_tests, check-golden,
# check-frozen, and fuzzing run through this script and take turns.
#
#   bash tools/run_exclusive.sh LABEL COMMAND [ARGS...]
#
# The lock is a directory, since mkdir is atomic. Its `owner` file holds
# the holder's PID, label, and start time. A lock whose holder is no
# longer running (a stopped task), or that has had no owner file for a
# minute (a holder killed between its mkdir and its write), is taken
# over, so a crash can't wedge the queue. RUN_EXCLUSIVE_LOCK names
# another lock directory, for a test. The command's exit status is this
# script's.
lock=${RUN_EXCLUSIVE_LOCK:-/c/w/heavy.lock}
label=$1
shift
mkdir -p "$(dirname "$lock")"
waited=0
while ! mkdir "$lock" 2>/dev/null; do
  holder=$(cut -d' ' -f1 "$lock/owner" 2>/dev/null)
  if [ -n "$holder" ] && ! kill -0 "$holder" 2>/dev/null; then
    echo "run_exclusive: taking over the lock of dead holder $(cat "$lock/owner")"
    rm -rf "$lock"
    continue
  fi
  if [ -z "$holder" ] && [ -n "$(find "$lock" -maxdepth 0 -mmin +1 2>/dev/null)" ]; then
    echo "run_exclusive: taking over a lock with no owner"
    rm -rf "$lock"
    continue
  fi
  sleep 15
  waited=$((waited + 15))
done
trap 'rm -rf "$lock"' EXIT
echo "$$ $label $(date +%H:%M:%S)" > "$lock/owner"
echo "run_exclusive: $label got the lock after ${waited}s of waiting"
"$@"
