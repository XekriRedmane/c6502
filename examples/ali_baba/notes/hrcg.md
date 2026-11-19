# hrcg.c

## What it translates

The live code of the HRCG, `main.asm` lines 15801-15881 and
15967-16617, with the behavior INIT_HRCG's two patches leave.

| Original | C |
|---|---|
| HRCG_INIT $92A8, HRCG_INIT_BODY $92C5, HRCG_RESET $95FA, HRCG_RESET_WINDOW $95D3, HRCG_SET_REPLACE $944A, HRCG_DISPLAY_PAGE $94B3 as patched | `hrcg_init` |
| HRCG_COUT $93BE, HRCG_PROCESS_CHAR $93CE | `hrcg_cout` |
| HRCG_DISPATCH_TBL $93EE | the switch in `hrcg_control`, and `hrcg_shifted_control` |
| HRCG_CTRL_A $9424, HRCG_CHARSET_SELECT $942C, HRCG_SET_STD_FONT $943F | case ctrl-A, `hrcg_select_font` |
| HRCG_CTRL_B_SAVE $9450, HRCG_CTRL_C $9478, HRCG_CTRL_D $9493 | `hrcg_save`, `hrcg_restore_column`, `hrcg_restore` |
| HRCG_CTRL_F, _I, _K, _L, _N, _O, _P | cases of `hrcg_control` |
| HRCG_CTRL_S, HRCG_CTRL_W (shifted) | `hrcg_shifted_control` |
| HRCG_CTRL_V $957C | `hrcg_set_top_left` |
| HRCG_DRAW_CHAR $9621 | `hrcg_draw_char(c, raw)` |
| HRCG_ADVANCE_CURSOR $969E, HRCG_LINE_FEED $96AA | `hrcg_advance_cursor`, `hrcg_line_feed` |
| HRCG_SCROLL_UP $96C7 | `hrcg_scroll_up` |
| HRCG_HOME $9716, HRCG_CLRLINE $9720 | `hrcg_clear_from(row, col)`, called by ctrl-P and ctrl-F |
| HRCG_FILL_LINE $9737 | `hrcg_fill_line(row, col)` |
| HRCG_CALC_HIRES_ADDR $9771, HRCG_CALC_HIRES_ROW $9773, HRCG_ROW_TABLE $9785 | `hrcg_row_address(row, col)`, `HRCG_ROW_BASE` |
| the parameter block $9306-$9319 | the statics `hrcg`, `hrcg_saved`, `hrcg_saved_valid`, `hrcg_saved_ch`, `hrcg_saved_cv`, `hrcg_next` |
| STD_FONT_DATA $97A5 | `static const uint8_t STD_FONT_DATA[0x300]`, from main.bin |

Left out, as UNITS.md allows: HRCG_ORIGINAL_ENTRY, HRCG_ENTRY2/3,
HRCG_PRINT_VERSION and its string, HRCG_GETKEY, HRCG_CLREOL, the
handlers of ctrl-E, G, H, J, M, Q, R, T, Y, and Z, the normal ctrl-S
and ctrl-W, the shifted variants of every handler but S and W, the OR,
AND, and XOR draw modes, and INVERT_FLAG's one-shot $C0. A coverage
run of the original in report_hrcg found none of them reached. In C
those codes do nothing. Nothing is left incomplete.

## Departures

The file's opening comment lists them. In short:

- CSW and KSW aren't set, and hrcg_cout doesn't preserve registers,
  because every unit calls hrcg_cout directly.
- The three parameters that never change in this game (VIDEO_MODE $20,
  DRAW_MODE 0, PAGE_MASK 0) are constants, so the saved parameters are
  four fields, `mask`, `wrap`, `lowercase`, and `font`.
- HRCG_MODE_FLAGS is `hrcg_next`, cleared before each character is
  handled, where the original shifts it left twice.
- The row table is three bytes indexed by row / 8.

## Header changes needed

None.

## Compiler bugs and gaps

No bug was hit. These cost bytes:

1. The products in hrcg_row_address's line address are 16-bit
   arithmetic through zero-page temporaries. `row % 8` and `row / 8`
   compile to `AND #7` and three `LSR`s, and `0x80 * (row % 8)` to an
   `LSR` and a `ROR`, but `0x28 * (row / 8)` is two 16-bit shifts and
   a 16-bit add, and the high byte of `row / 8 << 3`, which is 0 for
   any byte, is computed with five more `LSR`s. `row / 8` is at most
   31, so value_range's bounds would show that byte to be 0.
   hrcg_row_address is 90 bytes, where the old C, with `row >> 3` and
   a 3-byte table, was 51.

   The high byte was fixed on 19 Nov 2026: a left shift whose result
   stays below 256 is a byte shift
   (docs/optimizations/narrow_byte_range.md). hrcg_row_address is 73
   bytes, and the products are still 16-bit arithmetic.

   ```c
   #include <stdint.h>
   uint16_t g(uint8_t row) { return 0x80 * (row % 8) + 0x28 * (row / 8); }
   ```

