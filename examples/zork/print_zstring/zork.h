/*
    zork.h -- what the units of the print_zstring example share: Zork's
    zero page, the memory it owns, the routines outside this example,
    and each unit's routines, at the addresses they have in Zork I
    (r15).

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
extern uint8_t __dos_zp_2f __attribute__((zeropage(0x2F)));
extern uint8_t __dos_zp_31 __attribute__((zeropage(0x31)));
extern uint8_t __dos_zp_34 __attribute__((zeropage(0x34)));
extern uint8_t __dos_zp_3a[11] __attribute__((zeropage(0x3A)));
extern uint8_t __dos_zp_46[2] __attribute__((zeropage(0x46)));
extern uint8_t __dos_zp_49[5] __attribute__((zeropage(0x49)));

/* DEBUG_JUMP, a JMP instruction, then a spare byte. */
extern uint8_t __zork_zp_7c[3] __attribute__((zeropage(0x7C)));
extern uint8_t SECTORS_PER_TRACK __attribute__((zeropage(0x7F)));
/* CURR_OPCODE, OPERAND_COUNT, and OPERAND0-OPERAND3. */
extern uint8_t __zork_zp_80[10] __attribute__((zeropage(0x80)));
/* The instruction reader's program counter: low, high, and top byte. */
extern uint8_t Z_PC[3] __attribute__((zeropage(0x8A)));
/* The instruction reader's page: its address, whether it's mapped, and
 * its cache slot. */
extern uint8_t *ZCODE_PAGE_ADDR __attribute__((zeropage(0x8D)));
extern uint8_t ZCODE_PAGE_VALID __attribute__((zeropage(0x8F)));
extern uint8_t PAGE_TABLE_INDEX __attribute__((zeropage(0x90)));
/* The string reader's program counter, kept high byte first, and its
 * page. */
extern uint8_t Z_PC2_H __attribute__((zeropage(0x91)));
extern uint8_t Z_PC2_HH __attribute__((zeropage(0x92)));
extern uint8_t Z_PC2_L __attribute__((zeropage(0x93)));
extern uint8_t *ZCODE_PAGE_ADDR2 __attribute__((zeropage(0x94)));
extern uint8_t ZCODE_PAGE_VALID2 __attribute__((zeropage(0x96)));
extern uint8_t PAGE_TABLE_INDEX2 __attribute__((zeropage(0x97)));
/* GLOBAL_ZVARS_ADDR. */
extern uint8_t __zork_zp_98[2] __attribute__((zeropage(0x98)));
/* The current routine's 15 local variables, two bytes each. */
extern uint8_t __zork_zp_9a[30] __attribute__((zeropage(0x9A)));
/* Where the page cache's slots start. */
extern uint16_t HIGH_MEM_ADDR __attribute__((zeropage(0xB8)));
/* The story file, loaded at a page boundary. */
extern uint8_t *Z_HEADER_ADDR __attribute__((zeropage(0xBA)));
/* The page cache. Pages below NUM_IMAGE_PAGES were loaded at startup,
 * and the rest go through NUM_PAGE_TABLE_ENTRIES slots, kept as a list
 * ordered by use from FIRST_Z_PAGE to LAST_Z_PAGE. PAGE_L_TABLE and
 * PAGE_H_TABLE hold the story page in each slot, and NEXT_PAGE_TABLE
 * and PREV_PAGE_TABLE link the slots, with $FF at both ends. */
