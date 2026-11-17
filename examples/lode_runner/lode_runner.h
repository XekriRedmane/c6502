/*
    lode_runner.h -- what the units of Lode Runner share: the game's
    zero page and memory, the data tables outside its code, the Apple
    II's soft switches, DOS's disk routine, and each unit's routines.

    The game is main.asm of
    https://github.com/XekriRedmane/lode_runner_reveng, Broderbund's
    Lode Runner (1983) for the Apple II, rewritten in C. Its code fills
    $5F00-$8E52. Everything else in memory (the level maps, the sprite
    and pixel-shift tables, the splash screen, the demo's levels and
    moves, the high-score sector, and DOS) is data that the original's
    image puts in place, and this program reads it where the original
    does.

    The game's state is at the original's addresses, so that a program
    started from the original's memory image finds every byte it reads
    before writing it where the original would. The bytes the original
    uses only for scratch (its PTR1/PTR2, TMP_PTR, ROW_ADDR, MASK0,
    SCRATCH_5C, and the like) aren't declared here. In C they're
    locals, parameters, and return values, which the compiler places.
*/
#ifndef LODE_RUNNER_H
#define LODE_RUNNER_H

#include <stdbool.h>
#include <stdint.h>

/* ---- Sprites and the level grid -------------------------------------- */

/* A level is 16 rows of 28 sprites. The original's MAX_GAME_ROW and
 * MAX_GAME_COL are the last row and column. */
#define LEVEL_ROWS 16
#define LEVEL_COLS 28
#define MAX_GAME_ROW 15
#define MAX_GAME_COL 27

enum {
    SPRITE_EMPTY = 0x00,
    SPRITE_BRICK = 0x01,
    SPRITE_STONE = 0x02,
    SPRITE_LADDER = 0x03,
    SPRITE_ROPE = 0x04,
    SPRITE_TRAP = 0x05,
    SPRITE_INVISIBLE_LADDER = 0x06,
    SPRITE_GOLD = 0x07,
    SPRITE_GUARD = 0x08,
    SPRITE_PLAYER = 0x09,
    SPRITE_ALLWHITE = 0x0A,
    SPRITE_BRICK_FILL0 = 0x37,
    SPRITE_BRICK_FILL1 = 0x38,
    SPRITE_GUARD_EGG0 = 0x39,
    SPRITE_GUARD_EGG1 = 0x3A,
    /* PUT_DIGIT draws digit d as sprite SPRITE_DIGIT0 + d. */
    SPRITE_DIGIT0 = 0x3B,
};

/* GAME_MODE's values. Modes 0 and 1 run the demo, which reads its
 * levels from memory, and the others play levels read from the disk. */
enum {
    GAME_MODE_SPLASH_SCREEN = 0,
    GAME_MODE_ATTRACT_MODE = 1,
    GAME_MODE_PLAY_MODE = 2,
    GAME_MODE_PLAY_IN_EDITOR = 3,
    GAME_MODE_4 = 4,
    GAME_MODE_LEVEL_EDITOR = 5,
};

/* INPUT_MODE's values, the keys that choose them. */
enum {
    JOYSTICK_MODE = 0xCA,   /* 'J' */
    KEYBOARD_MODE = 0xCB,   /* 'K' */
};

/* The RWTS commands, for IOB_COMMAND_CODE. */
enum {
    DISK_ACCESS_READ = 1,
    DISK_ACCESS_WRITE = 2,
    DISK_ACCESS_FORMAT = 4,
};

/* A guard's move, the value DETERMINE_GUARD_MOVE returns. */
enum {
    GUARD_ACTION_DO_NOTHING = 0,
    GUARD_ACTION_MOVE_LEFT = 1,
    GUARD_ACTION_MOVE_RIGHT = 2,
    GUARD_ACTION_MOVE_UP = 3,
    GUARD_ACTION_MOVE_DOWN = 4,
};

/* ---- Zero page --------------------------------------------------------- */

