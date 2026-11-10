/* The Apple II Monitor ROM: routine entry points, and the zero-page
 * bytes the Monitor owns.
 *
 * Including this header tells c6502 that the program runs alongside
 * the Monitor. Every Monitor zero-page byte is declared below with
 * `__attribute__((address(...)))`, so the compiler keeps its own
 * zero-page block (SSP, FP, HARGS, DPTR) and its zero-page globals
 * off them, and doesn't initialize them. The bytes are the ones the
 * unenhanced Apple IIe F8 ROM listing at
 * https://6502disassembly.com/a2-rom/Unenh_IIe_F8ROM.html reads or
 * writes:
 *
 *   $00-$01   LOC0, LOC1, used only by the power-up slot scan
 *   $1F       written by CLREOL ("video's YSAV1")
 *   $20-$49   the Monitor's variables, $2A-$2B included
 *
 * $2A-$2B aren't touched by this ROM, but the Apple II+ ROM's SCROLL
 * uses them (BAS2L, BAS2H). On a IIe, CLREOL, HOME, SCROLL, and KEYIN
 * continue into the 80-column firmware at $C100, which isn't in that
 * listing, so any zero page it uses isn't covered here.
 *
 * The names a program is likely to read or set are declared on
 * their own. The rest are covered by `__monitor_*` arrays, since two
 * zero-page objects may not overlap.
 */
#ifndef C6502_APPLE2_MONITOR_H
#define C6502_APPLE2_MONITOR_H

#include <stdint.h>

/* ---- Routines -------------------------------------------------------
 * Each one takes its argument in A and returns its result in A, which
 * is c6502's zp_abi convention with a `register` parameter. */

/* Clears the text window and puts the cursor at its top left. */
__attribute__((zp_abi, address(0xFC58))) void HOME(void);

/* Waits for a key and returns it, with the high bit set. */
__attribute__((zp_abi, address(0xFD0C))) uint8_t RDKEY(void);

/* Prints `c` through the output hook CSW (normally COUT1). */
__attribute__((zp_abi, address(0xFDED))) void COUT(register uint8_t c);

/* Prints `c` on the screen, ignoring CSW. Characters $A0 and up are
 * ANDed with INVFLG first. */
__attribute__((zp_abi, address(0xFDF0))) void COUT1(register uint8_t c);

/* ---- Zero page ------------------------------------------------------ */

extern volatile uint8_t __monitor_loc[2]  __attribute__((address(0x00)));
extern volatile uint8_t __monitor_zp_1f   __attribute__((address(0x1F)));

extern volatile uint8_t WNDLFT            __attribute__((address(0x20)));
extern volatile uint8_t WNDWDTH           __attribute__((address(0x21)));
extern volatile uint8_t WNDTOP            __attribute__((address(0x22)));
extern volatile uint8_t WNDBTM            __attribute__((address(0x23)));
extern volatile uint8_t CH                __attribute__((address(0x24)));
extern volatile uint8_t CV                __attribute__((address(0x25)));
extern volatile uint16_t GBAS             __attribute__((address(0x26)));
extern volatile uint16_t BAS              __attribute__((address(0x28)));

/* $2A-$2F: BAS2 (II+), H2 / LMNEM, V2 / RMNEM, MASK, LENGTH. */
extern volatile uint8_t __monitor_zp_2a[6] __attribute__((address(0x2A)));

extern volatile uint8_t COLOR             __attribute__((address(0x30)));
extern volatile uint8_t __monitor_mode    __attribute__((address(0x31)));
extern volatile uint8_t INVFLG            __attribute__((address(0x32)));
extern volatile uint8_t PROMPT            __attribute__((address(0x33)));

/* $34-$35: YSAV, YSAV1. */
extern volatile uint8_t __monitor_ysav[2] __attribute__((address(0x34)));

extern volatile uint16_t CSW              __attribute__((address(0x36)));
extern volatile uint16_t KSW              __attribute__((address(0x38)));

/* $3A-$49: PC, A1-A5, XREG, YREG, STATUS, SPNT. */
extern volatile uint8_t __monitor_zp_3a[16] __attribute__((address(0x3A)));

#endif
