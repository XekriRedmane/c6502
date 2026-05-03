import unittest

# asm_emit consumes asm2_ast (the strictly-atomic IR after
# `passes.asm_to_asm2` has lowered the asm_ast compound nodes).
# Tests construct asm2_ast nodes; the alias avoids touching
# every reference in this large test file.
import asm2_ast as asm_ast
from asm_emit import (
    emit_function,
    emit_instruction,
    emit_program,
)


def _reg(r):
    return asm_ast.Reg(reg=r)


_A = asm_ast.A()
_X = asm_ast.X()
_Y = asm_ast.Y()


def _prog(*instrs, name="main") -> asm_ast.Type_program:
    # asm.asdl is plural now: Program holds a list of Functions and
    # Function carries a (possibly empty) params list. Tests build
    # one-function programs through this helper, so the wrapping
    # stays out of every test body.
    return asm_ast.Program(top_level=[asm_ast.Function(
        name=name, is_global=True, params=[], instructions=list(instrs),
    )])


class TestEmitMov(unittest.TestCase):
    def test_imm_to_a_emits_lda(self):
        for v, expected in [(0, "#$00"), (1, "#$01"), (0x2A, "#$2A"),
                            (0xFF, "#$FF"), (10, "#$0A")]:
            with self.subTest(v=v):
                self.assertEqual(
                    emit_instruction(
                        asm_ast.Mov(src=asm_ast.Imm(value=v), dst=_reg(_A))
                    ),
                    [f"   LDA   {expected}"],
                )

    def test_imm_to_x_emits_ldx(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Imm(value=0x2A), dst=_reg(_X))
            ),
            ["   LDX   #$2A"],
        )

    def test_imm_to_y_emits_ldy(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Imm(value=0x2A), dst=_reg(_Y))
            ),
            ["   LDY   #$2A"],
        )

    def test_imm_out_of_range_raises(self):
        for v in [-1, 256, 1000, -100]:
            with self.subTest(v=v):
                with self.assertRaises(ValueError):
                    emit_instruction(
                        asm_ast.Mov(src=asm_ast.Imm(value=v), dst=_reg(_A))
                    )

    def test_x_to_a_emits_txa(self):
        self.assertEqual(
            emit_instruction(asm_ast.Mov(src=_reg(_X), dst=_reg(_A))),
            ["   TXA"],
        )

    def test_y_to_a_emits_tya(self):
        self.assertEqual(
            emit_instruction(asm_ast.Mov(src=_reg(_Y), dst=_reg(_A))),
            ["   TYA"],
        )

    def test_a_to_x_emits_tax(self):
        self.assertEqual(
            emit_instruction(asm_ast.Mov(src=_reg(_A), dst=_reg(_X))),
            ["   TAX"],
        )

    def test_a_to_y_emits_tay(self):
        self.assertEqual(
            emit_instruction(asm_ast.Mov(src=_reg(_A), dst=_reg(_Y))),
            ["   TAY"],
        )

    def test_unsupported_mov_combinations_raise(self):
        unsupported = [
            # No 6502 instruction for X<->Y direct transfer.
            asm_ast.Mov(src=_reg(_X), dst=_reg(_Y)),
            asm_ast.Mov(src=_reg(_Y), dst=_reg(_X)),
            # X/Y <-> Stack not handled (would clobber A); codegen must go
            # via A explicitly.
            asm_ast.Mov(src=_reg(_X), dst=asm_ast.Stack(offset=2)),
            asm_ast.Mov(src=asm_ast.Stack(offset=2), dst=_reg(_X)),
            # Imm cannot be a destination.
            asm_ast.Mov(src=_reg(_A), dst=asm_ast.Imm(value=0)),
        ]
        for instr in unsupported:
            with self.subTest(instr=instr):
                with self.assertRaises(ValueError):
                    emit_instruction(instr)

    def test_self_mov_drops(self):
        # Mov(src, dst) where src == dst is a no-op (writes the same
        # value back to the same location). Drop it. Catches Reg→Reg
        # self-transfers (which used to raise) and the more
        # important case of regalloc giving a Phi src and dst the
        # same color, leading to a no-op Copy after de-SSA.
        same_pairs = [
            asm_ast.Mov(src=_reg(_A), dst=_reg(_A)),
            asm_ast.Mov(src=_reg(_X), dst=_reg(_X)),
            asm_ast.Mov(src=_reg(_Y), dst=_reg(_Y)),
            asm_ast.Mov(
                src=asm_ast.ZP(address=0x82, offset=0),
                dst=asm_ast.ZP(address=0x82, offset=0),
            ),
            asm_ast.Mov(
                src=asm_ast.Frame(offset=1),
                dst=asm_ast.Frame(offset=1),
            ),
            asm_ast.Mov(
                src=asm_ast.Data(name="g", offset=0),
                dst=asm_ast.Data(name="g", offset=0),
            ),
        ]
        for instr in same_pairs:
            with self.subTest(instr=instr):
                self.assertEqual(emit_instruction(instr), [])


class TestEmitMovStack(unittest.TestCase):
    def test_imm_to_stack(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Imm(value=0x2A),
                            dst=asm_ast.Stack(offset=3))
            ),
            ["   LDA   #$2A", "   LDY   #$03", "   STA   (SSP),Y"],
        )

    def test_stack_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Stack(offset=5), dst=_reg(_A))
            ),
            ["   LDY   #$05", "   LDA   (SSP),Y"],
        )

    def test_a_to_stack(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=_reg(_A), dst=asm_ast.Stack(offset=7))
            ),
            ["   LDY   #$07", "   STA   (SSP),Y"],
        )

    def test_stack_to_stack(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Stack(offset=1),
                            dst=asm_ast.Stack(offset=4))
            ),
            [
                "   LDY   #$01",
                "   LDA   (SSP),Y",
                "   LDY   #$04",
                "   STA   (SSP),Y",
            ],
        )

    def test_stack_offset_out_of_range_raises(self):
        for off in [-1, 256, 1000]:
            with self.subTest(off=off):
                with self.assertRaises(ValueError):
                    emit_instruction(
                        asm_ast.Mov(src=_reg(_A), dst=asm_ast.Stack(offset=off))
                    )