/*
    DOS 3.3's RWTS uses bytes in $26-$4F. The original keeps nothing
    there across a disk access, and neither does this program, so the
    whole range is reserved and the compiler places nothing in it.
*/
extern uint8_t __dos_zp [[c6502::zeropage(0x26)]][0x2A];

/* The player. The column and row are the level cell the player is in,
 * and the two adjustments, 0-4, are the offset from that cell's
 * center plus 2, so 2 is exactly on it. */
extern uint8_t PLAYER_COL [[c6502::zeropage(0x00)]];
extern uint8_t PLAYER_ROW [[c6502::zeropage(0x01)]];
extern uint8_t PLAYER_X_ADJ [[c6502::zeropage(0x02)]];
extern uint8_t PLAYER_Y_ADJ [[c6502::zeropage(0x03)]];
/* An index into SPRITE_ANIM_SEQS (player.c). */
extern uint8_t PLAYER_ANIM_STATE [[c6502::zeropage(0x04)]];
/* Bit 7 set: facing left. Otherwise facing right. */
extern uint8_t PLAYER_FACING_DIRECTION [[c6502::zeropage(0x05)]];

/* The guard being moved, copied out of the GUARD_* arrays by
 * load_guard_data and back by store_guard_data. GUARD_NUM is 1-based. */
extern uint8_t GUARD_LOC_COL [[c6502::zeropage(0x12)]];
extern uint8_t GUARD_LOC_ROW [[c6502::zeropage(0x13)]];
extern uint8_t GUARD_ANIM_STATE [[c6502::zeropage(0x14)]];
extern uint8_t GUARD_FACING_DIRECTION [[c6502::zeropage(0x15)]];
/* Positive: counting down while the guard is stuck in a hole.
 * Negative: the guard carries gold, and drops it when this counts up
 * to 0. */
extern uint8_t GUARD_GOLD_TIMER [[c6502::zeropage(0x16)]];
extern uint8_t GUARD_X_ADJ [[c6502::zeropage(0x17)]];
extern uint8_t GUARD_Y_ADJ [[c6502::zeropage(0x18)]];
extern uint8_t GUARD_NUM [[c6502::zeropage(0x19)]];

/* The column a killed guard is reborn at, stepped each turn. */
extern uint8_t GUARD_RESURRECT_COL [[c6502::zeropage(0x53)]];
/* The index of the last note in SOUND_PITCH and SOUND_DURATION, 0 when
 * none are queued. */
extern uint8_t NOTE_INDEX [[c6502::zeropage(0x54)]];
extern uint8_t GUARD_GOLD_TIMER_START_VALUE [[c6502::zeropage(0x5F)]];
/* Three bit masks of the guards that move in each of three phases. */
extern uint8_t GUARD_PATTERNS [[c6502::zeropage(0x60)]][3];
extern uint8_t GUARD_PHASE [[c6502::zeropage(0x64)]];
/* What READ_PADDLES counted for each paddle. */
extern uint8_t PADDLE_VALUES [[c6502::zeropage(0x65)]][2];

/* The text cursor and the cell DRAW_SPRITE_PAGE1/2 draw at. */
extern uint8_t GAME_COLNUM [[c6502::zeropage(0x85)]];
extern uint8_t GAME_ROWNUM [[c6502::zeropage(0x86)]];
/* 0x20 for page 1, 0x40 for page 2: where PUT_CHAR and PUT_DIGIT draw. */
extern uint8_t DRAW_PAGE [[c6502::zeropage(0x87)]];
/* TO_DECIMAL3's hundreds, tens, and units. BCD_TO_DECIMAL2 sets the
 * last two. */
