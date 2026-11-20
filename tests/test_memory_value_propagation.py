"""Unit tests for `passes.memory_value_propagation.apply_memory_value_propagation`.

Milestone 1 coverage: the CFG-aware DPTR-stage rewrite. Verifies
that an indirect-via-DPTR access whose DPTR-stage is in a
predecessor block (across one or more `Label` boundaries, plus
loop back-edges) gets rewritten to use the source ZP pair directly.
"""

import unittest

import asm_ast
from passes.memory_value_propagation import (
    apply_memory_value_propagation,
)


_DPTR = 0x24
_P0 = 0x80
_P1 = 0x81


def _A():
    return asm_ast.Reg(reg=asm_ast.A())


def _X():
    return asm_ast.Reg(reg=asm_ast.X())


def _Y():
    return asm_ast.Reg(reg=asm_ast.Y())


def _zp(addr, off=0):
    return asm_ast.ZP(address=addr, offset=off)


def _dptr_byte(off):
    return asm_ast.Data(name="DPTR", offset=off)


def _stage_dptr_from_zp_pair(p_lo, p_hi):
    """Build the 4-instruction DPTR-staging sequence:
    LDA p_lo; STA DPTR; LDA p_hi; STA DPTR+1."""
    return [
        asm_ast.Mov(src=_zp(p_lo), dst=_A(), is_volatile=False),
        asm_ast.Mov(src=_A(), dst=_dptr_byte(0), is_volatile=False),
        asm_ast.Mov(src=_zp(p_hi), dst=_A(), is_volatile=False),
        asm_ast.Mov(src=_A(), dst=_dptr_byte(1), is_volatile=False),
    ]


def _wrap(instrs):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    prog = asm_ast.Program(top_level=[fn])
    return apply_memory_value_propagation(prog)


def _instrs(prog):
    return prog.top_level[0].instructions


