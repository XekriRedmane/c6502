/*
    zork.h -- what the units of the Zork interpreter share: the
    interpreter's zero page, the memory it owns, DOS's disk routine, and
    each unit's routines.

    The interpreter is output/main.asm of
    https://github.com/XekriRedmane/zorki-r15-reveng, the Z-machine
    interpreter of Zork I (r15) for the Apple II, rewritten in C. The
    Monitor's routines and zero page come from <apple2/monitor_io.h>,
    DOS 3.3's RWTS is at $2900 where Zork's boot loader leaves it, and
    the line buffer is the Monitor's input buffer at $0200. The
    interpreter's own zero page is at Zork's addresses, so that a save
    file holds the same bytes. The exception is Z_PC2, whose bytes are
    in another order; see its declaration. The code, its data, and the
    page tables go wherever the linker script puts them.
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
    every zero-page object the program declares. DOS 3.3's RWTS uses
    bytes in $26-$4F, and <apple2/monitor_io.h> declares the Monitor's,
    so the bytes DOS uses and the Monitor's header doesn't are declared
    here as reservations.
*/

/* DOS 3.3's bytes in $26-$4F that monitor_io.h doesn't declare. */
extern uint8_t __dos_zp_2f [[c6502::zeropage(0x2F)]];
extern uint8_t __dos_zp_31 [[c6502::zeropage(0x31)]];
extern uint8_t __dos_zp_34 [[c6502::zeropage(0x34)]];
extern uint8_t __dos_zp_3a [[c6502::zeropage(0x3A)]][11];
extern uint8_t __dos_zp_46 [[c6502::zeropage(0x46)]][2];
extern uint8_t __dos_zp_49 [[c6502::zeropage(0x49)]][5];

/* Zork's boot loader sets this to 16 before it jumps to `main`. */
extern uint8_t SECTORS_PER_TRACK [[c6502::zeropage(0x7F)]];

/* The instruction being run: its opcode, how many operands it has,
 * and their values. */
extern uint8_t CURR_OPCODE [[c6502::zeropage(0x80)]];
extern uint8_t OPERAND_COUNT [[c6502::zeropage(0x81)]];
extern uint16_t OPERANDS [[c6502::zeropage(0x82)]][4];

/* The instruction reader's program counter. */
extern uint24_t Z_PC [[c6502::zeropage(0x8A)]];
/* The instruction reader's page: its address, whether it's mapped, and
 * its cache slot. */
extern uint8_t *ZCODE_PAGE_ADDR [[c6502::zeropage(0x8D)]];
extern uint8_t ZCODE_PAGE_VALID [[c6502::zeropage(0x8F)]];
extern uint8_t PAGE_TABLE_INDEX [[c6502::zeropage(0x90)]];
/* The string reader's program counter, and its page. Zork keeps
 * Z_PC2 high byte first, $91 the high byte, $92 the top byte, and $93
 * the low byte. Here it's a uint24_t like Z_PC, low byte first. A save
 * file doesn't hold Z_PC2. */
extern uint24_t Z_PC2 [[c6502::zeropage(0x91)]];
extern uint8_t *ZCODE_PAGE_ADDR2 [[c6502::zeropage(0x94)]];
extern uint8_t ZCODE_PAGE_VALID2 [[c6502::zeropage(0x96)]];
extern uint8_t PAGE_TABLE_INDEX2 [[c6502::zeropage(0x97)]];

/* The story's global variables, 240 big-endian words. */
extern uint8_t *GLOBAL_ZVARS_ADDR [[c6502::zeropage(0x98)]];
/* The current routine's 15 local variables, big-endian words. A save
 * writes these 30 bytes. */
extern uint8_t LOCAL_ZVARS [[c6502::zeropage(0x9A)]][30];

/* Where the page cache's slots start, a page boundary. */
extern uint8_t *HIGH_MEM_ADDR [[c6502::zeropage(0xB8)]];
/* The story file, loaded at a page boundary. */
extern uint8_t *Z_HEADER_ADDR [[c6502::zeropage(0xBA)]];
/* The page cache. Pages below NUM_IMAGE_PAGES were loaded at startup,
 * and the rest go through NUM_PAGE_TABLE_ENTRIES slots, kept as a list
 * ordered by use from FIRST_Z_PAGE to LAST_Z_PAGE. PAGE_L_TABLE and
 * PAGE_H_TABLE hold the story page in each slot, and NEXT_PAGE_TABLE
 * and PREV_PAGE_TABLE link the slots, with $FF at both ends. */
extern uint8_t NUM_IMAGE_PAGES [[c6502::zeropage(0xBC)]];
extern uint8_t NUM_PAGE_TABLE_ENTRIES [[c6502::zeropage(0xBD)]];
extern uint8_t FIRST_Z_PAGE [[c6502::zeropage(0xBE)]];
extern uint8_t LAST_Z_PAGE [[c6502::zeropage(0xBF)]];
extern uint8_t *PAGE_L_TABLE [[c6502::zeropage(0xC0)]];
extern uint8_t *PAGE_H_TABLE [[c6502::zeropage(0xC2)]];
extern uint8_t *NEXT_PAGE_TABLE [[c6502::zeropage(0xC4)]];
extern uint8_t *PREV_PAGE_TABLE [[c6502::zeropage(0xC6)]];