class TestEmitMovFrame(unittest.TestCase):
    def test_imm_to_frame(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Imm(value=0x2A),
                            dst=asm_ast.Frame(offset=3))
            ),
            ["   LDA   #$2A", "   LDY   #$03", "   STA   (FP),Y"],
        )

    def test_frame_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Frame(offset=5), dst=_reg(_A))
            ),
            ["   LDY   #$05", "   LDA   (FP),Y"],
        )

    def test_a_to_frame(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=_reg(_A), dst=asm_ast.Frame(offset=7))
            ),
            ["   LDY   #$07", "   STA   (FP),Y"],
        )

    def test_frame_to_frame(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Frame(offset=1),
                            dst=asm_ast.Frame(offset=4))
            ),
            [
                "   LDY   #$01",
                "   LDA   (FP),Y",
                "   LDY   #$04",
                "   STA   (FP),Y",
            ],
        )

    def test_frame_offset_out_of_range_raises(self):
        for off in [-1, 256, 1000]:
            with self.subTest(off=off):
                with self.assertRaises(ValueError):
                    emit_instruction(
                        asm_ast.Mov(src=_reg(_A), dst=asm_ast.Frame(offset=off))
                    )


class TestEmitMovMixed(unittest.TestCase):
    """Stack and Frame can appear together in a single Mov; each side
    resolves through its own pointer."""

    def test_stack_to_frame(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Stack(offset=2),
                            dst=asm_ast.Frame(offset=3))
            ),
            [
                "   LDY   #$02",
                "   LDA   (SSP),Y",
                "   LDY   #$03",
                "   STA   (FP),Y",
            ],
        )

    def test_frame_to_stack(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Frame(offset=2),
                            dst=asm_ast.Stack(offset=3))
            ),
            [
                "   LDY   #$02",
                "   LDA   (FP),Y",
                "   LDY   #$03",
                "   STA   (SSP),Y",
            ],
        )


class TestEmitCall(unittest.TestCase):
    """Call(name) maps to a single JSR <name>. tac_to_asm emits Calls
    for runtime helpers (mul8 / divmod8 / asl8 / asr8 plus their
    16-bit variants) and for user-function calls."""

    def test_call_mul8(self):
        self.assertEqual(
            emit_instruction(asm_ast.Call(name="mul8")),
            ["   JSR   mul8"],
        )

    def test_call_divmod8(self):
        self.assertEqual(
            emit_instruction(asm_ast.Call(name="divmod8")),
            ["   JSR   divmod8"],
        )

    def test_call_arbitrary_name(self):
        self.assertEqual(
            emit_instruction(asm_ast.Call(name="my_fn")),
            ["   JSR   my_fn"],
        )


class TestEmitLabel(unittest.TestCase):
    def test_label_at_column_1(self):
        # Labels share column 1 with the function name, distinguishing
        # them from indented opcodes. No operand column.
        self.assertEqual(
            emit_instruction(asm_ast.Label(name="loop")),
            ["loop:"],
        )

    def test_label_arbitrary_name(self):
        self.assertEqual(
            emit_instruction(asm_ast.Label(name="L_then_42")),
            ["L_then_42:"],
        )


class TestEmitJump(unittest.TestCase):
    def test_jump_emits_jmp_with_target(self):
        self.assertEqual(
            emit_instruction(asm_ast.Jump(target="exit")),
            ["   JMP   exit"],
        )


class TestEmitBranch(unittest.TestCase):
    """Each Branch(cond, target) maps to its 6502 Bxx opcode. The
    assembler resolves the PC-relative displacement; emit just writes
    the symbolic target."""

    _CASES = [
        (asm_ast.CC(), "BCC"),
        (asm_ast.CS(), "BCS"),
        (asm_ast.EQ(), "BEQ"),
        (asm_ast.MI(), "BMI"),
        (asm_ast.NE(), "BNE"),
        (asm_ast.PL(), "BPL"),
        (asm_ast.VC(), "BVC"),
        (asm_ast.VS(), "BVS"),
    ]

    def test_each_condition(self):
        for cond, opcode in self._CASES:
            with self.subTest(cond=type(cond).__name__):
                self.assertEqual(
                    emit_instruction(
                        asm_ast.Branch(cond=cond, target="L_then")
                    ),
                    [f"   {opcode}   L_then"],
                )

    def test_unknown_condition_raises(self):
        stub = type("Stub", (asm_ast.Type_condition,), {})
        with self.assertRaises(TypeError):
            emit_instruction(asm_ast.Branch(cond=stub(), target="X"))


class TestEmitFunctionWithLabels(unittest.TestCase):
    """Labels are interleaved with indented opcodes inside a function;
    a typical lowering of `if` would be Branch -> body -> Label."""

    def test_branch_then_label_layout(self):
        fn = asm_ast.Function(name="main", is_global=True, instructions=[
            asm_ast.Branch(cond=asm_ast.NE(), target="L_skip"),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_reg(_A)),
            asm_ast.Label(name="L_skip"),
            asm_ast.Return(),
        ])
        self.assertEqual(
            emit_function(fn),
            [
                "main:",
                "   SUBROUTINE",
                "",
                "   BNE   L_skip",
                "   LDA   #$01",
                "L_skip:",
                "   RTS",
            ],
        )

    def test_jump_back_to_top(self):
        fn = asm_ast.Function(name="main", is_global=True, instructions=[
            asm_ast.Label(name="L_top"),
            asm_ast.Jump(target="L_top"),
        ])
        self.assertEqual(
            emit_function(fn),
            [
                "main:",
                "   SUBROUTINE",
                "",
                "L_top:",
                "   JMP   L_top",
            ],
        )


