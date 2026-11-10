#include <stdbool.h>
#include <stdint.h>
#include <apple2/monitor_io.h>

/*
    print_string_literal and every routine it reaches, from the Zork I
    (r15) Z-machine interpreter for the Apple II, as reverse-engineered
    in https://github.com/XekriRedmane/zorki-r15-reveng. In that
    repository's output/main.asm, print_string_literal is at line 908,
    the page cache and z-string decoder at lines 2431-2815, the line
    buffer at lines 3050-3240, and the disk routines at lines
    3387-3430.

    The Z-machine stores text as z-strings. Each 16-bit word holds
    three 5-bit z-characters, and the top bit of the last word marks
    the end of the string. A z-character picks a letter from one of
    three alphabets, shifts to another alphabet for one character or
    until further notice, or names an abbreviation, which is itself a
    z-string, printed in place. print_string_literal prints the
    z-string that follows the current instruction, at the program
    counter Z_PC, and leaves Z_PC just past it.

    The story file is read in 256-byte pages. Pages below
    NUM_IMAGE_PAGES were loaded at startup, and the rest go through a
    cache of pages in high memory, kept as a list ordered by use
    (PAGE_L_TABLE / PAGE_H_TABLE hold the page number in each slot,
    and NEXT_PAGE_TABLE / PREV_PAGE_TABLE link the slots). The string
    reader keeps its own copy of the program counter, Z_PC2, and its
    own page pointer, ZCODE_PAGE_ADDR2, so that reading a string
    doesn't disturb the instruction reader.

    Printed characters go through Zork's line buffer, BUFF_AREA, which
    breaks lines between words and pauses with [MORE] when the screen
    fills.

    Departures from the original:

    - get_next_zchar returns its z-character in A and "end of string"
      in the carry, and find_index_of_page_table returns a slot in A
      and "not found" in the carry. Each returns a struct of the value
      and a bool, declared return_bool_in_carry, which uses the same
      registers.
    - When a page can't be read, the original jumps to `main`, which
      restarts the interpreter. Here that's a call to restart(), which
      doesn't return.
    - The original passes some values through its scratch words
      SCRATCH2 and SCRATCH3 where this version uses locals, in
      set_page_first and print_zstring. Everything that reads SCRATCH1
      or SCRATCH2 after a call still finds the value the original
      leaves there.
    - buffer_char_set_buffer_end, a label inside buffer_char that
      dump_buffer_with_more jumps to with X = 0, is the assignment
      BUFF_END = 0.
*/

/* ---- Zork's variables ------------------------------------------------ */

extern uint8_t SECTORS_PER_TRACK __attribute__((zeropage(0x7F)));
/* The instruction reader's program counter: low, high, and top byte. */
extern uint8_t Z_PC[3] __attribute__((zeropage(0x8A)));
extern uint8_t *ZCODE_PAGE_ADDR __attribute__((zeropage(0x8D)));
extern uint8_t ZCODE_PAGE_VALID __attribute__((zeropage(0x8F)));
extern uint8_t PAGE_TABLE_INDEX __attribute__((zeropage(0x90)));
/* The string reader's program counter, kept high byte first. */
extern uint8_t Z_PC2_H __attribute__((zeropage(0x91)));
extern uint8_t Z_PC2_HH __attribute__((zeropage(0x92)));
extern uint8_t Z_PC2_L __attribute__((zeropage(0x93)));
extern uint8_t *ZCODE_PAGE_ADDR2 __attribute__((zeropage(0x94)));
extern uint8_t ZCODE_PAGE_VALID2 __attribute__((zeropage(0x96)));
extern uint8_t PAGE_TABLE_INDEX2 __attribute__((zeropage(0x97)));
extern uint16_t HIGH_MEM_ADDR __attribute__((zeropage(0xB8)));
extern uint8_t *Z_HEADER_ADDR __attribute__((zeropage(0xBA)));
extern uint8_t NUM_IMAGE_PAGES __attribute__((zeropage(0xBC)));
extern uint8_t NUM_PAGE_TABLE_ENTRIES __attribute__((zeropage(0xBD)));
extern uint8_t FIRST_Z_PAGE __attribute__((zeropage(0xBE)));
extern uint8_t LAST_Z_PAGE __attribute__((zeropage(0xBF)));
extern uint8_t *PAGE_L_TABLE __attribute__((zeropage(0xC0)));
extern uint8_t *PAGE_H_TABLE __attribute__((zeropage(0xC2)));
extern uint8_t *NEXT_PAGE_TABLE __attribute__((zeropage(0xC4)));
extern uint8_t *PREV_PAGE_TABLE __attribute__((zeropage(0xC6)));
/* $FF when no single-character shift is pending, else the alphabet. */
extern uint8_t SHIFT_ALPHABET __attribute__((zeropage(0xCE)));
extern uint8_t LOCKED_ALPHABET __attribute__((zeropage(0xCF)));
/* Which z-character of ZCHARS comes next, 0 to 2, or $FF at the end. */
extern uint8_t ZDECOMPRESS_STATE __attribute__((zeropage(0xD0)));
extern uint16_t ZCHARS __attribute__((zeropage(0xD1)));
extern uint8_t *Z_ABBREV_TABLE __attribute__((zeropage(0xE2)));
extern uint16_t SCRATCH1 __attribute__((zeropage(0xE4)));
extern uint16_t SCRATCH2 __attribute__((zeropage(0xE6)));
extern uint8_t BUFF_END __attribute__((zeropage(0xEB)));
extern uint8_t BUFF_LINE_LEN __attribute__((zeropage(0xEC)));
extern uint8_t CURR_LINE __attribute__((zeropage(0xED)));
extern uint16_t PRINTER_CSW __attribute__((zeropage(0xEE)));
extern uint8_t BUFF_AREA[256] __attribute__((address(0x0200)));

