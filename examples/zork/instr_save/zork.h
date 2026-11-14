/*
    zork.h -- what the units of the instr_save example share: Zork's
    zero page, the memory it owns, and the routines outside this
    example, at the addresses they have in Zork I (r15).

    Every address comes from assembling output/main.asm of
    https://github.com/XekriRedmane/zorki-r15-reveng with dasm, which
    gives back main.bin byte for byte.
*/
#ifndef ZORK_H
#define ZORK_H

#include <stdbool.h>
#include <stdint.h>
#include <apple2/monitor_io.h>

/* ---- Zero page ------------------------------------------------------- */

/* A 24-bit unsigned integer, low byte first: the low 16 bits, then the
 * top 8. Zork's program counters are byte addresses into a story file
 * of up to 128K. */
typedef struct {
    uint16_t lo;
    uint8_t hi;
} uint24_t;

/*
    The compiler places its own zero-page bytes (the runtime's block, the
    zp_abi parameter and local slots, and globals it promotes) around
    every zero-page object the program declares. Zork uses $7C-$F2, DOS
    3.3's RWTS uses bytes in $26-$4F, and <apple2/monitor_io.h> declares
    the Monitor's. So every byte Zork or DOS owns is declared here, the
    ones this example uses by name and the rest as reservations, and the
    compiler is left $00-$1E, $50-$7B, and $F3-$FF.
*/

/* DOS 3.3's bytes in $26-$4F that monitor_io.h doesn't declare. */
extern uint8_t __dos_zp_2f [[c6502::zeropage(0x2F)]];
extern uint8_t __dos_zp_31 [[c6502::zeropage(0x31)]];
extern uint8_t __dos_zp_34 [[c6502::zeropage(0x34)]];
extern uint8_t __dos_zp_3a [[c6502::zeropage(0x3A)]][11];
extern uint8_t __dos_zp_46 [[c6502::zeropage(0x46)]][2];
extern uint8_t __dos_zp_49 [[c6502::zeropage(0x49)]][5];

/* DEBUG_JUMP, a JMP instruction, then a spare byte. */
extern uint8_t __zork_zp_7c [[c6502::zeropage(0x7C)]][3];
extern uint8_t SECTORS_PER_TRACK [[c6502::zeropage(0x7F)]];
/* CURR_OPCODE, OPERAND_COUNT, and OPERAND0-OPERAND3. */
extern uint8_t __zork_zp_80 [[c6502::zeropage(0x80)]][10];
/* The instruction reader's program counter. */
extern uint24_t Z_PC [[c6502::zeropage(0x8A)]];
/* The page cache's and the string reader's state, and GLOBAL_ZVARS_ADDR. */
extern uint8_t __zork_zp_8d [[c6502::zeropage(0x8D)]][13];
/* The current routine's 15 local variables, two bytes each. */
extern uint8_t LOCAL_ZVARS [[c6502::zeropage(0x9A)]][30];
/* HIGH_MEM_ADDR. */
extern uint8_t __zork_zp_b8 [[c6502::zeropage(0xB8)]][2];
/* The story file, loaded at a page boundary. */
extern uint8_t *Z_HEADER_ADDR [[c6502::zeropage(0xBA)]];
/* The page cache's tables. */
extern uint8_t __zork_zp_bc [[c6502::zeropage(0xBC)]][12];
/* STACK_COUNT, Z_SP, FRAME_Z_SP, and FRAME_STACK_COUNT, which a save
 * keeps together. */
extern uint8_t STACK_STATE [[c6502::zeropage(0xC8)]][6];
/* The z-string decoder's and the tokenizer's state. */
extern uint8_t __zork_zp_ce [[c6502::zeropage(0xCE)]][22];
/* Zork's scratch words. SCRATCH1 is a sector number while saving.
 * SCRATCH2 is where Zork's routines pass the address of the bytes to
 * copy, write, or print; this example passes it as an argument, but
 * Zork's other routines still use the bytes. */