class TestEmitInstruction(unittest.TestCase):
    def test_unknown_instruction_raises(self):
        stub = type("Stub", (asm_ast.Type_instruction,), {})
        with self.assertRaises(TypeError):
            emit_instruction(stub())



class TestEmitFunction(unittest.TestCase):
    def test_label_subroutine_blank_then_instructions(self):
        fn = asm_ast.Function(name="main", is_global=True, instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_reg(_A)),
            asm_ast.Return(),
        ])
        self.assertEqual(
            emit_function(fn),
            [
                "main:",
                "   SUBROUTINE",
                "",
                "   LDA   #$00",
                "   RTS",
            ],
        )

    def test_empty_instructions_label_and_subroutine_only(self):
        fn = asm_ast.Function(name="main", is_global=True, instructions=[])
        self.assertEqual(emit_function(fn), ["main:", "   SUBROUTINE"])

    def test_comment_blank_section_markers(self):
        # The asm_to_asm2 lowering emits Comment / Blank atoms to
        # mark the prologue / epilogue regions. Verify emit_function
        # renders them at opcode column and as bare blank lines.
        fn = asm_ast.Function(name="main", is_global=True, instructions=[
            asm_ast.Comment(text="prologue: 0 arg bytes, 1 local bytes"),
            asm_ast.Mov(src=asm_ast.Imm(value=7),
                        dst=asm_ast.Frame(offset=1)),
            asm_ast.Blank(),
            asm_ast.Comment(text="epilogue"),
            asm_ast.Return(),
        ])
        out = emit_function(fn)
        self.assertIn("   ; prologue: 0 arg bytes, 1 local bytes", out)
        self.assertIn("   ; epilogue", out)
        # Blank line immediately before the epilogue comment.
        epilogue_idx = out.index("   ; epilogue")
        self.assertEqual(out[epilogue_idx - 1], "")

    def test_consecutive_blanks_collapse(self):
        # Two adjacent Blank atoms collapse to one blank line so a
        # prologue's trailing blank and an epilogue's leading blank
        # don't pile up when a function has no body between them.
        fn = asm_ast.Function(name="main", is_global=True, instructions=[
            asm_ast.Blank(),
            asm_ast.Blank(),
            asm_ast.Return(),
        ])
        out = emit_function(fn)
        for i in range(len(out) - 1):
            with self.subTest(i=i):
                self.assertFalse(
                    out[i] == "" and out[i + 1] == "",
                    f"double blank at lines {i}..{i+1}",
                )