extern uint8_t DIGITS [[c6502::zeropage(0x89)]][3];
/* The number of passes PLAY_SOUND's delay takes, initially 6. */
extern uint8_t FRAME_PERIOD [[c6502::zeropage(0x8C)]];
extern uint8_t GUARD_COUNT [[c6502::zeropage(0x8D)]];
/* Eight BCD digits, the tens and units in SCORE[0]. */
extern uint8_t SCORE [[c6502::zeropage(0x8E)]][4];
extern uint8_t GOLD_COUNT [[c6502::zeropage(0x93)]];
/* 1 until the player picks up gold during a move. */
extern uint8_t DIDNT_PICK_UP_GOLD [[c6502::zeropage(0x94)]];
extern uint8_t INPUT_MODE [[c6502::zeropage(0x95)]];
/* The level's sector on the disk, from track 3 on: LEVELNUM - 1. */
extern uint8_t DISK_LEVEL_LOC [[c6502::zeropage(0x96)]];
extern uint8_t GUARD_PATTERN_OFFSET [[c6502::zeropage(0x97)]];
extern uint8_t LIVES [[c6502::zeropage(0x98)]];
/* 0: the speaker stays silent. */
extern uint8_t ENABLE_SOUND [[c6502::zeropage(0x99)]];
/* Nonzero while the player lives. The original clears it with LSR. */
extern uint8_t ALIVE [[c6502::zeropage(0x9A)]];
/* 0 while the player falls. PLAY_SOUND then plays the falling tone. */
extern uint8_t NOT_FALLING [[c6502::zeropage(0x9B)]];
/* 0xFF digging left, 0x00 not digging, 0x01 digging right. */
extern uint8_t DIG_DIRECTION [[c6502::zeropage(0x9C)]];
/* Nonzero when the game's score may go in the high-score table. The
 * cheat keys clear it. */
extern uint8_t HI_SCORE_ELIGIBLE [[c6502::zeropage(0x9D)]];
/* The key commands: 'I', 'J', 'K', 'L', 'O', 'U' with bit 7 set.
 * KEY_COMMAND_LR holds the left-right one. */
extern uint8_t KEY_COMMAND [[c6502::zeropage(0x9E)]];
extern uint8_t KEY_COMMAND_LR [[c6502::zeropage(0x9F)]];
extern uint8_t DIG_ANIM_STATE [[c6502::zeropage(0xA0)]];
/* Nonzero: LOAD_LEVEL takes the hidden ladders, the guards, and the
 * player out of the drawn level, as the game does. 0: it draws the
 * level as it's stored, as the editor does. */
extern uint8_t VERBATIM [[c6502::zeropage(0xA2)]];
extern uint8_t LADDER_COUNT [[c6502::zeropage(0xA3)]];
/* The falling tone's pitch, twice over. */
extern uint8_t FALL_PITCH [[c6502::zeropage(0xA4)]];
/* 0: the next iris wipe only opens. 1: it closes first. */
extern uint8_t WIPE_MODE [[c6502::zeropage(0xA5)]];
/* 1-based. */
extern uint8_t LEVELNUM [[c6502::zeropage(0xA6)]];
extern uint8_t GAME_MODE [[c6502::zeropage(0xA7)]];
/* The demo's next move, in pairs of a move and how many turns it
 * lasts; see CHECK_FOR_MODE_1_INPUT (input.c). */
extern const uint8_t *DEMO_PTR [[c6502::zeropage(0xA8)]];
extern uint8_t DEMO_MOVE [[c6502::zeropage(0xAA)]];
extern uint8_t DEMO_MOVE_TURNS [[c6502::zeropage(0xAB)]];
/* Nonzero while the demo runs. A key pressed during it clears it. */
extern uint8_t DEMO_RUNNING [[c6502::zeropage(0xAC)]];
/* The editor's level is saved: nonzero once saved, 0 once changed. */
extern uint8_t LEVEL_SAVED [[c6502::zeropage(0xAD)]];

/* COMPUTE_SHIFTED_SPRITE's output: 11 rows of 3 bytes, the sprite
 * shifted into the screen bytes it covers. */
extern uint8_t BLOCK_DATA [[c6502::zeropage(0xDF)]][33];

/* ---- Memory ------------------------------------------------------------ */