2. `row + 1 < WNDBTM`, with both bytes, is a signed 16-bit comparison
   with an overflow test (`BVC; EOR #$80`). row + 1 is 1-256 and
   WNDBTM 0-255, so an unsigned comparison would do. hrcg_scroll_up is
   116 bytes, where it was 100.

   ```c
   #include <stdint.h>
   extern uint8_t WNDTOP [[c6502::zeropage(0x22)]];
   extern uint8_t WNDBTM [[c6502::zeropage(0x23)]];
   void copy(uint8_t row);
   void f(void) {
       for (uint8_t row = WNDTOP; row + 1 < WNDBTM; row++)
           copy(row);
   }
   ```

3. A bool argument computed from a comparison
   (`hrcg_control(ch, next == HRCG_NEXT_SHIFTED)`) compiled to a
   compare and two branches that load 0 or 1. Splitting the function
   into `hrcg_control` and `hrcg_shifted_control` saved the bytes. This
   item was here before the rewrite and wasn't measured again.

hrcg_select_font's `FONT_DATA[c - '1']` compiles to a lookup in two
6-byte tables of the fonts' addresses that the compiler makes, 49
bytes in all, where the old C's 16-bit add was 50.

## Size

From `objdump.py -t` of `hrcg.o` (before linking), against `main.lst`.

| | Original | C |
|---|---:|---:|
| Code of the routines translated | 846 | 877 |
| Dispatch table and row table | 66 | 12 (the fonts' addresses, which the compiler makes) |
| Variables | 21 (at $9305-$9319) | 10 in .bss, 4 in zero page |
| Standard font | 768 | 768 |

The original's 846 bytes are the chunks that hold the translated paths
($92A8-$92AA, $92C5-$92E7, $93BE-$93ED, $9424-$94C0, $94C9-$94D0,
$94E6-$94FD, $9519-$9544, $9561-$956E, $957C-$95D0, $95D3-$95F7,
$95FA-$9732, and $9737-$9784), including the few shifted variants
inside them. The unit's whole code is 1141 bytes. The C's are the
symbol sizes of `nm.py -S hrcg.o`, and its code was 835 bytes before
the rewrite into plain C on 18 Nov 2026. It was 901 bytes after the
rewrite, and 877 once c6502 divided a byte by 8 without `sdivmod16`.

## Checked against the original

A differential test in the scratchpad
(`C:/Users/rober/AppData/Local/Temp/claude/b40883e6c3b1e075/7b225eef-0a6c-4700-af61-411f046928b9/scratchpad/tr/link/diff.py`)
links hrcg.c and text.c at $A900 with stubs that call the original's
RENDER_FONT_CHAR, DISPLAY_STATUS_BAR, DELAY_WITH_ANIMATION, and
TUNE_PLAYER, loads both programs on `sim/ali_baba.py`'s machine over
main.bin, and runs the original's HRCG_COUT and the C's hrcg_cout side
by side. After each character it compares the events (soft switches
and Monitor calls with A where the routine reads it, and hi-res page
1's CRC at each one), $20-$29, the text page, hi-res page 1, and page
$5A. A scripted run of every live code, the font selectors '0'-'9' and
others, scrolling, wrapping, ctrl-B/C/D, and ctrl-V windows, and 25
random runs of 1500 characters each, matched in every compared byte
and event. $2A/$2B differ, because the C keeps the row pointer in a
local, and the ROM's SCROLL sets them before it reads them. The test
ran on the C as it was before the rewrite into plain C on 18 Nov 2026,
and the rewrite was checked only with the six scripts of TestRuns.

## Where the original's comments and code disagree

- HRCG_VIDEO_MODE's comment names its values replace, OR, AND, and
  XOR. HRCG_CALC_HIRES_ROW ORs it into the high byte, so it is the
  hi-res page ($20 or $40). HRCG_DRAW_MODE is the combine mode.
- HRCG_INVERT_FLAG's comment calls $C0 "XOR-invert". HRCG_DRAW_CHAR
  shifts $C0 to $80 and draws that one character without the
  lower-case map, so $C0 delays the map by one character.
- HRCG_CTRL_Q's `JMP $FC24` is commented "ROM VTAB". $FC24 is VTABZ,
  with A = HRCG_SAVED_CV.
- HRCG_CTRL_H's comment "CV >= WNDTOP?" is on a branch that returns
  when WNDTOP >= CV, so CV is decremented only when it is greater than
  WNDTOP.

## For the integrator

- hrcg_init must run before the first hrcg_cout, as GAME_INIT's order
  has it. Before it, `hrcg.font` is null.
- A font selector '6'-'9' reads the memory after FONT_DATA, $92A5 on,
  as the original does. In the C build that memory holds whatever the
  linker puts there, so those selectors would draw different glyphs.
  The coverage run saw only '0'-'5', and entities' RENDER_FONT_CHAR
  should be checked for the largest CHAR_CHARSET it makes.