class TestEmitProgram(unittest.TestCase):
    def test_full(self):
        prog = _prog(
            asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_reg(_A)),
            asm_ast.Return(),
        )
        self.assertEqual(
            emit_program(prog),
            "main:\n   SUBROUTINE\n\n   LDA   #$2A\n   RTS\n",
        )

    def test_multi_function(self):
        # Two functions in source order, separated by a single
        # blank line. Each gets its own `name:` label and
        # SUBROUTINE directive.
        prog = asm_ast.Program(top_level=[
            asm_ast.Function(
                name="foo", is_global=True, params=[],
                instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_reg(_A)),
                    asm_ast.Return(),
                ],
            ),
            asm_ast.Function(
                name="main", is_global=True, params=[],
                instructions=[
                    asm_ast.Call(name="foo"),
                    asm_ast.Return(),
                ],
            ),
        ])
        out = emit_program(prog)
        # foo's body, then a blank line, then main's body.
        self.assertEqual(
            out,
            "foo:\n   SUBROUTINE\n\n   LDA   #$01\n   RTS\n"
            "\n"
            "main:\n   SUBROUTINE\n\n   JSR   foo\n   RTS\n",
        )

    def test_empty_program_emits_just_a_newline(self):
        # No functions at all → empty join + trailing newline. Not
        # a useful program in practice but the dispatcher should
        # handle it without crashing.
        prog = asm_ast.Program(top_level=[])
        self.assertEqual(emit_program(prog), "\n")

    def test_static_variable_renders_label_and_dc_b(self):
        # `static char g = 5;` → labeled byte at the named symbol.
        # The init byte is hex-formatted, two digits, with a leading
        # `$`. The label sits in column 1; the `DC.B` directive in
        # the opcode column. (CharInit is the 1-byte static-init
        # variant post C99 width refresh.)
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(name="g", is_global=False, init=[asm_ast.CharInit(value=5)]),
        ])
        self.assertEqual(emit_program(prog), "g:\n   DC.B  $05\n")

    def test_zero_initialized_static_variable(self):
        # File-scope `char x;` (tentative) resolves to init=0; emits
        # `DC.B $00`. Same shape as an explicit `char x = 0;`.
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(name="x", is_global=True, init=[asm_ast.CharInit(value=0)]),
        ])
        self.assertEqual(emit_program(prog), "x:\n   DC.B  $00\n")

    def test_function_then_static_variable(self):
        # Function and static variable separated by a single blank
        # line, same convention as multi-function programs.
        prog = asm_ast.Program(top_level=[
            asm_ast.Function(
                name="main", is_global=True, params=[],
                instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_reg(_A)),
                    asm_ast.Return(),
                ],
            ),
            asm_ast.StaticVariable(name="g", is_global=False, init=[asm_ast.CharInit(value=7)]),
        ])
        self.assertEqual(
            emit_program(prog),
            "main:\n   SUBROUTINE\n\n   LDA   #$2A\n   RTS\n"
            "\n"
            "g:\n   DC.B  $07\n",
        )

    def test_static_variable_init_byte_range_check(self):
        # CharInit must fit in a byte. The check uses `_check_byte`
        # internally so out-of-range values raise.
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(name="bad", is_global=False, init=[asm_ast.CharInit(value=256)]),
        ])
        with self.assertRaises(ValueError):
            emit_program(prog)

    def test_static_array_renders_per_element(self):
        # Array statics arrive with a flat list of inits — emit
        # one `DC.B` per CharInit element under the variable's label.
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="a", is_global=False,
                init=[
                    asm_ast.CharInit(value=1),
                    asm_ast.CharInit(value=2),
                    asm_ast.CharInit(value=3),
                ],
            ),
        ])
        self.assertEqual(
            emit_program(prog),
            "a:\n   DC.B  $01\n   DC.B  $02\n   DC.B  $03\n",
        )

    def test_static_zero_init_renders_ds_b(self):
        # ZeroInit(N) lays down a run of `N` zero bytes via dasm's
        # `ds.b` directive — more compact than N separate `dc.b`s.
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="a", is_global=False,
                init=[
                    asm_ast.CharInit(value=1),
                    asm_ast.ZeroInit(bytes=4),
                ],
            ),
        ])
        self.assertEqual(
            emit_program(prog),
            "a:\n   DC.B  $01\n   DS.B  4\n",
        )

    def test_static_zero_init_byte_count_must_be_positive(self):
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="a", is_global=False,
                init=[asm_ast.ZeroInit(bytes=0)],
            ),
        ])
        with self.assertRaises(ValueError):
            emit_program(prog)

    def test_static_string_init_renders_dc_b_bytes(self):
        # StringInit lays down `bytes` byte cells: the first
        # len(str) hold the bytes of `str`, any remaining cells
        # are zero-padded. asm_emit renders as `dc.b $XX, $XX, …`
        # — raw hex bytes, sidesteps any string-quoting concerns.
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="s", is_global=False,
                init=[asm_ast.StringInit(str="abc", bytes=4)],
            ),
        ])
        self.assertEqual(
            emit_program(prog),
            "s:\n   DC.B  $61, $62, $63, $00\n",
        )

    def test_static_string_init_no_terminator_when_bytes_equals_len(self):
        # `bytes == len(str)` — the array has no room for the
        # null terminator, so it's elided.
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="s", is_global=False,
                init=[asm_ast.StringInit(str="abc", bytes=3)],
            ),
        ])
        self.assertEqual(
            emit_program(prog),
            "s:\n   DC.B  $61, $62, $63\n",
        )

    def test_static_string_init_extra_padding(self):
        # `bytes > len(str) + 1` — extra trailing zero-pad on top
        # of the conventional null terminator.
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="s", is_global=False,
                init=[asm_ast.StringInit(str="hi", bytes=5)],
            ),
        ])
        self.assertEqual(
            emit_program(prog),
            "s:\n   DC.B  $68, $69, $00, $00, $00\n",
        )

    def test_static_string_init_bytes_lt_len_raises(self):
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="s", is_global=False,
                init=[asm_ast.StringInit(str="abcdef", bytes=3)],
            ),
        ])
        with self.assertRaises(ValueError):
            emit_program(prog)

    def test_static_long_renders_dc_l(self):
        # LongInit lays down 4 bytes via dasm's `dc.l` directive —
        # same byte width as Float, but storing a raw integer rather
        # than an IEEE 754 single.
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="g", is_global=True,
                init=[asm_ast.LongInit(value=1234567890)],
            ),
        ])
        self.assertEqual(
            emit_program(prog),
            "g:\n   DC.L  $499602D2\n",
        )

    def test_static_long_renders_negative_as_twos_complement(self):
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="g", is_global=False,
                init=[asm_ast.LongInit(value=-1)],
            ),
        ])
        self.assertEqual(
            emit_program(prog),
            "g:\n   DC.L  $FFFFFFFF\n",
        )

    def test_static_long_init_range_check(self):
        # The 4-byte cell accepts -2^31..2^32-1; out-of-range raises.
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="bad", is_global=False,
                init=[asm_ast.LongInit(value=1 << 32)],
            ),
        ])
        with self.assertRaises(ValueError):
            emit_program(prog)

    def test_static_int_array_renders_per_element_dc_w(self):
        # `int a[3] = {1, 2, 3};` — each element is a 2-byte IntInit.
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="a", is_global=False,
                init=[
                    asm_ast.IntInit(value=1),
                    asm_ast.IntInit(value=2),
                    asm_ast.IntInit(value=3),
                ],
            ),
        ])
        self.assertEqual(
            emit_program(prog),
            "a:\n   DC.W  $0001\n   DC.W  $0002\n   DC.W  $0003\n",
        )

    def test_static_float_renders_dc_l(self):
        # FloatInit lays down 4 bytes IEEE 754 single, little-endian.
        # 0.5f → 0x3F000000 (sign=0, exp=126, mantissa=0).
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="half", is_global=False,
                init=[asm_ast.FloatInit(bits=0x3F000000)],
            ),
        ])
        self.assertEqual(emit_program(prog), "half:\n   DC.L  $3F000000\n")

    def test_static_double_renders_two_dc_ls(self):
        # DoubleInit lays down 8 bytes IEEE 754 double — two `DC.L`
        # halves, low-half then high-half. 0.5 (double) →
        # 0x3FE0000000000000 → low LE half $00000000, high LE half
        # $3FE00000.
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="half_d", is_global=False,
                init=[asm_ast.DoubleInit(bits=0x3FE0000000000000)],
            ),
        ])
        self.assertEqual(
            emit_program(prog),
            "half_d:\n   DC.L  $00000000\n   DC.L  $3FE00000\n",
        )

    def test_static_double_pi_byte_pattern(self):
        # Spot-check a non-trivial double — 3.14 → 0x40091EB851EB851F.
        # In LE bytes (low to high): 1F 85 EB 51 B8 1E 09 40. As two
        # LE 32-bit halves: $51EB851F, $40091EB8.
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="pi", is_global=False,
                init=[asm_ast.DoubleInit(bits=0x40091EB851EB851F)],
            ),
        ])
        self.assertEqual(
            emit_program(prog),
            "pi:\n   DC.L  $51EB851F\n   DC.L  $40091EB8\n",
        )