/* Slot 1's scratchpad RAM, a screen hole. */
extern volatile uint8_t SLOT1_SCRATCH __attribute__((address(0x0779)));

/* Byte $11 of the story file's header. Bit 0 asks for a transcript on
 * the printer. */
#define HEADER_FLAGS2 0x10

/* ---- Routines outside this file ------------------------------------- */

/* Restarts the interpreter. It doesn't return. */
void restart(void);

/* DOS's disk routine. It takes the address of its parameter block
 * with the high byte in A and the low byte in Y, and sets the carry
 * when the operation failed. */
__attribute__((zp_abi, return_bool_in_carry, address(0x2900)))
bool RWTS(register uint8_t iob_hi __attribute__((in_a)),
          register uint8_t iob_lo __attribute__((in_y)));

/* ---- Disk ------------------------------------------------------------ */

/* The device characteristics table for a Disk II. */
struct dct {
    uint8_t device_type;
    uint8_t phases_per_track;
    uint16_t motor_on_time_count;
};

struct dct dct = { 0x00, 0x01, 0xD8EF };

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

struct iob iob = {
    0x01, 0x60, 0x01, 0x00, 0x00, 0x00, &dct, 0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x60, 0x01,
};

/* Reads or writes (`command`) the 256-byte sector numbered SCRATCH1,
 * counting from the start of track 3, to or from the buffer at
 * SCRATCH2. Returns true if the operation failed. */
__attribute__((return_bool_in_carry))
bool do_rwts_on_sector(register uint8_t command) {
    iob.command = command;
    iob.buffer_addr = (uint8_t *)SCRATCH2;
    iob.track = 3;
    uint16_t sector = SCRATCH1;
    while (sector >= SECTORS_PER_TRACK) {
        sector -= SECTORS_PER_TRACK;
        iob.track++;
    }
    iob.sector = (uint8_t)sector;
    return RWTS((uint8_t)((uint16_t)&iob >> 8), (uint8_t)(uint16_t)&iob);
}

__attribute__((return_bool_in_carry)) bool read_from_sector(void) {
    return do_rwts_on_sector(1);
}

/* ---- The page cache -------------------------------------------------- */

struct page_slot { uint8_t slot; bool not_found; };

/* Finds the cache slot holding story page SCRATCH2. If no slot holds
 * it, returns the least recently used slot, LAST_Z_PAGE, with
 * not_found set. */
__attribute__((return_bool_in_carry))
struct page_slot find_index_of_page_table(void) {
    struct page_slot r;
    uint8_t lo = (uint8_t)SCRATCH2;
    uint8_t hi = (uint8_t)(SCRATCH2 >> 8);
    uint8_t i = 0;
    uint8_t n = NUM_PAGE_TABLE_ENTRIES;
    do {
        if (PAGE_L_TABLE[i] == lo && PAGE_H_TABLE[i] == hi) {
            r.slot = i;
            r.not_found = false;
            return r;
        }
        i++;
    } while (--n != 0);
    r.slot = LAST_Z_PAGE;
    r.not_found = true;
    return r;
}

