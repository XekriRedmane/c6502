"""Behavioral tests for `passes.optimization.pool`.

Coverage:
  - Default Pool() partitions [0x80, 0xFF] into two 64-byte halves.
  - Custom even starts produce balanced halves.
  - Validation rejects odd, negative, > 0xFF starts.
  - Range membership matches the closed-open interval definition.
"""

from __future__ import annotations

import unittest

from passes.optimization.pool import Pool


class TestDefaults(unittest.TestCase):
    def test_default_start(self) -> None:
        p = Pool()
        self.assertEqual(p.start, 0x80)
        self.assertEqual(p.mid, 0xC0)

    def test_default_caller_range(self) -> None:
        p = Pool()
        self.assertEqual(p.caller_saved(), range(0x80, 0xC0))
        self.assertEqual(len(p.caller_saved()), 64)

    def test_default_callee_range(self) -> None:
        p = Pool()
        self.assertEqual(p.callee_saved(), range(0xC0, 0x100))
        self.assertEqual(len(p.callee_saved()), 64)


class TestCustomStart(unittest.TestCase):
    def test_start_a0(self) -> None:
        p = Pool(start=0xA0)
        self.assertEqual(p.mid, 0xD0)
        self.assertEqual(p.caller_saved(), range(0xA0, 0xD0))
        self.assertEqual(p.callee_saved(), range(0xD0, 0x100))
        self.assertEqual(len(p.caller_saved()), 48)
        self.assertEqual(len(p.callee_saved()), 48)

    def test_start_c0(self) -> None:
        p = Pool(start=0xC0)
        self.assertEqual(p.mid, 0xE0)
        self.assertEqual(len(p.caller_saved()), 32)
        self.assertEqual(len(p.callee_saved()), 32)

    def test_start_zero(self) -> None:
        # Pathological but legal: start at 0 → caller [0, 0x80),
        # callee [0x80, 0x100). Doesn't conflict with the runtime's
        # actual reserved range — that's the user's problem.
        p = Pool(start=0x00)
        self.assertEqual(p.mid, 0x80)
        self.assertEqual(len(p.caller_saved()), 128)
        self.assertEqual(len(p.callee_saved()), 128)


class TestValidation(unittest.TestCase):
    def test_odd_start_rejected(self) -> None:
        with self.assertRaises(ValueError):
            Pool(start=0x81)

    def test_negative_start_rejected(self) -> None:
        with self.assertRaises(ValueError):
            Pool(start=-2)

    def test_overflow_start_rejected(self) -> None:
        with self.assertRaises(ValueError):
            Pool(start=0x100)


class TestRangeMembership(unittest.TestCase):
    def test_default_caller_membership(self) -> None:
        p = Pool()
        self.assertIn(0x80, p.caller_saved())
        self.assertIn(0xBF, p.caller_saved())
        self.assertNotIn(0xC0, p.caller_saved())
        self.assertNotIn(0x7F, p.caller_saved())

    def test_default_callee_membership(self) -> None:
        p = Pool()
        self.assertIn(0xC0, p.callee_saved())
        self.assertIn(0xFF, p.callee_saved())
        self.assertNotIn(0xBF, p.callee_saved())
        self.assertNotIn(0x100, p.callee_saved())