class TestEmitDataOperand(unittest.TestCase):
    """`Data(name)` is the absolute-addressing operand the frame-
    layout pass produces from a Pseudo whose name is a top-level
    StaticVariable. It uses 6502 absolute addressing (no LDY
    indirect-Y preamble) — `LDA name`, `STA name`, `ADC name`, etc."""

    def test_mov_data_to_a(self):
        out = emit_instruction(asm_ast.Mov(
            src=asm_ast.Data(name="g", offset=0), dst=_reg(_A),
        ))
        self.assertEqual(out, ["   LDA   g"])

    def test_mov_a_to_data(self):
        out = emit_instruction(asm_ast.Mov(
            src=_reg(_A), dst=asm_ast.Data(name="g", offset=0),
        ))
        self.assertEqual(out, ["   STA   g"])

    def test_mov_imm_to_data(self):
        out = emit_instruction(asm_ast.Mov(
            src=asm_ast.Imm(value=42), dst=asm_ast.Data(name="g", offset=0),
        ))
        self.assertEqual(out, ["   LDA   #$2A", "   STA   g"])

    def test_mov_data_to_data(self):
        # Static-to-static copy: load via absolute, store via absolute.
        # No LDY needed for either side.
        out = emit_instruction(asm_ast.Mov(
            src=asm_ast.Data(name="src", offset=0),
            dst=asm_ast.Data(name="dst", offset=0),
        ))
        self.assertEqual(out, ["   LDA   src", "   STA   dst"])

    def test_mov_data_to_frame(self):
        # Static → local: absolute load, then indirect-Y store.
        out = emit_instruction(asm_ast.Mov(
            src=asm_ast.Data(name="g", offset=0),
            dst=asm_ast.Frame(offset=1),
        ))
        self.assertEqual(out, [
            "   LDA   g",
            "   LDY   #$01",
            "   STA   (FP),Y",
        ])

    def test_mov_frame_to_data(self):
        # Local → static: indirect-Y load, then absolute store.
        out = emit_instruction(asm_ast.Mov(
            src=asm_ast.Frame(offset=1),
            dst=asm_ast.Data(name="g", offset=0),
        ))
        self.assertEqual(out, [
            "   LDY   #$01",
            "   LDA   (FP),Y",
            "   STA   g",
        ])

    def test_add_data_to_a(self):
        # ADC has absolute-mode support; no LDY needed.
        out = emit_instruction(asm_ast.Add(
            src=asm_ast.Data(name="g", offset=0), dst=_reg(_A),
        ))
        self.assertEqual(out, ["   ADC   g"])

    def test_sub_data_from_a(self):
        out = emit_instruction(asm_ast.Sub(
            src=asm_ast.Data(name="g", offset=0), dst=_reg(_A),
        ))
        self.assertEqual(out, ["   SBC   g"])

    def test_and_data(self):
        out = emit_instruction(asm_ast.And(
            src=asm_ast.Data(name="g", offset=0), dst=_reg(_A),
        ))
        self.assertEqual(out, ["   AND   g"])

    def test_or_data(self):
        out = emit_instruction(asm_ast.Or(
            src=asm_ast.Data(name="g", offset=0), dst=_reg(_A),
        ))
        self.assertEqual(out, ["   ORA   g"])

    def test_xor_data(self):
        out = emit_instruction(asm_ast.Xor(
            src1=_reg(_A), src2=asm_ast.Data(name="g", offset=0), dst=_reg(_A),
        ))
        self.assertEqual(out, ["   EOR   g"])

    def test_compare_a_with_data(self):
        out = emit_instruction(asm_ast.Compare(
            left=_reg(_A), right=asm_ast.Data(name="g", offset=0),
        ))
        self.assertEqual(out, ["   CMP   g"])

    def test_compare_x_with_data(self):
        # CPX has absolute-mode support, so Data on the right works
        # with X on the left.
        out = emit_instruction(asm_ast.Compare(
            left=_reg(asm_ast.X()), right=asm_ast.Data(name="g", offset=0),
        ))
        self.assertEqual(out, ["   CPX   g"])


