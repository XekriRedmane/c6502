/* The Apple II Monitor ROM's interactive part: the zero-page bytes it
 * uses beyond the ones <apple2/monitor_io.h> reserves.
 *
 * Include this header, in addition to <apple2/monitor_io.h>, in a program
 * that runs the Monitor's command loop or its memory tools, calls the
 * disassembler or the cassette routines, or stops at a BRK and then
 * continues. The bytes come from the unenhanced Apple IIe F8 ROM
 * listing at https://6502disassembly.com/a2-rom/Unenh_IIe_F8ROM.html:
 *
 *   $00-$01    LOC0, LOC1, the power-up slot scan
 *   $2F        LENGTH (disassembler), LASTIN (cassette read)
 *   $31        MODE, command loop
 *   $34        YSAV, command loop
 *   $3A-$3B    PCL, PCH: disassembler, LIST, GO, BREAK
 *   $3C-$44    A1-A4 and A5L: memory examine, move, verify, store,
 *              number entry, cassette
 *   $46-$47    XREG, YREG, register save and display
 *   $49        SPNT, register save and display
 *
 * The disassembler also uses $2C-$2E, which it shares with the lo-res
 * graphics routines, and the register display reads $45 and $48,
 * which the IRQ entry and INIT also write. <apple2/monitor_io.h> already
 * reserves those.
 */
#ifndef C6502_APPLE2_INTERACTIVE_MONITOR_H
#define C6502_APPLE2_INTERACTIVE_MONITOR_H

#include <apple2/monitor_io.h>

extern volatile uint8_t __monitor_loc      [[c6502::address(0x00)]][2];
extern volatile uint8_t __monitor_length   [[c6502::address(0x2F)]];
extern volatile uint8_t __monitor_mode     [[c6502::address(0x31)]];
extern volatile uint8_t __monitor_ysav     [[c6502::address(0x34)]];
extern volatile uint16_t __monitor_pc      [[c6502::address(0x3A)]];
extern volatile uint8_t __monitor_a1_a5l   [[c6502::address(0x3C)]][9];
extern volatile uint8_t __monitor_xyreg    [[c6502::address(0x46)]][2];
extern volatile uint8_t __monitor_spnt     [[c6502::address(0x49)]];

#endif