/* Moves cache slot `slot` to the front of the list ordered by use.
 * $FF ends the list in both directions. */
void set_page_first(register uint8_t slot) {
    if (slot == FIRST_Z_PAGE)
        return;
    uint8_t prev_first = FIRST_Z_PAGE;
    FIRST_Z_PAGE = slot;
    uint8_t next = NEXT_PAGE_TABLE[slot];
    NEXT_PAGE_TABLE[slot] = prev_first;
    uint8_t prev = PREV_PAGE_TABLE[slot];
    PREV_PAGE_TABLE[slot] = 0xFF;
    NEXT_PAGE_TABLE[prev] = next;
    PREV_PAGE_TABLE[prev_first] = FIRST_Z_PAGE;
    if (next == 0xFF)
        LAST_Z_PAGE = prev;
    else
        PREV_PAGE_TABLE[next] = prev;
}

/* ---- Reading the story file ----------------------------------------- */

void invalidate_zcode_page2(void) {
    ZCODE_PAGE_VALID2 = 0;
}

/* Returns the byte at Z_PC2 and advances Z_PC2. When Z_PC2 crosses
 * into a page that isn't mapped, maps it first, reading it from disk
 * into the least recently used cache slot if it isn't cached. */
uint8_t get_next_code_byte2(void) {
    for (;;) {
        if (ZCODE_PAGE_VALID2) {
            uint8_t b = ZCODE_PAGE_ADDR2[Z_PC2_L];
            if (++Z_PC2_L == 0) {
                ZCODE_PAGE_VALID2 = 0;
                if (++Z_PC2_H == 0)
                    Z_PC2_HH++;
            }
            return b;
        }
        uint8_t page;
        if (Z_PC2_HH == 0 && Z_PC2_H < NUM_IMAGE_PAGES) {
            page = Z_PC2_H;
        } else {
            SCRATCH2 = Z_PC2_H | ((uint16_t)Z_PC2_HH << 8);
            struct page_slot found = find_index_of_page_table();
            PAGE_TABLE_INDEX2 = found.slot;
            if (found.not_found) {
                /* The instruction reader's page is about to go. */
                if (found.slot == PAGE_TABLE_INDEX)
                    ZCODE_PAGE_VALID = 0;
                SCRATCH2 = HIGH_MEM_ADDR
                    + ((uint16_t)PAGE_TABLE_INDEX2 << 8);
                SCRATCH1 = Z_PC2_H | ((uint16_t)Z_PC2_HH << 8);
                if (read_from_sector())
                    restart();
                PAGE_L_TABLE[PAGE_TABLE_INDEX2] = Z_PC2_H;
                PAGE_H_TABLE[PAGE_TABLE_INDEX2] = Z_PC2_HH;
            }
            set_page_first(PAGE_TABLE_INDEX2);
            page = PAGE_TABLE_INDEX2 + NUM_IMAGE_PAGES;
        }
        uint8_t high = page + (uint8_t)((uint16_t)Z_HEADER_ADDR >> 8);
        ZCODE_PAGE_ADDR2 = (uint8_t *)((uint16_t)high << 8);
        ZCODE_PAGE_VALID2 = 0xFF;
    }
}

/* Reads the big-endian word at Z_PC2 into SCRATCH2. */
void get_next_code_word(void) {
    uint8_t hi = get_next_code_byte2();
    uint8_t lo = get_next_code_byte2();
    SCRATCH2 = ((uint16_t)hi << 8) | lo;
}

/* Sets Z_PC2 to the byte address of the packed (word) address in
 * SCRATCH2, and unmaps the string reader's page. */
void load_packed_address(void) {
    Z_PC2_L = (uint8_t)(SCRATCH2 << 1);
    Z_PC2_H = (uint8_t)(SCRATCH2 >> 7);
    Z_PC2_HH = (uint8_t)(SCRATCH2 >> 15);
    invalidate_zcode_page2();
}

/* ---- Output ---------------------------------------------------------- */

static uint8_t printer_card_initialized_flag;

void dump_buffer_to_screen(void) {
    for (uint8_t x = 0; x != BUFF_END; x++)
        COUT1(BUFF_AREA[x]);
    BUFF_END = 0;
}