class TestEmitZPOperand(unittest.TestCase):
    """`ZP(address, offset)` is the absolute-addressing operand the
    frame-layout pass produces from a Pseudo whose name was assigned
    a zero-page slot by register allocation. Equivalent to Data for
    emit purposes — both lower to native 6502 absolute / zero-page
    addressing — but `address + offset` is folded at emit time into
    a literal byte address. Should support every dispatch site that
    Data does."""

    def test_mov_imm_to_zp(self):
        out = emit_instruction(asm_ast.Mov(
            src=asm_ast.Imm(value=7),
            dst=asm_ast.ZP(address=0x82, offset=0),
        ))
        self.assertEqual(out, ["   LDA   #$07", "   STA   $82"])

    def test_mov_zp_to_reg_a(self):
        out = emit_instruction(asm_ast.Mov(
            src=asm_ast.ZP(address=0x82, offset=0), dst=_reg(_A),
        ))
        self.assertEqual(out, ["   LDA   $82"])

    def test_mov_a_to_zp(self):
        out = emit_instruction(asm_ast.Mov(
            src=_reg(_A), dst=asm_ast.ZP(address=0x82, offset=0),
        ))
        self.assertEqual(out, ["   STA   $82"])

    def test_mov_zp_high_byte_offset_folds(self):
        # ZP(0x82, 1) → $83. Same offset-folding semantics as Data.
        out = emit_instruction(asm_ast.Mov(
            src=asm_ast.ZP(address=0x82, offset=1), dst=_reg(_A),
        ))
        self.assertEqual(out, ["   LDA   $83"])

    def test_mov_zp_to_reg_x(self):
        # 6502 has LDX zp/abs natively — no need to bounce through A.
        out = emit_instruction(asm_ast.Mov(
            src=asm_ast.ZP(address=0x82, offset=0), dst=_reg(_X),
        ))
        self.assertEqual(out, ["   LDX   $82"])

    def test_mov_zp_to_reg_y(self):
        out = emit_instruction(asm_ast.Mov(
            src=asm_ast.ZP(address=0x82, offset=0), dst=_reg(_Y),
        ))
        self.assertEqual(out, ["   LDY   $82"])

    def test_mov_zp_to_zp(self):
        out = emit_instruction(asm_ast.Mov(
            src=asm_ast.ZP(address=0x82, offset=0),
            dst=asm_ast.ZP(address=0x90, offset=0),
        ))
        self.assertEqual(out, ["   LDA   $82", "   STA   $90"])

    def test_mov_zp_to_frame(self):
        out = emit_instruction(asm_ast.Mov(
            src=asm_ast.ZP(address=0x82, offset=0),
            dst=asm_ast.Frame(offset=1),
        ))
        self.assertEqual(out, [
            "   LDA   $82",
            "   LDY   #$01",
            "   STA   (FP),Y",
        ])

    def test_add_zp_source(self):
        out = emit_instruction(asm_ast.Add(
            src=asm_ast.ZP(address=0x82, offset=0), dst=_reg(_A),
        ))
        self.assertEqual(out, ["   ADC   $82"])

    def test_sub_zp_source(self):
        out = emit_instruction(asm_ast.Sub(
            src=asm_ast.ZP(address=0x82, offset=0), dst=_reg(_A),
        ))
        self.assertEqual(out, ["   SBC   $82"])

    def test_compare_a_with_zp(self):
        out = emit_instruction(asm_ast.Compare(
            left=_reg(_A), right=asm_ast.ZP(address=0x82, offset=0),
        ))
        self.assertEqual(out, ["   CMP   $82"])

    def test_compare_x_with_zp(self):
        out = emit_instruction(asm_ast.Compare(
            left=_reg(_X), right=asm_ast.ZP(address=0x82, offset=0),
        ))
        self.assertEqual(out, ["   CPX   $82"])

    def test_inc_zp(self):
        out = emit_instruction(asm_ast.Inc(
            dst=asm_ast.ZP(address=0x82, offset=0),
        ))
        self.assertEqual(out, ["   INC   $82"])

    def test_dec_zp(self):
        out = emit_instruction(asm_ast.Dec(
            dst=asm_ast.ZP(address=0x82, offset=0),
        ))
        self.assertEqual(out, ["   DEC   $82"])

    def test_asl_zp(self):
        out = emit_instruction(asm_ast.ArithmeticShiftLeft(
            dst=asm_ast.ZP(address=0x82, offset=0),
        ))
        self.assertEqual(out, ["   ASL   $82"])

    def test_zp_address_out_of_range_raises(self):
        # Defensive: regalloc shouldn't produce > 0xFF, but we check.
        with self.assertRaises(ValueError):
            emit_instruction(asm_ast.Mov(
                src=asm_ast.ZP(address=0x100, offset=0), dst=_reg(_A),
            ))


class TestColumnAlignment(unittest.TestCase):
    """Column 1 labels, column 4 opcodes / directives, column 10 operands."""

    def test_columns(self):
        prog = _prog(
            asm_ast.Mov(src=asm_ast.Imm(value=0x2A), dst=_reg(_A)),
            asm_ast.Return(),
        )
        lines = emit_program(prog).splitlines()
        # Label at column 1 (index 0).
        self.assertTrue(lines[0].startswith("main:"))
        # SUBROUTINE directive at column 4.
        self.assertEqual(lines[1][:3], "   ")
        self.assertEqual(lines[1][3:], "SUBROUTINE")
        # Blank line separating directive from instructions.
        self.assertEqual(lines[2], "")
        # Opcode at column 4 (index 3), operand at column 10 (index 9).
        self.assertEqual(lines[3][:3], "   ")
        self.assertEqual(lines[3][3:6], "LDA")
        self.assertEqual(lines[3][6:9], "   ")
        self.assertEqual(lines[3][9:], "#$2A")
        # RTS has no operand.
        self.assertEqual(lines[4], "   RTS")