/* The Z-machine's stack. STACK_COUNT is one more than the number of
 * words on it, Z_SP points at the top word, and FRAME_Z_SP and
 * FRAME_STACK_COUNT are Z_SP and STACK_COUNT as the current routine
 * started. A save writes these six bytes together, from STACK_STATE. */
extern uint8_t STACK_COUNT [[c6502::zeropage(0xC8)]];
extern uint8_t *Z_SP [[c6502::zeropage(0xC9)]];
extern uint8_t *FRAME_Z_SP [[c6502::zeropage(0xCB)]];
extern uint8_t FRAME_STACK_COUNT [[c6502::zeropage(0xCD)]];
/* STACK_STATE is an address. The compiler takes an access through a
 * pointer to reach any object, and one through a named object to
 * reach only that object, and the save code copies the six bytes
 * through a pointer. */
#define STACK_STATE ((uint8_t *)0x00C8)

/* The z-string decoder. SHIFT_ALPHABET is $FF when no
 * single-character shift is pending, else the alphabet.
 * ZDECOMPRESS_STATE says which z-character of ZCHARS comes next, 0 to
 * 2, or $FF at the end. */
extern uint8_t SHIFT_ALPHABET [[c6502::zeropage(0xCE)]];
extern uint8_t LOCKED_ALPHABET [[c6502::zeropage(0xCF)]];
extern uint8_t ZDECOMPRESS_STATE [[c6502::zeropage(0xD0)]];
extern uint16_t ZCHARS [[c6502::zeropage(0xD1)]];

/* The tokenizer. ZCHAR_SCRATCH1 holds a word of input, up to six
 * characters padded with zeros, and a seventh byte, always $FF, which
 * ascii_to_zchar reads when it looks one character ahead of the sixth.
 * ZCHAR_SCRATCH2 holds the word as six z-characters, and then as the
 * two 16-bit words they pack into, each low byte first. */
extern uint8_t ZCHAR_SCRATCH1 [[c6502::zeropage(0xD3)]][7];
extern uint8_t ZCHAR_SCRATCH2 [[c6502::zeropage(0xDA)]][6];

/* The story's abbreviation table. */
extern uint8_t *Z_ABBREV_TABLE [[c6502::zeropage(0xE2)]];

/* The line buffer: its end, where the line breaks, and the screen line
 * the cursor is on. */
extern uint8_t BUFF_END [[c6502::zeropage(0xEB)]];
extern uint8_t BUFF_LINE_LEN [[c6502::zeropage(0xEC)]];
extern uint8_t CURR_LINE [[c6502::zeropage(0xED)]];
/* The printer's output hook, swapped with the Monitor's CSW. */
extern uint16_t PRINTER_CSW [[c6502::zeropage(0xEE)]];

/* ---- Memory ---------------------------------------------------------- */

/* Zork's line buffer, the Monitor's input buffer. A save also uses it
 * for its sectors. */
extern uint8_t BUFF_AREA [[c6502::address(0x0200)]][256];
/* The Z-machine's stack, $0280-$03E7, which grows down from $03E8. It
 * overlaps BUFF_AREA, so it's an address, for the reason STACK_STATE
 * is. */
#define Z_STACK ((uint8_t *)0x0280)
#define Z_STACK_TOP ((uint8_t *)0x03E8)

/* Slot 1's scratchpad RAM, a screen hole the printer card reads. */
extern volatile uint8_t SLOT1_SCRATCH [[c6502::address(0x0779)]];

/* The page cache's four tables, 128 bytes each, which the linker
 * script places. */
extern uint8_t page_tables[512];
/* The first byte after the program, a page boundary, where the story
 * file is loaded. The linker script defines it. */
extern uint8_t __story[];

/* Offsets into the story file's header. Each word is big-endian. */
#define HEADER_VERSION 0x00
#define HEADER_FLAGS1 0x01
#define HEADER_HIMEM_BASE 0x04
#define HEADER_INITIAL_ZPC 0x06
#define HEADER_DICT_ADDR 0x08
#define HEADER_OBJECT_TABLE_ADDR 0x0A
#define HEADER_GLOBALVARS_ADDR 0x0C
/* The high byte of the address where static memory starts: the number
 * of pages of dynamic memory, which a save writes. */
#define HEADER_STATIC_MEM_BASE 0x0E
/* Bit 0 of byte $11 turns the transcript on the printer on. */
#define HEADER_FLAGS2 0x10
#define HEADER_ABBREVS_ADDR 0x18