/* The level being played. LEVEL_MAP holds what's in each cell now,
 * guards and the player included. LEVEL_BG holds what's behind them:
 * the level's bricks, ladders, ropes, and gold. */
extern uint8_t LEVEL_MAP [[c6502::address(0x0800)]][LEVEL_ROWS][LEVEL_COLS];
extern uint8_t LEVEL_BG [[c6502::address(0x0A00)]][LEVEL_ROWS][LEVEL_COLS];

/* The hidden ladders that appear once the gold is all picked up.
 * LADDER_LOCS_COL[0] is a flag that ENABLE_NEXT_LEVEL_LADDERS sets, and
 * entries 1..LADDER_COUNT are ladders; a column of 0xFF marks one
 * that's been drawn. */
extern uint8_t LADDER_LOCS_COL [[c6502::address(0x0C00)]][48];
extern uint8_t LADDER_LOCS_ROW [[c6502::address(0x0C30)]][48];

/* The guards, entries 1..GUARD_COUNT. */
extern uint8_t GUARD_LOCS_COL [[c6502::address(0x0C60)]][8];
extern uint8_t GUARD_LOCS_ROW [[c6502::address(0x0C68)]][8];
extern uint8_t GUARD_GOLD_TIMERS [[c6502::address(0x0C70)]][8];
extern uint8_t GUARD_X_ADJS [[c6502::address(0x0C78)]][8];
extern uint8_t GUARD_Y_ADJS [[c6502::address(0x0C80)]][8];
extern uint8_t GUARD_ANIM_STATES [[c6502::address(0x0C88)]][8];
extern uint8_t GUARD_FACING_DIRECTIONS [[c6502::address(0x0C90)]][8];
extern uint8_t GUARD_RESURRECTION_TIMERS [[c6502::address(0x0C98)]][8];

/* The holes being dug or filling in, 31 of them. A timer of 0 is a
 * free entry. */
extern uint8_t BRICK_DIG_COLS [[c6502::address(0x0CA0)]][32];
extern uint8_t BRICK_DIG_ROWS [[c6502::address(0x0CC0)]][32];
extern uint8_t BRICK_FILL_TIMERS [[c6502::address(0x0CE0)]][32];

/* The sector RWTS reads into and writes from. A level is stored in
 * its first 224 bytes, a nibble to a cell, low nibble first. */
extern uint8_t DISK_BUFFER [[c6502::address(0x0D00)]][256];

/* The queued notes, entries 1..NOTE_INDEX, the last played first. */
extern uint8_t SOUND_DURATION [[c6502::address(0x0E00)]][128];
extern uint8_t SOUND_PITCH [[c6502::address(0x0E80)]][128];

/* The splash screen, run-length coded; see RESET_GAME (main.c). */
extern const uint8_t SPLASH_SCREEN [[c6502::address(0x0F00)]][];

/* The address of each of the 192 lines of the high-resolution screen,
 * for page 1 less $2000. */
extern const uint8_t ROW_TO_OFFSET_LO [[c6502::address(0x1A85)]][192];
extern const uint8_t ROW_TO_OFFSET_HI [[c6502::address(0x1B45)]][192];

/* The screen coordinates of the level's cells: the first line of each
 * row (17 rows, the status line last), and the first double pixel of
 * each column. */
extern const uint8_t HALF_SCREEN_COL_TABLE [[c6502::address(0x1C35)]][28];
extern const uint8_t SCREEN_ROW_TABLE [[c6502::address(0x1C51)]][17];
/* For each column, the screen byte its first pixel is in and that
 * pixel's place in the byte. */
extern const uint8_t COL_BYTE_TABLE [[c6502::address(0x1C62)]][28];
extern const uint8_t COL_SHIFT_TABLE [[c6502::address(0x1C7E)]][28];
/* The same for each double-pixel column, 0-139. */
extern const uint8_t HALF_SCREEN_COL_BYTE_TABLE [[c6502::address(0x1C9A)]][140];
extern const uint8_t HALF_SCREEN_COL_SHIFT_TABLE [[c6502::address(0x1D26)]][140];

