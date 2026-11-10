/* The Apple II Monitor ROM routines a program calls: routine entry
 * points, and the zero-page bytes those routines use.
 *
 * Including this header tells c6502 that the program calls the
 * Monitor's text, keyboard, line input, lo-res graphics, or delay
 * routines. Every zero-page byte those routines use is declared below
 * with `__attribute__((address(...)))`, so the compiler keeps its own
 * zero-page block (SSP, FP, HARGS, DPTR) and its zero-page globals
 * off them, and doesn't initialize them.
 *
 * The bytes come from the unenhanced Apple IIe F8 ROM listing at
 * https://6502disassembly.com/a2-rom/Unenh_IIe_F8ROM.html and the
 * 80-column firmware listing at
 * https://6502disassembly.com/a2-rom/Unenh_IIe_80col.html, which
 * HOME, CLREOL, scrolling, and KEYIN reach through GOTOCX ($FBB4).
 * They're grouped by the routines that read or write them:
 *
 *   $1F        CLREOL ("video's YSAV1")
 *   $20-$25    text window and cursor (SETWND, VTAB, BS, UP, ADVANCE,
 *              LF, CR; CH also by RDKEY)
 *   $26-$27    GBASL/GBASH, lo-res graphics only
 *   $28-$29    BASL/BASH, text output, RDKEY, GETLN
 *   $2A-$2B    not touched by this ROM, but the Apple II+ ROM's SCROLL
 *              uses them (BAS2L, BAS2H)
 *   $2C-$2E    H2, V2, MASK, lo-res graphics
 *   $30        COLOR, lo-res graphics only
 *   $32        INVFLG, COUT1 and line input
 *   $33        PROMPT, GETLN
 *   $35        YSAV1, COUT1
 *   $36-$39    CSW and KSW, the output and input hooks
 *   $45        ACC, written by the IRQ entry at $FA40 on every interrupt
 *   $48        STATUS, written by INIT
 *   $4E-$4F    RNDL/RNDH, KEYIN's random seed in the 80-column
 *              firmware (KEYDLY, $C2D5 and $C2D9)
 *
 * The rest of the Monitor's zero page belongs to its interactive part
 * (the command loop and memory tools, the disassembler, register
 * display and BRK handling, cassette I/O, and the power-up slot scan),
 * which a program calling these routines never reaches. A program
 * that uses that part too includes <apple2/interactive_monitor.h> as well.
 *
 * The firmware also keeps state in screen holes ($0478, $047B,
 * $04FB, $057B, $05FB, $077B, $07FB) and briefly writes $0400. The
 * compiler never places anything there.
 *
 * Two zero-page objects may not overlap, so the bytes a program is
 * unlikely to use directly are covered by `__monitor_*` objects.
 */
#ifndef C6502_APPLE2_MONITOR_IO_H
#define C6502_APPLE2_MONITOR_IO_H

#include <stdint.h>

/* ---- Routines -------------------------------------------------------
 * Each one takes its argument in A and returns its result in A, which
 * is c6502's zp_abi convention with a `register` parameter.
 * `preserves_x` / `preserves_y` mark the registers every path through
 * the routine leaves unchanged, per the two listings, so the compiler
 * can keep a value in that register across the call. */

/* Clears the text window and puts the cursor at its top left.
 * Neither it nor the firmware it reaches writes X. It loads Y with
 * the firmware's function code. */
__attribute__((zp_abi, preserves_x, address(0xFC58))) void HOME(void);

/* Clears the text window from the cursor to the end of the line.
 * Neither it nor the 80-column firmware it reaches writes X. Y ends
 * as WNDWDTH. */
__attribute__((zp_abi, preserves_x, address(0xFC9C))) void CLREOL(void);

/* Waits for a key and returns it, with the high bit set. RDKEY loads
 * Y from CH; KEYIN saves and restores X ($C294, $C2BC). */
__attribute__((zp_abi, preserves_x, address(0xFD0C))) uint8_t RDKEY(void);

/* Prints `c` through the output hook CSW (normally COUT1). What it
 * preserves depends on where CSW points, so it promises nothing. */
__attribute__((zp_abi, address(0xFDED))) void COUT(register uint8_t c);

/* Prints `c` on the screen, ignoring CSW. Characters $A0 and up are
 * ANDed with INVFLG first. No code it runs writes X; scrolling at
 * the bottom of the window goes through the firmware, which saves and
 * restores X ($CCA4, $CD1B). It saves Y in YSAV1 ($FDF6) and reloads
 * it before returning ($FDFD), on every path. */
__attribute__((zp_abi, preserves_x, preserves_y, address(0xFDF0)))
void COUT1(register uint8_t c);

/* Waits for a time that grows with `a`. It writes neither X nor Y. */
__attribute__((zp_abi, preserves_x, preserves_y, address(0xFCA8)))
void WAIT(register uint8_t a);

/* ---- Zero page ------------------------------------------------------ */

extern volatile uint8_t __monitor_zp_1f   __attribute__((address(0x1F)));

extern volatile uint8_t WNDLFT            __attribute__((address(0x20)));
extern volatile uint8_t WNDWDTH           __attribute__((address(0x21)));
extern volatile uint8_t WNDTOP            __attribute__((address(0x22)));
extern volatile uint8_t WNDBTM            __attribute__((address(0x23)));
extern volatile uint8_t CH                __attribute__((address(0x24)));
extern volatile uint8_t CV                __attribute__((address(0x25)));
extern volatile uint16_t GBAS             __attribute__((address(0x26)));
extern volatile uint16_t BAS              __attribute__((address(0x28)));
extern volatile uint16_t __monitor_bas2   __attribute__((address(0x2A)));

/* $2C-$2E: H2, V2, MASK. */
extern volatile uint8_t __monitor_zp_2c[3] __attribute__((address(0x2C)));

extern volatile uint8_t COLOR             __attribute__((address(0x30)));
extern volatile uint8_t INVFLG            __attribute__((address(0x32)));
extern volatile uint8_t PROMPT            __attribute__((address(0x33)));
extern volatile uint8_t __monitor_ysav1   __attribute__((address(0x35)));
extern volatile uint16_t CSW              __attribute__((address(0x36)));
extern volatile uint16_t KSW              __attribute__((address(0x38)));
extern volatile uint8_t __monitor_acc     __attribute__((address(0x45)));
extern volatile uint8_t __monitor_status  __attribute__((address(0x48)));
extern volatile uint16_t __monitor_rnd    __attribute__((address(0x4E)));

#endif