/* An object's entry: 32 attribute bits, then its parent, sibling, and
 * child, then the address of its properties. */
#define FIRST_OBJECT_OFFSET 0x35
#define OBJECT_PARENT_OFFSET 0x04
#define OBJECT_SIBLING_OFFSET 0x05
#define OBJECT_CHILD_OFFSET 0x06
#define OBJECT_PROPS_OFFSET 0x07

/* The global variables the status line shows. */
#define VAR_CURR_ROOM 0x10
#define VAR_SCORE 0x11
#define VAR_MAX_SCORE 0x12

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

/* DOS's disk routine. It takes the address of its parameter block
 * with the low byte in Y and the high byte in A, and sets the carry
 * when the operation failed. */
[[c6502::zp_abi, c6502::return_bool_in_carry, c6502::address(0x2900)]]
bool RWTS(struct iob *iob [[c6502::in_ya]]);

/* ---- main.c ---------------------------------------------------------- */

/* `main` is at $0800, where Zork's boot loader jumps, and Zork goes
 * back there to start over. restart() is a jump to it, declared like a
 * ROM routine so that the compiler's call graph has no cycle through
 * `main`. It
 * doesn't return, and neither do the other three, which end in
 * restart() or an endless loop. */
[[noreturn, c6502::zp_abi, c6502::address(0x0800)]] void restart(void);
[[noreturn]] void internal_error(void);
[[noreturn]] void instr_restart(void);
[[noreturn]] void instr_quit(void);

/* ---- decode.c -------------------------------------------------------- */

void do_instruction(void);
uint16_t var_get(register uint8_t var);
void var_put(register uint8_t var, uint16_t value);
void store_result(uint16_t value);
void push(uint16_t value);
uint16_t pop(void);
void branch(register bool condition);
void return_value(uint16_t value);
uint8_t get_next_code_byte(void);

/* ---- objects.c ------------------------------------------------------- */

void print_obj(register uint8_t obj);
void instr_get_sibling(void);
void instr_get_child(void);
void instr_get_parent(void);
void instr_get_prop_len(void);
void instr_remove_obj(void);
void instr_jin(void);
void instr_test_attr(void);
void instr_set_attr(void);
void instr_clear_attr(void);
void instr_insert_obj(void);
void instr_get_prop(void);
void instr_get_prop_addr(void);
void instr_get_next_prop(void);
void instr_put_prop(void);

/* ---- numbers.c ------------------------------------------------------- */

void print_number(uint16_t n);
void instr_mul(void);
void instr_div(void);
void instr_mod(void);
void instr_random(void);

/* ---- input.c --------------------------------------------------------- */

void print_status_line(void);
void instr_sread(void);

/* ---- pages.c --------------------------------------------------------- */

void load_address(uint16_t addr);
void load_packed_address(uint16_t addr);
uint16_t get_next_code_word(void);
uint8_t get_next_code_byte2(void);

/* ---- zstring.c ------------------------------------------------------- */

struct zchar { uint8_t code; bool end; };

extern const uint8_t a2_table[24];
void print_zstring(void);
uint8_t A_mod_3(register uint8_t a);
uint8_t get_alphabet(void);
[[c6502::return_bool_in_carry]] struct zchar get_next_zchar(void);
void print_string_literal(void);

/* ---- output.c -------------------------------------------------------- */

void buffer_char(register uint8_t c);
void new_line(void);
void dump_buffer_line(void);
void dump_buffer_to_printer(void);
void dump_buffer_to_screen(void);
void dump_buffer_with_more(void);
void cout_string(const char *s, uint8_t count [[c6502::in_x]]);
void print_ascii_string(const char *s, uint8_t count [[c6502::in_x]]);
void home(void);
void reset_window(void);

/* ---- disk.c ---------------------------------------------------------- */

extern struct iob iob;
extern struct dct dct;

/* The sector the disk routines read or write next, counted from the
 * start of track 3, and the 256 bytes they read it into or write it
 * from. They stand in for Zork's SCRATCH1 and SCRATCH2. Neither
 * routine changes disk_buffer, and only the inc_sector_and_* routines
 * change disk_sector, so the save code sets them once and steps them
 * from sector to sector. */
extern uint16_t disk_sector [[c6502::zeropage]];
extern uint8_t *disk_buffer [[c6502::zeropage]];

/* Each returns true if the read or write failed. read_next_sector and
 * write_next_sector use BUFF_AREA as the buffer, and they and the
 * inc_sector_and_* routines step disk_sector before reading or
 * writing. */
bool read_from_sector(void);
bool inc_sector_and_read(void);
bool read_next_sector(void);
bool write_to_sector(void);
bool inc_sector_and_write(void);
bool write_next_sector(void);

/* ---- prompts.c ------------------------------------------------------- */

uint16_t please_insert_save_diskette(void);
void please_reinsert_game_diskette(void);

/* ---- save.c ---------------------------------------------------------- */

void instr_save(void);
void instr_restore(void);

#endif