/* What the editor writes to track 0, sector 0 of a data disk. */
extern const uint8_t DISK_BOOT_SECTOR_DATA [[c6502::address(0x1DB2)]][256];

/* The high-score sector, track 12, sector 15. Entries are 1-based in
 * the original; HI_SCORES[n - 1] is its entry n. The sector's
 * buffer for RWTS is HI_SCORES, the sector's first byte. */
struct hi_score {
    uint8_t initials[3];    /* high-bit ASCII */
    uint8_t level;
    uint8_t score[4];       /* BCD, the highest two digits first */
};
extern struct hi_score HI_SCORES [[c6502::address(0x1F00)]][10];
/* "LODE RUNNER" in high-bit ASCII on a data disk. */
extern uint8_t HI_SCORE_MARKER [[c6502::address(0x1FF4)]][11];
/* Nonzero on the master disk. */
extern uint8_t HI_SCORE_MASTER_FLAG [[c6502::address(0x1FFF)]];

/* The two high-resolution pages. */
extern uint8_t HGR1 [[c6502::address(0x2000)]][0x2000];
extern uint8_t HGR2 [[c6502::address(0x4000)]][0x2000];

/* The demo's moves, read through DEMO_PTR. */
extern const uint8_t DEMO_MOVES [[c6502::address(0x9B00)]][];
/* The demo's three levels, stored as on the disk. DEMO_LEVELS[n] is
 * level n, so DEMO_LEVELS[0] isn't a level. */
extern const uint8_t DEMO_LEVELS [[c6502::address(0x9E00)]][4][256];

/* For each of the 7 shifts, a page of the addresses of each byte's
 * pixels shifted: the low bytes at 0x00-0x7F and the high bytes at
 * 0x80-0xFF. Each address is of 2 bytes in PIXEL_PATTERN_TABLE. */
extern const uint8_t PIXEL_SHIFT_TABLE [[c6502::address(0xA200)]][7][256];
/* The sprites: 22 rows of 0x68 bytes. Row 2 * line + half holds the
 * left (half 0) or right (half 1) byte of each sprite's line. */
extern const uint8_t SPRITE_DATA [[c6502::address(0xAD00)]][22][0x68];

/* ---- DOS --------------------------------------------------------------- */

/* The RWTS input/output block. */
struct iob {
    uint8_t table_type;             /* must be 1 */
    uint8_t slot_times_16;
    uint8_t drive_number;
    uint8_t volume;                 /* 0 for any */
    uint8_t track;
    uint8_t sector;
    void *dct_addr;
    uint8_t *buffer_addr;
    uint8_t unused;
    uint8_t partial_byte_count;
    uint8_t command;                /* DISK_ACCESS_* */
    uint8_t ret_code;
    uint8_t last_volume;
    uint8_t last_slot_times_16;
    uint8_t last_drive_number;
};
extern struct iob DOS_IOB [[c6502::address(0xB7E8)]];

/* DOS's RWTS, behind the routine that disables interrupts around it.
 * It takes the address of its parameter block with the low byte in Y
 * and the high byte in A, and sets the carry when the access failed. */
[[c6502::zp_abi, c6502::return_bool_in_carry, c6502::address(0xB7B5)]]
bool RWTS(struct iob *iob [[c6502::in_ya]]);

/* DOS's routines that FORMAT_PATCH (editor.c) goes on to. */
[[c6502::zp_abi, c6502::address(0xB9A0)]]
void SEEKABS(uint8_t track [[c6502::in_a]]);
[[noreturn, c6502::zp_abi, c6502::address(0xBEAF)]] void DSKFORM(void);

/* DOS's current track for the drive, which FORMAT_PATCH sets. */
extern uint8_t DOS_CURRENT_TRACK [[c6502::address(0x0478)]];

/* ---- Soft switches ----------------------------------------------------- */