extern uint16_t SCRATCH1 [[c6502::zeropage(0xE4)]];
extern uint8_t __zork_zp_e6 [[c6502::zeropage(0xE6)]][2];
/* SCRATCH3 and SCRATCH3+1, SIGN_BIT, the line buffer's state,
 * PRINTER_CSW, and TMP_Z_PC. */
extern uint8_t __zork_zp_e8 [[c6502::zeropage(0xE8)]][11];

/* ---- Memory ---------------------------------------------------------- */

/* Zork's line buffer, which a save also uses for its sectors. */
extern uint8_t BUFF_AREA [[c6502::address(0x0200)]][256];
/* The Z-machine's stack, $0280-$03E7, which grows down from $03E8. It
 * overlaps BUFF_AREA, so it's an address rather than an object: the
 * compiler takes two differently named objects to be different bytes,
 * and assumes an access through a pointer may reach any of them. */
#define Z_STACK ((uint8_t *)0x0280)

/* Offsets into the story file's header. */
#define HEADER_VERSION 0x00
/* The high byte of the address where static memory starts: the number
 * of pages of dynamic memory, which a save writes. */
#define HEADER_STATIC_MEM_BASE 0x0E

/* ---- DOS ------------------------------------------------------------- */

/* The device characteristics table for a Disk II. */
struct dct {
    uint8_t device_type;
    uint8_t phases_per_track;
    uint16_t motor_on_time_count;
};

/* The RWTS input/output block. */
struct iob {
    uint8_t table_type;             /* must be 1 */
    uint8_t slot_times_16;
    uint8_t drive_number;
    uint8_t volume;
    uint8_t track;
    uint8_t sector;
    struct dct *dct_addr;
    uint8_t *buffer_addr;
    uint8_t unused;
    uint8_t partial_byte_count;
    uint8_t command;                /* 1 = read, 2 = write */
    uint8_t ret_code;
    uint8_t last_volume;
    uint8_t last_slot_times_16;
    uint8_t last_drive_number;
};

extern struct dct dct;
extern struct iob iob;

/* DOS's disk routine. It takes the address of its parameter block
 * with the low byte in Y and the high byte in A, and sets the carry
 * when the operation failed. */
[[c6502::zp_abi, c6502::return_bool_in_carry, c6502::address(0x2900)]]
bool RWTS(struct iob *iob [[c6502::in_ya]]);

/* ---- Zork's routines outside this example ----------------------------- */

/* These are declared like ROM routines: `zp_abi`, so that the compiler
 * knows they don't take arguments on the soft stack, and so don't need
 * its zero-page window. None of them uses $F3-$FF, where the compiler
 * puts this example's own parameters and locals. */

/* Clears the screen and Zork's line count. */
[[c6502::zp_abi, c6502::address(0x1CC9)]] void home(void);
/* Prints the line buffer and a carriage return, pausing with [MORE]
 * when the screen is full. */
[[c6502::zp_abi, c6502::address(0x1C5B)]] void dump_buffer_with_more(void);
/* Prints the line buffer without a carriage return. */
[[c6502::zp_abi, c6502::address(0x1BE2)]] void dump_buffer_line(void);
/* Adds a character to the line buffer. */
[[c6502::zp_abi, c6502::address(0x1B90)]]
void buffer_char(register uint8_t c);
/* The ends of an instruction that branches: branch takes the branch
 * when its condition holds, and negated_branch when it doesn't. */
[[c6502::zp_abi, c6502::address(0x0BAA)]] void branch(void);
[[c6502::zp_abi, c6502::address(0x0BB3)]] void negated_branch(void);

/* ---- This example's routines ------------------------------------------ */

/* disk.c. inc_sector_and_write's parameter is `const`: it leaves the
 * pointer where it is, so instr_save can keep its page pointer there
 * across the call. */
[[c6502::return_bool_in_carry]] bool write_next_sector(void);
[[c6502::return_bool_in_carry]]
bool inc_sector_and_write(uint8_t *const buffer);

/* prompts.c */
void please_insert_save_diskette(void);
void please_reinsert_game_diskette(void);

/* save.c */
void instr_save(void);

#endif