class TestEmitAdd(unittest.TestCase):
    """Add at emit is a single ADC (with addressing-mode setup for
    indirect-Y sources). Carry must be set up by an earlier ClearCarry;
    dst must be Reg(A); src can be Imm/Stack/Frame."""

    def test_imm_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Add(src=asm_ast.Imm(value=0x2A), dst=_reg(_A))
            ),
            ["   ADC   #$2A"],
        )

    def test_stack_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Add(src=asm_ast.Stack(offset=3), dst=_reg(_A))
            ),
            ["   LDY   #$03", "   ADC   (SSP),Y"],
        )

    def test_frame_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Add(src=asm_ast.Frame(offset=2), dst=_reg(_A))
            ),
            ["   LDY   #$02", "   ADC   (FP),Y"],
        )

    def test_imm_out_of_range_raises(self):
        for v in [-1, 256, 1000]:
            with self.subTest(v=v):
                with self.assertRaises(ValueError):
                    emit_instruction(
                        asm_ast.Add(src=asm_ast.Imm(value=v), dst=_reg(_A))
                    )

    def test_dst_must_be_a(self):
        for dst in [_reg(_X), _reg(_Y), asm_ast.Stack(offset=1),
                    asm_ast.Frame(offset=1), asm_ast.Imm(value=0)]:
            with self.subTest(dst=dst):
                with self.assertRaises(ValueError):
                    emit_instruction(
                        asm_ast.Add(src=asm_ast.Imm(value=1), dst=dst)
                    )

    def test_register_src_raises(self):
        # Reg(X)/Reg(Y)/Reg(A) — ADC has no register-direct source.
        for src in [_reg(_X), _reg(_Y), _reg(_A)]:
            with self.subTest(src=src):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Add(src=src, dst=_reg(_A)))

class TestEmitSub(unittest.TestCase):
    """Sub at emit is a single SBC (with addressing-mode setup for
    indirect-Y sources); same operand constraints as Add. Carry must
    be set by a preceding SetCarry."""

    def test_imm_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Sub(src=asm_ast.Imm(value=0x2A), dst=_reg(_A))
            ),
            ["   SBC   #$2A"],
        )

    def test_stack_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Sub(src=asm_ast.Stack(offset=3), dst=_reg(_A))
            ),
            ["   LDY   #$03", "   SBC   (SSP),Y"],
        )

    def test_frame_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Sub(src=asm_ast.Frame(offset=2), dst=_reg(_A))
            ),
            ["   LDY   #$02", "   SBC   (FP),Y"],
        )

    def test_dst_must_be_a(self):
        with self.assertRaises(ValueError):
            emit_instruction(
                asm_ast.Sub(src=asm_ast.Imm(value=1), dst=_reg(_X))
            )

    def test_register_src_raises(self):
        for src in [_reg(_X), _reg(_Y), _reg(_A)]:
            with self.subTest(src=src):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Sub(src=src, dst=_reg(_A)))


class TestEmitClearSetCarry(unittest.TestCase):
    def test_clear_carry(self):
        self.assertEqual(emit_instruction(asm_ast.ClearCarry()), ["   CLC"])

    def test_set_carry(self):
        self.assertEqual(emit_instruction(asm_ast.SetCarry()), ["   SEC"])


class TestEmitInc(unittest.TestCase):
    def test_inc_x(self):
        self.assertEqual(
            emit_instruction(asm_ast.Inc(dst=_reg(_X))),
            ["   INX"],
        )

    def test_inc_y(self):
        self.assertEqual(
            emit_instruction(asm_ast.Inc(dst=_reg(_Y))),
            ["   INY"],
        )

    def test_inc_a_raises(self):
        # Plain 6502 has no INA.
        with self.assertRaises(ValueError):
            emit_instruction(asm_ast.Inc(dst=_reg(_A)))

    def test_inc_other_raises(self):
        for dst in [asm_ast.Imm(value=1), asm_ast.Stack(offset=1),
                    asm_ast.Frame(offset=1)]:
            with self.subTest(dst=dst):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Inc(dst=dst))


class TestEmitDec(unittest.TestCase):
    def test_dec_x(self):
        self.assertEqual(
            emit_instruction(asm_ast.Dec(dst=_reg(_X))),
            ["   DEX"],
        )

    def test_dec_y(self):
        self.assertEqual(
            emit_instruction(asm_ast.Dec(dst=_reg(_Y))),
            ["   DEY"],
        )

    def test_dec_a_raises(self):
        with self.assertRaises(ValueError):
            emit_instruction(asm_ast.Dec(dst=_reg(_A)))


class TestEmitPushPop(unittest.TestCase):
    def test_push_a(self):
        self.assertEqual(
            emit_instruction(asm_ast.Push(src=_reg(_A))),
            ["   PHA"],
        )

    def test_pop_a(self):
        self.assertEqual(
            emit_instruction(asm_ast.Pop(dst=_reg(_A))),
            ["   PLA"],
        )

    def test_push_non_a_raises(self):
        for src in [_reg(_X), _reg(_Y),
                    asm_ast.Imm(value=1), asm_ast.Stack(offset=1)]:
            with self.subTest(src=src):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Push(src=src))

    def test_pop_non_a_raises(self):
        for dst in [_reg(_X), _reg(_Y), asm_ast.Stack(offset=1)]:
            with self.subTest(dst=dst):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Pop(dst=dst))