/* Bit 7 set: a key is waiting, high-bit ASCII. */
extern volatile uint8_t KBD [[c6502::address(0xC000)]];
extern volatile uint8_t KBDSTRB [[c6502::address(0xC010)]];
extern volatile uint8_t SPKR [[c6502::address(0xC030)]];
extern volatile uint8_t TXTCLR [[c6502::address(0xC050)]];
extern volatile uint8_t MIXCLR [[c6502::address(0xC052)]];
extern volatile uint8_t TXTPAGE1 [[c6502::address(0xC054)]];
extern volatile uint8_t TXTPAGE2 [[c6502::address(0xC055)]];
extern volatile uint8_t HIRES [[c6502::address(0xC057)]];
/* Bit 7 set: the button (or open or solid apple) is down. */
extern volatile uint8_t BUTN0 [[c6502::address(0xC061)]];
extern volatile uint8_t BUTN1 [[c6502::address(0xC062)]];
/* Bit 7 set until the paddle's timer runs out. */
extern volatile uint8_t PADDL [[c6502::address(0xC064)]][2];
extern volatile uint8_t PTRIG [[c6502::address(0xC070)]];
/* The disk controllers' switches, from $C088. A slot's are at slot * 16 from
 * here: DISK_IO[slot16] turns the motor off, and
 * DISK_IO[slot16 + 4] reads the data latch. */
extern volatile uint8_t DISK_IO [[c6502::address(0xC088)]][0x78];
extern volatile uint8_t ROMIN_RDROM_WRRAM2 [[c6502::address(0xC081)]];

/* ---- main.c ------------------------------------------------------------ */

/* These start the game over at one of its stages, and the original
 * jumps to them from wherever it is, leaving the stack as it was. */
[[noreturn]] void reset_game(void);
[[noreturn]] void long_delay(void);
[[noreturn]] void init_game_data(void);
/* The code at $6000, which the reset vector reaches. */
[[noreturn]] void cold_start(void);

/* ---- level.c ----------------------------------------------------------- */

void load_level(bool verbatim);
void compress_and_save_level_data(void);
/* Reads, writes, or formats (DISK_ACCESS_*) the level's sector, or in
 * the demo copies the level out of DEMO_LEVELS. */
void access_compressed_level_data(uint8_t command);
/* Runs RWTS on DOS_IOB, and starts the game over if it fails. */
void access_disk_or_reset_game(void);
/* Reads or writes the high-score sector. After a read, returns 0 if
 * the disk isn't a data disk, 1 if it's the master disk, and 0xFF if
 * it's a data disk. */
uint8_t access_hi_score_data_from_disk(uint8_t command);
void enable_next_level_ladders(void);
/* "LODE RUNNER" in high-bit ASCII, the mark of a data disk. */
extern const uint8_t HI_SCORE_DATA_MARKER[11];

/* ---- player.c ---------------------------------------------------------- */

void move_player(void);

/* ---- input.c ----------------------------------------------------------- */

/* Sets KEY_COMMAND and KEY_COMMAND_LR from the keyboard, the joystick,
 * or the demo, and handles the control keys. */
void check_for_input(void);
/* Blink a cursor (SPRITE_ALLWHITE for 0) at GAME_COLNUM, GAME_ROWNUM on
 * page 2 or page 1 until a key is pressed, and return the key. */
uint8_t wait_for_key(uint8_t cursor);
uint8_t wait_for_key_with_cursor_page_1(uint8_t cursor);
/* Clears the keyboard strobe until no key is waiting. */
void wait_key(void);
void detect_lack_of_joystick(void);

/* ---- guards.c ---------------------------------------------------------- */

void move_guards(void);
void load_guard_data(void);
/* Three masks of the guards that move in each phase (GUARD_PATTERNS),
 * for each of 12 numbers of guards and difficulties. */
extern const uint8_t GUARD_PATTERNS_LIST[36];
/* Erase or draw the current guard. draw_guard_sprite draws `sprite` at
 * the guard's place. */