void dump_buffer_to_printer(void) {
    uint16_t saved_csw = CSW;
    CSW = PRINTER_CSW;
    if (!printer_card_initialized_flag) {
        printer_card_initialized_flag = 1;
        COUT(0x09);                     /* ctrl-I */
        SLOT1_SCRATCH = 0x91;
        COUT(0xB8);                     /* '8' */
        COUT(0xB0);                     /* '0' */
        COUT(0xCE);                     /* 'N' */
    }
    for (uint8_t x = 0; x != BUFF_END; x++)
        COUT(BUFF_AREA[x]);
    PRINTER_CSW = CSW;
    CSW = saved_csw;
}

/* Prints the buffered line on the screen, and on the printer too when
 * the story has asked for a transcript. */
void dump_buffer_line(void) {
    if (Z_HEADER_ADDR[HEADER_FLAGS2 + 1] & 0x01)
        dump_buffer_to_printer();
    dump_buffer_to_screen();
}

/* Prints `count` characters from SCRATCH2 with COUT1, in normal text
 * unless INVFLG says otherwise. */
void cout_string(register uint8_t count __attribute__((in_x))) {
    uint8_t *s = (uint8_t *)SCRATCH2;
    uint8_t y = 0;
    do {
        COUT1(s[y] | 0x80);
        y++;
    } while (--count != 0);
}

static uint8_t string_more[6] = { '[', 'M', 'O', 'R', 'E', ']' };

/* Prints the buffered line and ends it. When the window is full, first
 * shows [MORE] in inverse and waits for a key. */
void dump_buffer_with_more(void) {
    if (++CURR_LINE >= WNDBTM) {
        SCRATCH2 = (uint16_t)string_more;
        INVFLG = 0x3F;
        cout_string(6);
        INVFLG = 0xFF;
        RDKEY();
        CH -= 6;
        CLREOL();
        CURR_LINE = WNDTOP + 1;
    }
    uint8_t end = BUFF_END;
    dump_buffer_line();
    /* A full line has already wrapped. */
    if (end != WNDWDTH)
        COUT1(0x8D);
    if (Z_HEADER_ADDR[HEADER_FLAGS2 + 1] & 0x01) {
        uint16_t saved_csw = CSW;
        CSW = PRINTER_CSW;
        COUT(0x8D);
        PRINTER_CSW = CSW;
        CSW = saved_csw;
    }
    BUFF_END = 0;
}

/* Adds `c` to the line buffer. A carriage return ends the line, and
 * other control characters are dropped. Lower case becomes upper
 * case. When the line reaches the window's width, the line is printed
 * up to its last space, and the word after it moves to the start of
 * the next line. */
void buffer_char(register uint8_t c) {
    uint8_t x = BUFF_END;
    if (c == 0x0D) {
        dump_buffer_with_more();
        return;
    }
    if (c < 0x20)
        return;
    if (c >= 0x60 && c < 0x80)
        c -= 0x20;
    BUFF_AREA[x] = c | 0x80;
    if (x < WNDWDTH) {
        BUFF_END = x + 1;
        return;
    }
    /* Find the last space. With none, break at the window's width. */
    for (;;) {
        if (BUFF_AREA[x] == 0xA0)
            break;
        if (--x == 0) {
            x = WNDWDTH;
            break;
        }
    }
    BUFF_LINE_LEN = x;
    BUFF_END = x;
    dump_buffer_with_more();
    /* Move what followed the break to the start of the buffer. */
    for (;;) {
        uint8_t from = ++BUFF_LINE_LEN;
        if (from > WNDWDTH)
            return;
        BUFF_AREA[BUFF_END++] = BUFF_AREA[from];
    }
}

/* ---- Z-strings ------------------------------------------------------- */

struct zchar { uint8_t code; bool end; };

/* Returns the next z-character of the string at Z_PC2, or `end` once
 * the word with the end bit has been used up. */