class TestSameBlockRewrite(unittest.TestCase):
    """Sanity: when the stage and the use sit in the same block,
    the rewrite happens (mirrors apply_indirect_base_prop's
    coverage)."""

    def test_indirecty_in_same_block_rewrites(self):
        instrs = _stage_dptr_from_zp_pair(_P0, _P1) + [
            asm_ast.Mov(
                src=_zp(0x10), dst=_A(), is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        # The final Mov's dst should be IndirectZpY(_P0).
        self.assertIsInstance(out[-1].dst, asm_ast.IndirectZpY)
        self.assertEqual(out[-1].dst.address, _P0)


class TestCrossBlockRewrite(unittest.TestCase):
    """The headline case: stage in preheader, use after a label
    boundary (loop entry)."""

    def test_use_after_label_rewrites(self):
        # Stage DPTR in preheader, use across a label boundary.
        instrs = _stage_dptr_from_zp_pair(_P0, _P1) + [
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(
                src=_zp(0x10), dst=_A(), is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
            asm_ast.Return(save_a=False),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        # Find the IndirectY-shaped Mov post-label.
        last = next(
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, (asm_ast.IndirectY, asm_ast.IndirectZpY))
        )
        self.assertIsInstance(last.dst, asm_ast.IndirectZpY)
        self.assertEqual(last.dst.address, _P0)

    def test_use_on_loop_exit_path_rewrites(self):
        # find_active_entity's shape: stage DPTR in preheader, loop
        # scans for a match, the indirect-Y write happens on the
        # exit path (not the back-edge). The fact survives the loop
        # because the back-edge doesn't include the indirect write.
        instrs = _stage_dptr_from_zp_pair(_P0, _P1) + [
            asm_ast.Mov(
                src=_zp(0x20), dst=_X(), is_volatile=False,
            ),
            asm_ast.Label(name=".loop_start"),
            asm_ast.Compare(left=_X(), right=asm_ast.Imm(value=0)),
            asm_ast.Branch(cond=asm_ast.MI(), target=".loop_end"),
            # "Match found" path branches out to the indirect-write.
            asm_ast.Mov(
                src=asm_ast.IndexedData(
                    name="arr", offset=0, index=asm_ast.X(),
                ),
                dst=_A(), is_volatile=False,
            ),
            asm_ast.Compare(left=_A(), right=asm_ast.Imm(value=0)),
            asm_ast.Branch(cond=asm_ast.NE(), target=".write_and_exit"),
            # No-match path: DEX, JMP back.
            asm_ast.Dec(dst=_X()),
            asm_ast.Jump(target=".loop_start"),
            asm_ast.Label(name=".write_and_exit"),
            asm_ast.Mov(
                src=_zp(0x21), dst=_A(), is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".loop_end"),
            asm_ast.Return(save_a=False),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        # The indirect-Y on the exit path should be rewritten.
        indirect_uses = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, (asm_ast.IndirectY, asm_ast.IndirectZpY))
        ]
        self.assertEqual(len(indirect_uses), 1)
        self.assertIsInstance(indirect_uses[0].dst, asm_ast.IndirectZpY)
        self.assertEqual(indirect_uses[0].dst.address, _P0)


class TestInvalidation(unittest.TestCase):
    """Writes that invalidate the equivalence must prevent the
    rewrite."""

    def test_volatile_source_not_read_again(self):
        # `k = KBD` with KBD volatile. A later read of k must stay a
        # read of k, since a second read of KBD can return a
        # different value.
        kbd = asm_ast.Data(name="KBD", offset=0)
        instrs = [
            asm_ast.Mov(src=kbd, dst=_zp(_P0), is_volatile=True),
            asm_ast.Mov(src=_zp(_P0), dst=_A(), is_volatile=False),
            asm_ast.Return(save_a=True),
        ]
        out = _instrs(_wrap(instrs))
        self.assertEqual(out[1].src, _zp(_P0))

    def test_write_to_dptr_kills_equivalence(self):
        # Stage, then overwrite DPTR low byte, then use — should NOT
        # rewrite (DPTR no longer matches the staged pair).
        instrs = _stage_dptr_from_zp_pair(_P0, _P1) + [
            asm_ast.Mov(
                src=asm_ast.Imm(value=0x99), dst=_A(),
                is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=_dptr_byte(0),
                is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        last = out[-1]
        # DPTR low byte was overwritten — should still be IndirectY,
        # NOT IndirectZpY.
        self.assertIsInstance(last.dst, asm_ast.IndirectY)

    def test_write_to_source_kills_equivalence(self):
        # Stage, then overwrite the SOURCE pair's byte, then use —
        # should NOT rewrite (DPTR still has the OLD source value,
        # not the source's current value).
        instrs = _stage_dptr_from_zp_pair(_P0, _P1) + [
            asm_ast.Mov(
                src=asm_ast.Imm(value=0x77), dst=_A(),
                is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=_zp(_P0),
                is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        last = out[-1]
        self.assertIsInstance(last.dst, asm_ast.IndirectY)

    def test_call_invalidates(self):
        # Stage, then Call, then use — Call clobbers everything,
        # so should NOT rewrite.
        instrs = _stage_dptr_from_zp_pair(_P0, _P1) + [
            asm_ast.Call(name="callee"),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        last = out[-1]
        self.assertIsInstance(last.dst, asm_ast.IndirectY)


class TestMeetAcrossBranches(unittest.TestCase):
    """At a join, only facts agreed by every predecessor survive."""

    def test_one_predecessor_doesnt_stage_kills_fact(self):
        # Block A stages DPTR from (P0, P1). Block B (other
        # predecessor of the join) does NOT stage. At the join,
        # DPTR is not known to equal anything. The use after the
        # join should NOT rewrite.
        instrs = [
            # Entry: branch on something.
            asm_ast.Mov(
                src=_zp(0x10), dst=_A(), is_volatile=False,
            ),
            asm_ast.Compare(left=_A(), right=asm_ast.Imm(value=0)),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".other"),
            # Fall-through: stage DPTR.
            *_stage_dptr_from_zp_pair(_P0, _P1),
            asm_ast.Jump(target=".join"),
            asm_ast.Label(name=".other"),
            # Branch arm: don't stage; just clobber A.
            asm_ast.Mov(
                src=asm_ast.Imm(value=0x55), dst=_A(),
                is_volatile=False,
            ),
            asm_ast.Label(name=".join"),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
            asm_ast.Return(save_a=False),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        # Find the IndirectY use after the .join label.
        join_idx = next(
            i for i, ins in enumerate(out)
            if isinstance(ins, asm_ast.Label) and ins.name == ".join"
        )
        post_join = out[join_idx:]
        indirect_uses = [
            i for i in post_join
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, (asm_ast.IndirectY, asm_ast.IndirectZpY))
        ]
        self.assertEqual(len(indirect_uses), 1)
        self.assertIsInstance(indirect_uses[0].dst, asm_ast.IndirectY,
                              "meet should drop the fact when one "
                              "predecessor doesn't have it")



class TestYSetupInvalidates(unittest.TestCase):
    """`floor_enemy_draw` regression: a cell holding `arr,Y` was read
    back as `LDA arr,Y` after a `(zp),Y` access had loaded Y with 0.
    An operand lowered with an `LDY #off` setup writes Y."""

    def _program(self, middle):
        return [
            asm_ast.Mov(src=asm_ast.IndexedData(
                name="arr", offset=0, index=asm_ast.Y(),
            ), dst=_A(), is_volatile=False),
            asm_ast.Mov(src=_A(), dst=_zp(0x84), is_volatile=False),
            middle,
            asm_ast.Mov(src=_zp(0x84), dst=_A(), is_volatile=False),
            asm_ast.Return(save_a=True, save_x=False),
        ]

    def test_indirect_zp_setup_kills_fact(self):
        middle = asm_ast.Mov(
            src=asm_ast.IndirectZp(address=0x90, offset=0), dst=_A(),
            is_volatile=False,
        )
        out = _instrs(_wrap(self._program(middle)))
        self.assertEqual(out[3].src, _zp(0x84))

    def test_frame_setup_kills_fact(self):
        middle = asm_ast.Mov(
            src=asm_ast.Frame(offset=2), dst=_A(), is_volatile=False,
        )
        out = _instrs(_wrap(self._program(middle)))
        self.assertEqual(out[3].src, _zp(0x84))


def _data(name, off=0):
    return asm_ast.Data(name=name, offset=off)


# A fixed zero-page object, a slot of this function, and a parameter
# slot of another, as a unit's symbol table gives them.
_SLOT_ADDRS = {"OPS": 0x82, "__local_f__t": 0x90, "__zpabi_g__p": 0x92}


def _run_slots(instrs):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    return _instrs(apply_memory_value_propagation(
        asm_ast.Program(top_level=[fn]), zp_symbol_addrs=_SLOT_ADDRS,
    ))


def _load(src):
    return asm_ast.Mov(src=src, dst=_A(), is_volatile=False)


def _store(dst):
    return asm_ast.Mov(src=_A(), dst=dst, is_volatile=False)


class TestSlotReadsFixedSource(unittest.TestCase):
    """A read of a `__local_*` slot that holds a copy of a zero-page
    byte the linker doesn't move reads that byte, when every read the
    slot's stores reach can, so the store can go."""

    def _copy_then_add(self, src, tail):
        # LDA src; STA t; LDA #1; CLC; ADC t; STA OPS+1; <tail>
        return [
            _load(src), _store(_data("__local_f__t")),
            _load(asm_ast.Imm(value=1)), asm_ast.ClearCarry(),
            asm_ast.Add(src=_data("__local_f__t"), dst=_A()),
            _store(_data("OPS", 1)), tail,
        ]

    def test_read_of_fixed_source(self):
        out = _run_slots(self._copy_then_add(
            _data("OPS"), asm_ast.Return(),
        ))
        self.assertEqual(out[4].src, _data("OPS"))

    def test_read_after_tail_call_function(self):
        # A function that ends in a JMP to another function is
        # rewritten too.
        out = _run_slots(self._copy_then_add(
            _data("OPS"), asm_ast.Jump(target="g"),
        ))
        self.assertEqual(out[4].src, _data("OPS"))

    def test_copy_of_slot_left_alone(self):
        # Another function's parameter slot may get another address
        # when linked, so a copy of it isn't read through it.
        out = _run_slots(self._copy_then_add(
            _data("__zpabi_g__p"), asm_ast.Return(),
        ))
        self.assertEqual(out[4].src, _data("__local_f__t"))

    def test_web_with_other_store_left_alone(self):
        # t is OPS on one path and 5 on the other. The read after the
        # join can't be rewritten, so the read on the first path
        # isn't either, since its store stays for the join.
        out = _run_slots([
            asm_ast.Branch(cond=asm_ast.EQ(), target=".b"),
            _load(_data("OPS")), _store(_data("__local_f__t")),
            asm_ast.Add(src=_data("__local_f__t"), dst=_A()),
            asm_ast.Jump(target=".join"),
            asm_ast.Label(name=".b"),
            _load(asm_ast.Imm(value=5)), _store(_data("__local_f__t")),
            asm_ast.Label(name=".join"),
            _load(_data("__local_f__t")),
            asm_ast.Return(save_a=True),
        ])
        self.assertEqual(out[3].src, _data("__local_f__t"))
        self.assertEqual(out[9].src, _data("__local_f__t"))


class TestDearerReads(unittest.TestCase):
    """A read of a zero-page slot that holds a copy of `arr,X` reads
    `arr,X` (3 bytes for the slot's 2) only when every read the slot's
    stores reach is rewritten, so the store goes, and at most two of
    them are (`_dearer_read`)."""

    _T = _data("__local_f__t")

    def _arr(self):
        return asm_ast.IndexedData(name="arr", offset=0, index=asm_ast.X())

    def _run(self, reads):
        instrs = [_load(self._arr()), _store(self._T)]
        for k, after in enumerate(reads):
            instrs += [_load(asm_ast.Imm(value=k)),
                       _store(_data("OPS")), *after,
                       _load(self._T), _store(_data("OPS", 1))]
        return _run_slots(instrs + [asm_ast.Return()])

    def _reads_of_t(self, out):
        return sum(1 for i in out if isinstance(i, asm_ast.Mov)
                   and i.src == self._T)

    def test_one_read_rewritten(self):
        out = self._run([[]])
        self.assertEqual(self._reads_of_t(out), 0)

    def test_two_reads_rewritten(self):
        out = self._run([[], []])
        self.assertEqual(self._reads_of_t(out), 0)

    def test_three_reads_left_alone(self):
        out = self._run([[], [], []])
        self.assertEqual(self._reads_of_t(out), 3)

    def test_read_that_stays_keeps_the_others(self):
        # After `LDX #5`, `arr,X` names another byte, so the second
        # read stays, and the store with it, and the first read
        # stays too.
        ldx = asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_X(), is_volatile=False)
        out = self._run([[], [ldx]])
        self.assertEqual(self._reads_of_t(out), 2)


class TestCopiedPointer(unittest.TestCase):
    """A `(zp),Y` access through a slot pair that holds a copy of a
    named zero-page pointer goes through that pointer, which is what
    a pointer returned in `F@ret` (passes/return_in_zero_page.py)
    leaves at each call."""

    _ADDRS = {"RET": 0x84, "__local_f__p": 0x94, "__zpabi_g__q": 0x96}

    def _run(self, src, between=()):
        # LDA src; STA p; LDA src+1; STA p+1; <between>; LDA (p),Y; RTS
        fn = asm_ast.Function(name="f", is_global=True, params=[], instructions=[
            _load(_data(src)), _store(_data("__local_f__p")),
            _load(_data(src, 1)), _store(_data("__local_f__p", 1)),
            *between,
            _load(asm_ast.IndirectZpY(address=0x94)),
            asm_ast.Return(save_a=True),
        ])
        return _instrs(apply_memory_value_propagation(
            asm_ast.Program(top_level=[fn]), zp_symbol_addrs=self._ADDRS,
        ))

    def test_read_through_copy_of_named_pointer(self):
        out = self._run("RET")
        self.assertEqual(out[4].src, asm_ast.IndirectZpY(address=0x84))

    def test_call_in_between_keeps_the_copy(self):
        # The call may write RET again.
        out = self._run("RET", [asm_ast.Call(name="g")])
        self.assertEqual(out[5].src, asm_ast.IndirectZpY(address=0x94))

    def test_store_through_a_pointer_keeps_the_copy(self):
        # A store through a pointer may write RET's bytes.
        out = self._run("RET", [_store(asm_ast.IndirectZpY(address=0x96))])
        self.assertEqual(out[5].src, asm_ast.IndirectZpY(address=0x94))

    def test_copy_of_another_functions_slot_left_alone(self):
        # The linker may give `__zpabi_g__q` another address.
        out = self._run("__zpabi_g__q")
        self.assertEqual(out[4].src, asm_ast.IndirectZpY(address=0x94))


class TestCallSummaries(unittest.TestCase):
    """A call to a function of the program whose writes are known kills
    only the facts about the bytes it writes, so a slot that holds a
    copy of `OPS` across the call still reads `OPS` after it."""

    _ADDRS = {"OPS": 0x82, "__local_f__t": 0x90, "OTHER": 0x84}

    def _run(self, callee_body, noreturn=frozenset()):
        # f: LDA OPS; STA t; JSR g; LDA #1; CLC; ADC t; STA OTHER+1; RTS
        f = asm_ast.Function(name="f", is_global=True, params=[], instructions=[
            _load(_data("OPS")), _store(_data("__local_f__t")),
            asm_ast.Call(name="g"),
            _load(asm_ast.Imm(value=1)), asm_ast.ClearCarry(),
            asm_ast.Add(src=_data("__local_f__t"), dst=_A()),
            _store(_data("OTHER", 1)), asm_ast.Return(),
        ])
        g = asm_ast.Function(name="g", is_global=False, params=[],
                             instructions=[*callee_body, asm_ast.Return()])
        out = apply_memory_value_propagation(
            asm_ast.Program(top_level=[f, g]), zp_symbol_addrs=self._ADDRS,
            noreturn=noreturn,
        )
        return out.top_level[0].instructions[5].src

    def test_callee_writing_other_bytes_keeps_the_fact(self):
        src = self._run([_load(asm_ast.Imm(value=0)), _store(_data("OTHER"))])
        self.assertEqual(src, _data("OPS"))

    def test_callee_writing_the_source_kills_it(self):
        src = self._run([_load(asm_ast.Imm(value=0)), _store(_data("OPS"))])
        self.assertEqual(src, _data("__local_f__t"))

    def test_callee_storing_through_a_pointer_kills_everything(self):
        src = self._run([_load(asm_ast.Imm(value=0)),
                         _store(asm_ast.IndirectZpY(address=0x84))])
        self.assertEqual(src, _data("__local_f__t"))

    def test_callee_calling_outside_the_program_kills_everything(self):
        self.assertEqual(self._run([asm_ast.Call(name="h")]),
                         _data("__local_f__t"))

    def test_call_that_never_returns_adds_nothing(self):
        # `h` stops the program, so nothing after `JSR h` runs.
        src = self._run([asm_ast.Call(name="h")], noreturn=frozenset({"h"}))
        self.assertEqual(src, _data("OPS"))


if __name__ == "__main__":
    unittest.main()