void erase_guard(void);
void draw_guard_sprite(uint8_t sprite);

/* ---- timers.c ---------------------------------------------------------- */

/* Refills the holes and brings the dead guards back. */
void handle_timers(void);

/* ---- screen.c ---------------------------------------------------------- */

void put_status(void);
void put_status_draw(void);
void put_status_lives(void);
void put_status_level(void);
/* Adds a BCD number of points, 0x0250 for 250, to SCORE and draws it. */
void add_and_update_score(uint16_t bcd_points);
void bcd_to_decimal2(uint8_t bcd);
void to_decimal3(uint8_t n);
void put_digit(uint8_t digit);
uint8_t char_to_sprite_num(uint8_t c);
/* Draws a high-bit ASCII character at GAME_COLNUM, GAME_ROWNUM on
 * DRAW_PAGE, and moves the cursor right. 0x8D starts a new line. */
void put_char(uint8_t c);
void newline(void);
/* Puts the ASCII string `s`, each character with bit 7 set, so that
 * '\r' starts a new line. */
void put_string(const char *s);
void clear_hgr1(void);
void clear_hgr2(void);
/* The address of screen line `row` on the page at 0x20 or 0x40. */
uint8_t *row_to_addr(uint8_t row, uint8_t page);

/* ---- hiscore.c --------------------------------------------------------- */

void hi_score_screen(void);
void record_hi_score_data_to_disk(void);

/* ---- editor.c ---------------------------------------------------------- */

[[noreturn]] void level_editor(void);
[[noreturn]] void start_level_editor(void);
[[noreturn]] void editor_command_loop(void);
void bad_data_disk(void);

/* ---- sprites.c --------------------------------------------------------- */

/* Draw `sprite` over the cell at GAME_COLNUM, GAME_ROWNUM on page 1 or
 * page 2. */
void draw_sprite_page1(uint8_t sprite);
void draw_sprite_page2(uint8_t sprite);
/* Take `sprite` off page 1 at a double-pixel column and screen line,
 * leaving page 2 (the background) showing where it was. */
void erase_sprite_at_pixel_coords(uint8_t sprite, uint8_t half_col, uint8_t row);
/* Draw `sprite` on page 1 at a double-pixel column and screen line.
 * Returns the bits where the sprite covers page 1 bits that differ
 * from page 2's, nonzero when it touches another sprite. */
uint8_t draw_sprite_at_pixel_coords(uint8_t sprite, uint8_t half_col, uint8_t row);
/* The screen line and double-pixel column of a cell's sprite, with
 * the cell's 0-4 adjustment applied. */
uint8_t screen_row_for(uint8_t row, uint8_t y_adj);
uint8_t half_screen_col_for(uint8_t col, uint8_t x_adj);

/* ---- sound.c ----------------------------------------------------------- */

void sound_delay(uint8_t index);
void sound_delay1(uint8_t count);
void beep(void);
/* Clicks the speaker every `pitch` passes for `duration` passes of 256.
 * `y` is the first pass count, which the original takes from whatever
 * the caller left in Y. */
void play_note(uint8_t pitch, uint8_t duration, uint8_t y);
void append_note(uint8_t pitch, uint8_t duration);
/* Queues the notes at `notes`, pairs of a duration and a pitch, up to
 * a 0. */
void load_sound_data(const uint8_t *notes);
/* Plays the last queued note, or the falling tone or a delay when none
 * is queued. Returns true when it played a queued note. */
bool play_sound(void);

/* ---- wipe.c ------------------------------------------------------------ */

/* Closes the screen in a circle if WIPE_MODE says to, then opens it
 * on page 2's picture. */
void iris_wipe(void);
/* Shows GAME OVER turning, and returns true if a key or button cut it
 * short. game_over_key then holds what it read from KBD, which the
 * original leaves in A for its caller. */
bool spinning_game_over(void);
extern uint8_t game_over_key;

#endif