class TestEmitXor(unittest.TestCase):
    def test_a_imm(self):
        self.assertEqual(
            emit_instruction(asm_ast.Xor(
                src1=_reg(_A),
                src2=asm_ast.Imm(value=0xFF),
                dst=_reg(_A),
            )),
            ["   EOR   #$FF"],
        )

    def test_imm_a_other_order(self):
        # XOR is commutative; emit accepts either ordering of the
        # (Reg(A), Imm) pair.
        self.assertEqual(
            emit_instruction(asm_ast.Xor(
                src1=asm_ast.Imm(value=0x0A),
                src2=_reg(_A),
                dst=_reg(_A),
            )),
            ["   EOR   #$0A"],
        )

    def test_dst_must_be_a(self):
        with self.assertRaises(ValueError):
            emit_instruction(asm_ast.Xor(
                src1=_reg(_A),
                src2=asm_ast.Imm(value=1),
                dst=_reg(_X),
            ))

    def test_one_src_must_be_a(self):
        # One src must be Reg(A); the other carries the addressing mode.
        # Neither side being A is rejected. Two Reg(A) is also rejected
        # because the non-A side then fails the Imm/Stack/Frame check.
        bad = [
            (asm_ast.Imm(value=1), asm_ast.Imm(value=2)),     # both Imm
            (_reg(_A), _reg(_A)),                              # both A
            (_reg(_X), asm_ast.Imm(value=1)),                  # X not A
            (asm_ast.Stack(offset=1), asm_ast.Imm(value=1)),   # Stack not A
        ]
        for s1, s2 in bad:
            with self.subTest(src1=s1, src2=s2):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Xor(
                        src1=s1, src2=s2, dst=_reg(_A),
                    ))

    def test_a_stack(self):
        # A XOR <byte at SSP+off> — reads via indirect-Y, then EOR.
        self.assertEqual(
            emit_instruction(asm_ast.Xor(
                src1=_reg(_A),
                src2=asm_ast.Stack(offset=3),
                dst=_reg(_A),
            )),
            ["   LDY   #$03", "   EOR   (SSP),Y"],
        )

    def test_a_frame_either_order(self):
        # XOR is commutative; the non-A operand picks the addressing
        # mode regardless of which slot it sits in.
        expected = ["   LDY   #$02", "   EOR   (FP),Y"]
        self.assertEqual(
            emit_instruction(asm_ast.Xor(
                src1=_reg(_A),
                src2=asm_ast.Frame(offset=2),
                dst=_reg(_A),
            )),
            expected,
        )
        self.assertEqual(
            emit_instruction(asm_ast.Xor(
                src1=asm_ast.Frame(offset=2),
                src2=_reg(_A),
                dst=_reg(_A),
            )),
            expected,
        )

    def test_imm_out_of_range_raises(self):
        with self.assertRaises(ValueError):
            emit_instruction(asm_ast.Xor(
                src1=_reg(_A),
                src2=asm_ast.Imm(value=256),
                dst=_reg(_A),
            ))


class TestEmitShiftRotateAcc(unittest.TestCase):
    """ASL/LSR/ROL/ROR all currently target the accumulator only —
    soft-stack values can't be addressed by these opcodes (no
    indirect-Y mode). Reg(A) emits `<OP> A`; anything else raises."""

    _CASES = [
        (asm_ast.ArithmeticShiftLeft, "ASL"),
        (asm_ast.LogicalShiftRight,   "LSR"),
        (asm_ast.RotateLeft,          "ROL"),
        (asm_ast.RotateRight,         "ROR"),
    ]

    def test_acc_emit(self):
        for cls, opcode in self._CASES:
            with self.subTest(op=opcode):
                self.assertEqual(
                    emit_instruction(cls(dst=_reg(_A))),
                    [f"   {opcode}   A"],
                )

    def test_non_acc_dst_raises(self):
        for cls, _ in self._CASES:
            for dst in [_reg(_X), _reg(_Y),
                        asm_ast.Imm(value=1),
                        asm_ast.Stack(offset=1),
                        asm_ast.Frame(offset=1)]:
                with self.subTest(op=cls.__name__, dst=dst):
                    with self.assertRaises(ValueError):
                        emit_instruction(cls(dst=dst))

class TestEmitAnd(unittest.TestCase):
    """And at emit is a single AND instruction (with addressing-mode
    setup for indirect-Y sources). dst must be Reg(A); src can be
    Imm/Stack/Frame. AND/ORA do not affect carry, so no carry setup
    is required."""

    def test_imm_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.And(src=asm_ast.Imm(value=0x0F), dst=_reg(_A))
            ),
            ["   AND   #$0F"],
        )

    def test_stack_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.And(src=asm_ast.Stack(offset=3), dst=_reg(_A))
            ),
            ["   LDY   #$03", "   AND   (SSP),Y"],
        )

    def test_frame_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.And(src=asm_ast.Frame(offset=2), dst=_reg(_A))
            ),
            ["   LDY   #$02", "   AND   (FP),Y"],
        )

    def test_dst_must_be_a(self):
        with self.assertRaises(ValueError):
            emit_instruction(
                asm_ast.And(src=asm_ast.Imm(value=1), dst=_reg(_X))
            )

    def test_register_src_raises(self):
        for src in [_reg(_X), _reg(_Y), _reg(_A)]:
            with self.subTest(src=src):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.And(src=src, dst=_reg(_A)))

class TestEmitOr(unittest.TestCase):
    """Or at emit is a single ORA instruction; same operand shape as And."""

    def test_imm_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Or(src=asm_ast.Imm(value=0xF0), dst=_reg(_A))
            ),
            ["   ORA   #$F0"],
        )

    def test_stack_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Or(src=asm_ast.Stack(offset=3), dst=_reg(_A))
            ),
            ["   LDY   #$03", "   ORA   (SSP),Y"],
        )

    def test_frame_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Or(src=asm_ast.Frame(offset=2), dst=_reg(_A))
            ),
            ["   LDY   #$02", "   ORA   (FP),Y"],
        )

    def test_dst_must_be_a(self):
        with self.assertRaises(ValueError):
            emit_instruction(
                asm_ast.Or(src=asm_ast.Imm(value=1), dst=_reg(_X))
            )

    def test_register_src_raises(self):
        for src in [_reg(_X), _reg(_Y), _reg(_A)]:
            with self.subTest(src=src):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Or(src=src, dst=_reg(_A)))


if __name__ == "__main__":
    unittest.main()