__attribute__((return_bool_in_carry)) struct zchar get_next_zchar(void) {
    struct zchar r;
    uint8_t state = ZDECOMPRESS_STATE;
    if (state & 0x80) {
        r.code = state;
        r.end = true;
        return r;
    }
    if (state == 0) {
        ZDECOMPRESS_STATE = 1;
        get_next_code_word();
        ZCHARS = SCRATCH2;
        r.code = (uint8_t)(ZCHARS >> 10) & 0x1F;
    } else if (state == 1) {
        ZDECOMPRESS_STATE = 2;
        r.code = (uint8_t)(ZCHARS >> 5) & 0x1F;
    } else {
        ZDECOMPRESS_STATE = (ZCHARS & 0x8000) ? 0xFF : 0x00;
        r.code = (uint8_t)ZCHARS & 0x1F;
    }
    r.end = false;
    return r;
}

uint8_t A_mod_3(register uint8_t a) {
    while (a >= 3)
        a -= 3;
    return a;
}

/* Returns the alphabet for the next character: the shifted one, which
 * is then used up, or else the locked one. */
uint8_t get_alphabet(void) {
    uint8_t shift = SHIFT_ALPHABET;
    if (shift & 0x80)
        return LOCKED_ALPHABET;
    SHIFT_ALPHABET = 0xFF;
    return shift;
}

/* Alphabet 2's characters for z-characters 8 to 31. */
static const uint8_t a2_table[24] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',',
    '!', '?', '_', '#', '\'', '"', '/', '\\', '-', ':', '(', ')',
};

/* Prints the z-string at Z_PC2. An abbreviation prints another
 * z-string in the middle of this one, so this calls itself. */
void print_zstring(void) {
    LOCKED_ALPHABET = 0;
    ZDECOMPRESS_STATE = 0;
    SHIFT_ALPHABET = 0xFF;
    for (;;) {
        struct zchar c = get_next_zchar();
        if (c.end)
            return;
        uint8_t z = c.code;
        uint8_t ch;
        if (z == 0) {
            ch = ' ';
        } else if (z == 1) {
            /* An abbreviation: the table holds its packed address. */
            uint8_t i = get_next_zchar().code << 1;
            SCRATCH2 = ((uint16_t)Z_ABBREV_TABLE[i] << 8)
                | Z_ABBREV_TABLE[i + 1];
            uint8_t locked = LOCKED_ALPHABET;
            uint8_t state = ZDECOMPRESS_STATE;
            uint16_t zchars = ZCHARS;
            uint8_t pc_l = Z_PC2_L;
            uint8_t pc_h = Z_PC2_H;
            uint8_t pc_hh = Z_PC2_HH;
            load_packed_address();
            print_zstring();
            Z_PC2_HH = pc_hh;
            Z_PC2_H = pc_h;
            Z_PC2_L = pc_l;
            ZCODE_PAGE_VALID2 = 0;
            ZCHARS = zchars;
            ZDECOMPRESS_STATE = state;
            LOCKED_ALPHABET = locked;
            SHIFT_ALPHABET = 0xFF;
            continue;
        } else if (z < 4) {
            SHIFT_ALPHABET = A_mod_3(get_alphabet() + 2 + z);
            continue;
        } else if (z < 6) {
            LOCKED_ALPHABET = A_mod_3(get_alphabet() + z);
            continue;
        } else {
            uint8_t alphabet = get_alphabet();
            if (alphabet == 0) {
                ch = 0x5B + z;                  /* 'a' for z = 6 */
            } else if (alphabet == 1) {
                ch = 0x3B + z;                  /* 'A' for z = 6 */
            } else if (z == 6) {
                /* A 10-bit character code in the next two z-chars. */
                uint8_t hi = get_next_zchar().code << 5;
                ch = hi | get_next_zchar().code;
            } else if (z == 7) {
                buffer_char(0x0D);
                ch = 0x0A;
            } else {
                ch = a2_table[z - 8];
            }
        }
        buffer_char(ch);
    }
}

/* Prints the z-string at Z_PC and moves Z_PC past it. The instruction
 * reader's page stays mapped as the string reader left it. */
void print_string_literal(void) {
    Z_PC2_L = Z_PC[0];
    Z_PC2_H = Z_PC[1];
    Z_PC2_HH = Z_PC[2];
    ZCODE_PAGE_VALID2 = 0;
    print_zstring();
    Z_PC[0] = Z_PC2_L;
    Z_PC[1] = Z_PC2_H;
    Z_PC[2] = Z_PC2_HH;
    ZCODE_PAGE_VALID = ZCODE_PAGE_VALID2;
    ZCODE_PAGE_ADDR = ZCODE_PAGE_ADDR2;
}