extern uint8_t NUM_IMAGE_PAGES __attribute__((zeropage(0xBC)));
extern uint8_t NUM_PAGE_TABLE_ENTRIES __attribute__((zeropage(0xBD)));
extern uint8_t FIRST_Z_PAGE __attribute__((zeropage(0xBE)));
extern uint8_t LAST_Z_PAGE __attribute__((zeropage(0xBF)));
extern uint8_t *PAGE_L_TABLE __attribute__((zeropage(0xC0)));
extern uint8_t *PAGE_H_TABLE __attribute__((zeropage(0xC2)));
extern uint8_t *NEXT_PAGE_TABLE __attribute__((zeropage(0xC4)));
extern uint8_t *PREV_PAGE_TABLE __attribute__((zeropage(0xC6)));
/* STACK_COUNT, Z_SP, FRAME_Z_SP, and FRAME_STACK_COUNT. */
extern uint8_t __zork_zp_c8[6] __attribute__((zeropage(0xC8)));
/* $FF when no single-character shift is pending, else the alphabet. */
extern uint8_t SHIFT_ALPHABET __attribute__((zeropage(0xCE)));
extern uint8_t LOCKED_ALPHABET __attribute__((zeropage(0xCF)));
/* Which z-character of ZCHARS comes next, 0 to 2, or $FF at the end. */
extern uint8_t ZDECOMPRESS_STATE __attribute__((zeropage(0xD0)));
extern uint16_t ZCHARS __attribute__((zeropage(0xD1)));
/* The tokenizer's state. */
extern uint8_t __zork_zp_d3[15] __attribute__((zeropage(0xD3)));
extern uint8_t *Z_ABBREV_TABLE __attribute__((zeropage(0xE2)));
/* Zork's scratch words. Zork's routines pass sector numbers in
 * SCRATCH1 and addresses in SCRATCH2. */
extern uint16_t SCRATCH1 __attribute__((zeropage(0xE4)));
extern uint16_t SCRATCH2 __attribute__((zeropage(0xE6)));
/* SCRATCH3 and SIGN_BIT. */
extern uint8_t __zork_zp_e8[3] __attribute__((zeropage(0xE8)));
/* The line buffer: its end, where the line breaks, and the screen line
 * the cursor is on. */
extern uint8_t BUFF_END __attribute__((zeropage(0xEB)));
extern uint8_t BUFF_LINE_LEN __attribute__((zeropage(0xEC)));
extern uint8_t CURR_LINE __attribute__((zeropage(0xED)));
/* The printer's output hook, swapped with the Monitor's CSW. */
extern uint16_t PRINTER_CSW __attribute__((zeropage(0xEE)));
/* TMP_Z_PC. */
extern uint8_t __zork_zp_f0[3] __attribute__((zeropage(0xF0)));

/* ---- Memory ---------------------------------------------------------- */

/* Zork's line buffer. */
extern uint8_t BUFF_AREA[256] __attribute__((address(0x0200)));

/* Slot 1's scratchpad RAM, a screen hole the printer card reads. */
extern volatile uint8_t SLOT1_SCRATCH __attribute__((address(0x0779)));

/* Byte $11 of the story file's header. Bit 0 asks for a transcript on
 * the printer. */
#define HEADER_FLAGS2 0x10

/* ---- Outside this example -------------------------------------------- */

/* Zork's entry point, which restarts the interpreter. Zork jumps there
 * when a page can't be read. It's declared like a ROM routine,
 * `zp_abi`, so that the compiler knows it takes no arguments on the
 * soft stack and so doesn't need the compiler's zero-page window. */
__attribute__((zp_abi, address(0x0800))) void restart(void);

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
__attribute__((zp_abi, return_bool_in_carry, address(0x2900)))
bool RWTS(struct iob *iob __attribute__((in_ya)));

/* ---- disk.c ---------------------------------------------------------- */

extern struct iob iob;
extern struct dct dct;
__attribute__((return_bool_in_carry))
bool do_rwts_on_sector(register uint8_t command);
__attribute__((return_bool_in_carry)) bool read_from_sector(void);

/* ---- pages.c --------------------------------------------------------- */

struct page_slot { uint8_t slot; bool not_found; };

void invalidate_zcode_page2(void);
void load_packed_address(void);
void get_next_code_word(void);
uint8_t get_next_code_byte2(void);
void set_page_first(register uint8_t slot);
__attribute__((return_bool_in_carry))
struct page_slot find_index_of_page_table(void);

/* ---- output.c -------------------------------------------------------- */

void buffer_char(register uint8_t c);
void dump_buffer_line(void);
void dump_buffer_to_printer(void);
void dump_buffer_to_screen(void);
void dump_buffer_with_more(void);
void cout_string(const uint8_t *s __attribute__((zeropage(0xE6))),
                 uint8_t count __attribute__((in_x)));

/* ---- zstring.c ------------------------------------------------------- */

struct zchar { uint8_t code; bool end; };

void print_zstring(void);
uint8_t A_mod_3(register uint8_t a);
uint8_t get_alphabet(void);
__attribute__((return_bool_in_carry)) struct zchar get_next_zchar(void);
void print_string_literal(void);

#endif
