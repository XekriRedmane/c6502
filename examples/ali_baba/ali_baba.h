/*
    ali_baba.h -- what the units of Ali Baba and the Forty Thieves
    share: the game's zero page and memory, the data its memory image
    keeps in place, the Apple II's soft switches, the Monitor and disk
    routines it calls, and each unit's routines.

    The game is output/main.asm of
    https://github.com/XekriRedmane/ali_baba_reveng, Electronic Arts'
    Ali Baba and the Forty Thieves (1982) for the Apple II, rewritten in
    C. The program starts at $0800 from main.bin, the memory image
    $0500-$BFFF as the loader and GAME_INIT's relocation leave it. The
    C program replaces the original's code and the tables among it,
    and reads everything else where the original's image puts it: the
    location and mob records at $4000-$59FF, the variables of page
    $5A, the scene buffer at $5C00, the stats screen, the shop table,
    the item and object names, the tunes, the fonts, the messages, the
    copy-protected disk reader at $B300, and DOS's RWTS at $B600.

    The game's state is at the original's addresses, so that a program
    started from the original's image finds every byte it reads before
    writing it where the original would. The bytes the original uses
    only as scratch aren't declared here, and in C they're locals,
    parameters, and return values, which the compiler places. Those
    are $BA-$BF (DATA_PTR, TARGET_REC, PRINT_STRING_ADDR, NUM_VALUE,
    EVENT_PTR, and their other names), $E1-$E3, $E6/$E7, $F6/$F7, the
    HRCG's $EB-$EF and $FF, and a few bytes of page $5A that UNITS.md
    lists.

    Departures that every unit has to follow:

    - Output. The original prints with JSR ROM_COUT1 ($FDED), which
      jumps through CSW to the HRCG's HRCG_COUT. The units call
      hrcg_cout() directly. A C function reached through CSW would be
      a call that the compiler's call graph doesn't see, so the
      zero-page bytes of hrcg_cout and everything it calls could be
      shared with its caller's, and a function with register
      parameters can't have its address taken anyway. COUT is not
      declared here, so that no unit calls it by mistake.
    - The chain. The turn and the menus run as routines that jump to
      each other at one stack level, until one of them returns to
      whoever called START_MOB_TURN or INIT_WORLD. In C each such
      routine returns the CHAIN_ state of the routine that runs next,
      and run_chain() (turn.c) calls them in a loop. A JMP to another
      chain routine is `return CHAIN_X;`, a JMP to INPUT_LOOP with
      descriptor T is `return input_loop(&T);`, a JMP to an ordinary
      routine is a call followed by `return CHAIN_RETURN;`, and an RTS
      is `return CHAIN_RETURN;`. Nothing called from a chain routine
      may call run_chain(), because that would be recursion.
    - No relocation. The C starts from main.bin, which the original's
      GAME_INIT has already relocated, so RELOCATE_HIRES_STAGING isn't
      translated and GAME_INIT doesn't call it.
    - No calls through function pointers. c6502 can't link them. Every
      table of routine addresses becomes a table of small numbers and
      a switch: the menus' exits are CHAIN_ states, their init
      routines are MENU_INIT_ numbers, the tune player's tone vector is
      an `if`, and the shop compares each slot's handler word with the
      SHOP_*_ADDR constants below.

    UNITS.md lists every routine of every unit, the tables each unit
    defines, and what each translator has to know.
*/
#ifndef ALI_BABA_H
#define ALI_BABA_H

#include <stdbool.h>
#include <stdint.h>

/* ---- Map positions and directions ----------------------------------- */

/* A location's map is 20 columns by 10 rows, and a position is
 * 20 * row + col, 0-199. POS_TO_COLROW gives col = pos % 20 and
 * row = pos / 20 for any byte, so a wrapped position such as 245 has
 * row 12. */
constexpr uint8_t MAP_WIDTH = 20;
constexpr uint8_t MAP_HEIGHT = 10;

/* A move's direction, as READ_KEYBOARD and NPC_MOVE_AI return it and
 * as a menu option's neighbors are indexed. GAME_TURN_LOOP tests for
 * up with `CMP #2` and `BPL`, so it moves up for DIR_NONE as well. */
enum {
    DIR_NONE = 0,       /* the fire key or button; "stay" from the AI */
    DIR_UP = 1,
    DIR_DOWN = 2,
    DIR_LEFT = 3,
    DIR_RIGHT = 4,
};

/* ---- Records ------------------------------------------------------- */

/*
    A location: 9 bytes. LOCATIONS[0] at $4000 is a header (struct
    location_header below), and LOCATIONS[1]-[62] at $4009-$4236 are
    the places. The original's CHAR_PTR, CHAR_REC, GET_CHAR_RECORD,
    CURRENT_PLAYER, ACTIVE_CHAR, and SOURCE_CHAR all name locations,
    although the names say "character".

    style: bits 6-0 are the map style, which ENTER_LOCATION compares
    with LOCATION_STYLE to reload the tiles. Bits 6-5 are also the
    location's faction ((style & $60) against a mob's flags & $70) and
    select the rock-fall message. Bit 7 is set by DRAW_MAP_ENTITIES
    once the location's entities are drawn.

    pc_count: the player characters (appearance below $15) in the
    location's mob list. Record 0's byte ($4008) is never changed.
*/
struct location {
    const uint8_t *name;    /* +0: length byte, then high-bit ASCII */
    uint8_t mob_head;       /* +2: 1-based mob index of the first mob here, 0 = none */
    uint8_t min_pos;        /* +3: top-left corner, a position */
    uint8_t max_pos;        /* +4: bottom-right corner, a position */
    uint8_t style;          /* +5 */
    struct event *events;   /* +6: 3-byte entries, ended by pos $FF */
    uint8_t pc_count;       /* +8 */
};

/* Record 0 at $4000, the same 9 bytes seen as a header. Its mob list
 * (CHAR_LIST_HEAD) holds the mobs that are at no location. The
 * comments give the original's EQU names of the fields. */
struct location_header {
    struct mob *mob_base;   /* +0: MOB_BASE_LO/HI, $4C80, GET_MOB_DATA's base */
    uint8_t char_list_head; /* +2: CHAR_LIST_HEAD */
    uint8_t unused_3[2];
    const uint8_t *msg_table; /* +5: MSG_TABLE_PTR, $9AA6, DISPLAY_MESSAGE's table */
    uint8_t unused_7[2];
};

/*
    A mob or character: 16 bytes. MOBS[n - 1] is mob index n, and a
    mob index of 0 ends a list. The field names are the stats screen's,
    and the original's MFIELD_ names are in the comments.

    - appearance: below $15 is a player character. appearance / 21 is
      the font group that START_MOB_TURN stores in CHAR_AI_MODE
      (0 = controlled by the player).
    - strength: bits 4-0 are STRENGTH. (byte >> 1) & $0F is a defense,
      and (byte >> 3) & 3 the shop's bonus.
    - life: bits 5-0 are LIFE FORCE. Below 3 the mob is out of action.
    - sword, dagger: bits 3-0 are the weapon minus 1. Bit 7 is set by a
      fumble.
    - armor: bits 3-0 are ARMOR, bits 6-4 the pending level amount,
      bit 7 a status bit that FLUSH_PENDING_XP keeps. The original's EQU
      comment calls the low nibble a group count, which the code
      doesn't bear out.
    - level: bits 4-0 are the level (DEXTERITY on the stats screen).
      Bit 5 is rage, bit 6 cautious, bit 7 aggressive.
    - speed: bits 3-0 are the move points per turn. (byte >> 1) & 7 is
      compared with STEPS_TAKEN.
    - flags: bits 1-0 the combat state, bit 2 engaged (restricted),
      bit 4 dead, bits 6-5 the faction, bit 7 a class bit.

    PLAYER_ATTACK and SET_BLINK_TARGET also point MOB_PTR at an event
    entry, and then read the name's high byte as the entry's type
    (MFIELD_TYPE). The C keeps that.
*/
struct mob {
    const uint8_t *name;    /* +0  MFIELD_NAME: length byte, then high-bit ASCII */
    uint8_t next;           /* +2  MFIELD_NEXT: next mob index in the same list */
    uint8_t pos;            /* +3  MFIELD_POSITION */
    uint8_t appearance;     /* +4  MFIELD_APPEARANCE */
    uint8_t strength;       /* +5  MFIELD_STRENGTH */
    uint8_t life;           /* +6  MFIELD_HP */
    uint8_t sword;          /* +7  MFIELD_ATTACK */
    uint8_t armor;          /* +8  MFIELD_XP */
    uint8_t dagger;         /* +9  MFIELD_DEFENSE */
    uint8_t unused_10;      /* +10 */
    uint8_t level;          /* +11 MFIELD_LEVEL */
    uint8_t speed;          /* +12 MFIELD_LEVEL_ENC */
    uint16_t gold;          /* +13 MFIELD_GOLD: DUCATS */
    uint8_t flags;          /* +15 MFIELD_FLAGS */
};

/* The original's byte offsets, for code that reads a record a byte at
 * a time. */
enum {
    MFIELD_NAME = 0, MFIELD_TYPE = 1, MFIELD_NEXT = 2, MFIELD_POSITION = 3,
    MFIELD_APPEARANCE = 4, MFIELD_STRENGTH = 5, MFIELD_HP = 6,
    MFIELD_ATTACK = 7, MFIELD_XP = 8, MFIELD_DEFENSE = 9, MFIELD_LEVEL = 11,
    MFIELD_LEVEL_ENC = 12, MFIELD_GOLD = 13, MFIELD_FLAGS = 15,
};

/* Bits and masks of a mob's fields. */
constexpr uint8_t STRENGTH_MASK = 0x1F;
constexpr uint8_t HP_MASK = 0x3F;
constexpr uint8_t HP_FLAGS_MASK = 0xC0;
constexpr uint8_t LEVEL_MASK = 0x1F;
constexpr uint8_t LOW_NIBBLE_MASK = 0x0F;
constexpr uint8_t FLAGS_STATE_MASK = 0x03;
constexpr uint8_t FLAGS_ENGAGED_BIT = 0x04;
constexpr uint8_t FLAGS_DEAD_BIT = 0x10;
constexpr uint8_t FLAGS_FACTION_MASK = 0x70;
constexpr uint8_t NPC_APPEARANCE_MIN = 0x15;    /* appearance below this: a player character */

/*
    An event-list entry: 3 bytes. A location's `events` points at its
    list.

    type        meaning                                   value
    $00-$3F     random encounter                          spawn position
    $40-$BF     specific encounter (mob + $40)            spawn position
    $C0-$C8     gold: (type - $C0) << 8 | value           gold low byte
    $C9-$D0     trapped chest: gold high = type - $C9     gold low byte
    $D1         special chest
    $D2         persistent scene                          scene
    $D3         one-time scene                            scene
    $D4         scene and damage                          scene
    $D5, $D6    weapon or armor shop ($D7-$DA also reach  slot mask
                DISPLAY_SHOP)
    $DB-$FD     sent to TEARDOWN_INPUT by MOVE_LOOP
    $FE         an object (HANDLE_EVENT_SLOT)             object code

    UPDATE_EVENT_LIST adds gold to types $C0-$D0 with an 8-bit add and
    carry, which can carry a type past $D0 up to $D8.
*/
struct event {
    uint8_t pos;            /* 0-199, EVENT_EMPTY, or EVENT_END */
    uint8_t type;
    uint8_t value;
};

enum {
    EVENT_EMPTY = 0xFE,             /* pos: a free slot. type: an object. */
    EVENT_END = 0xFF,               /* pos: the end of the list */
    EVENT_TYPE_MOB = 0x40,
    EVENT_ACTIVATED = 0xC0,         /* types from here on are gold and objects */
    EVENT_TYPE_GOLD = 0xC0,
    EVENT_TYPE_TRAPPED_CHEST = 0xC9,
    EVENT_ACCUM_MAX = 0xD0,         /* the last type UPDATE_EVENT_LIST adds gold to */
    EVENT_TYPE_SPECIAL_CHEST = 0xD1,
    EVENT_TYPE_SCENE = 0xD2,
    EVENT_TYPE_SCENE_ONCE = 0xD3,
    EVENT_TYPE_SCENE_DAMAGE = 0xD4,
    EVENT_TYPE_WEAPON_SHOP = 0xD5,
    EVENT_TYPE_ARMOR_SHOP = 0xD6,
    EVENT_TYPE_SHOP_LAST = 0xDA,
    EVENT_TYPE_OBJECT = 0xFE,
};

/*
    What FIND_ENTITY_AT_POS returns in ENTITY_REC ($BE/$BF), and what
    GET_ENTITY_FONTCHAR and CLASSIFY_ENTITY_TYPE take: 0 for an empty
    interior cell, 1 for a cell on the viewport's edge, a mob record's
    address with bit 15 set, or an event entry's address. MOVE_LOOP
    tests the high byte, so 0 with a nonzero low byte is the edge.
*/
typedef uint16_t entity_ref;
constexpr entity_ref ENTITY_NONE = 0x0000;
constexpr entity_ref ENTITY_EDGE = 0x0001;
#define ENTITY_IS_MOB(e) (((e) & 0x8000) != 0)
#define ENTITY_IS_EVENT(e) ((e) > 0x00FF && !ENTITY_IS_MOB(e))
#define ENTITY_MOB(e) ((struct mob *)((e) & 0x7FFF))
#define ENTITY_EVENT(e) ((struct event *)(e))
#define MOB_REF(m) ((entity_ref)(uint16_t)(m) | 0x8000)

/* The original tests only the high byte of a pointer that a mob
 * group's walk leaves, so a pointer into page 0 counts as none. ai.c
 * and combat.c walk the groups. */
#define IN_PAGE_0(p) ((uint16_t)(p) < 0x0100)

/* ---- Menus ---------------------------------------------------------- */

/*
    One option of a menu. The original's 10-byte option record has the
    exit routine's address in its last two bytes. Here that is the
    CHAIN_ state that runs when the option is chosen, and the record
    is 9 bytes.
*/
struct menu_option {
    uint8_t enabled;        /* 0: not drawn, and skipped by moves */
    uint8_t position;       /* 0-39 row 21, 40-79 row 22, 80-119 row 23;
                               also the index into INPUT_BUFFER */
    uint8_t next[4];        /* the option reached by DIR_UP, DIR_DOWN,
                               DIR_LEFT, DIR_RIGHT (next[dir - 1]); 0 = none */
    uint8_t *text;          /* length byte, then high-bit ASCII */
    uint8_t exit;           /* CHAIN_ state */
};

/* A menu. The original's 4-byte descriptor is followed by its options.
 * c6502 rejects flexible array members, so the options are a separate
 * array that `options` points at, options[0] being option 1. */
struct menu {
    uint8_t count;
    uint8_t selection;      /* the option highlighted first, 1-based. The
                               original's comments call it max_range. */
    uint8_t init;           /* MENU_INIT_ */
    struct menu_option *options;
};

/* The routine CALL_INPUT_INIT runs for a menu, from the descriptor's
 * bytes 2-3. input.c's call_input_init switches on it. */
enum menu_init {
    MENU_INIT_NONE,                 /* $626B, an RTS */
    MENU_INIT_CONFIGURE_INPUT,      /* $60D2 CONFIGURE_INPUT */
    MENU_INIT_SETTINGS,             /* $61FD MENU_SETTINGS_INIT */
    MENU_INIT_ATTACK,               /* $6268 MENU_ATTACK_INIT */
    MENU_INIT_DROP_GOLD,            /* $62A9 MENU_DROP_GOLD_INIT */
    MENU_INIT_SAVE_LOAD,            /* $62EC SAVE_LOAD_INIT */
    MENU_INIT_FORMAT_SLOT_NUMBER,   /* $631B FORMAT_SLOT_NUMBER */
};

/* ---- The chain ------------------------------------------------------ */

/*
    The routines the turn and the menus run one after another at one
    stack level. Each returns the state that runs next, and run_chain()
    calls them until one returns CHAIN_RETURN, which is the original's
    RTS to START_MOB_TURN's caller or to GAME_INIT. A few states name
    ordinary routines that a menu option or a chain routine reaches by
    JMP, and run_chain() calls each of those and then stops.
*/
enum chain_state : uint8_t {
    CHAIN_RETURN = 0,
    /* turn.c */
    CHAIN_GAME_TURN_LOOP, CHAIN_MOVE_LOOP, CHAIN_END_TURN, CHAIN_TURN_DISPATCH,
    CHAIN_SET_STATE_FIGHT,              /* ordinary; GAME_INPUT option 4 */
    /* main.c */
    CHAIN_TOGGLE_SOUND,
    /* util.c */
    CHAIN_INIT_CHAR_SELECT,
    /* combat.c */
    CHAIN_AUTO_HEAL,                    /* ordinary; GAME_INPUT option 6 */
    CHAIN_ENTER_COMBAT_STATE,           /* ordinary; runs enter_combat_state(0x14) */
    CHAIN_ATTACK_RIGHT, CHAIN_ATTACK_LEFT, CHAIN_ATTACK_UP, CHAIN_ATTACK_DOWN,
    CHAIN_FIND_ATTACK_TARGET, CHAIN_SELECT_SOMEONE_ELSE,
    CHAIN_RESOLVE_ATTACK,               /* ordinary; ATTACK_TARGET option 1 */
    CHAIN_HANDLE_MOB_ENCOUNTER,
    /* input.c */
    CHAIN_TEARDOWN_INPUT, CHAIN_MENU_SEE_YOUR_LOT, CHAIN_MENU_CHECK_ATTACK,
    CHAIN_MENU_OTHER_OPTIONS, CHAIN_MENU_GAME_SETTINGS, CHAIN_MENU_GAME_SETTINGS_ENTER,
    CHAIN_SETTINGS_SPEED_UP, CHAIN_SETTINGS_SPEED_DOWN,
    CHAIN_SETTINGS_DANGER_UP, CHAIN_SETTINGS_DANGER_DOWN,
    CHAIN_MENU_ATTACK_TARGET, CHAIN_MENU_DROP_GOLD, CHAIN_MENU_DROP_INC, CHAIN_MENU_DROP_DEC,
    CHAIN_INIT_WORLD,
    CHAIN_SLOT_INC_ENTER, CHAIN_SLOT_DEC_ENTER,
    CHAIN_ENTER_SAVE_GAME_SLOT, CHAIN_SLOT_INC_REENTER, CHAIN_SLOT_DEC_REENTER,
    CHAIN_ENTER_SCENARIO_DISK, CHAIN_ENTER_CHAR_SELECT,
    CHAIN_CHAR_SELECT_INIT, CHAIN_CHAR_SELECT_NEXT, CHAIN_CHAR_SELECT_PREV,
    CHAIN_CHAR_SELECT_HERE, CHAIN_CHAR_SELECT_HOME,
    CHAIN_START_GAME_INPUT, CHAIN_CONTINUE_GAME, CHAIN_SAVE_GAME,
    CHAIN_CANCEL_SCENARIO, CHAIN_FORMAT_SCENARIO_DISK, CHAIN_USE_SCENARIO_DISK,
    CHAIN_RETIRE_CHARACTER,             /* ordinary; OTHER_OPTIONS option 5 */
    /* events.c */
    CHAIN_FLUSH_PENDING_XP, CHAIN_INIT_DROP_GOLD, CHAIN_ADJ_DROP_INC, CHAIN_ADJ_DROP_DEC,
    CHAIN_DROP_GOLD_EXEC,
    /* ai.c */
    CHAIN_SHOP_BUY_HANDLER, CHAIN_BROWSE_HANDLER,
};

/* ---- Shops ---------------------------------------------------------- */

/* SHOP_TABLE's records, kept at $7EDD. A slot's handler word is the
 * address of the original's routine for the item, one of the ten
 * below. No code sits at those addresses in this program, and
 * shop_buy_handler compares the word with them to pick the effect. */
struct shop_slot {
    uint16_t handler;       /* SHOP_*_ADDR, or junk in an unused slot */
    const uint8_t *name;    /* length byte, then high-bit ASCII */
    uint8_t price;          /* base price, times the location's tier + 1 */
};
struct shop {
    struct shop_slot slot[8];   /* slot n is chosen by bit 7 - n of the entry's mask */
    uint8_t scene;              /* +$28: the scene DISPLAY_SHOP plays */
};

enum {
    SHOP_BUY_EFFECT_0_ADDR = 0x720B,
    SHOP_BUY_EFFECT_1_ADDR = 0x7211,
    SHOP_BUY_EFFECT_2_ADDR = 0x7217,
    SHOP_BUY_EFFECT_3_ADDR = 0x721D,
    SHOP_BUY_EFFECT_4_ADDR = 0x7223,
    XP_AWARD_0_ADDR = 0x7249,
    XP_AWARD_1_ADDR = 0x724D,
    XP_AWARD_2_ADDR = 0x7251,
    XP_AWARD_3_ADDR = 0x7255,
    XP_AWARD_4_ADDR = 0x7259,
};

/* ---- Messages, scenes, and tunes ------------------------------------ */

/* DISPLAY_MESSAGE's message numbers, with the original's EQU names.
 * Several names describe a different message from the one the number
 * prints. Code that passes another number writes it as a constant. */
enum {
    MSG_DOOR_OPENS = 0x01,
    MSG_INSERT_DISK = 0x02,
    MSG_INSERT_GAME = 0x03,
    MSG_FEELING_WEAK = 0x09,
    MSG_LOTS_OF_FIGHT = 0x0A,
    MSG_HIT_WALL = 0x0B,
    MSG_HIT_STATUE = 0x0C,
    MSG_DEATH = 0x0D,
    MSG_ROCK_FALLS = 0x0F,
    MSG_HEAVY_ROCK = 0x10,
    MSG_DUST_CLOUD = 0x11,
    MSG_TOPPLES = 0x12,
    MSG_TOUCHED = 0x14,
    MSG_TOUGH_HIDE = 0x16,
    MSG_BREAKS_FREE = 0x1B,
    MSG_ESCAPE_FAIL = 0x1C,
    MSG_TACKLE_FAIL = 0x1D,
    MSG_FLYING_TACKLE = 0x1E,
    MSG_UNCONSCIOUS = 0x1F,
    MSG_REST_STRONGER = 0x20,
    MSG_REGAIN_CONSC = 0x21,
    MSG_JOINS_PARTY = 0x24,
    MSG_THANK_YOU = 0x25,
    MSG_SHUFFLES_OFF = 0x26,
    MSG_ARMOR_WORTHLESS = 0x2A,
    MSG_XP_GAINED = 0x2B,
    MSG_GOLD_LOST = 0x2C,
    MSG_GOLD_DROPPED = 0x2D,
    MSG_NO_GOLD = 0x2E,
    MSG_JOYSTICK_MODE = 0x2F,
    MSG_PRESS_KEY = 0x30,
    MSG_TOO_POOR = 0x31,
    MSG_INSERT_SCENARIO = 0x33,
    MSG_ENOUGH = 0x34,
    MSG_SAVED_ME = 0x35,
    MSG_NO_XP = 0x37,
    MSG_INPUT_PROMPT = 0x38,
    MSG_READ_ERROR = 0x3B,
};

/* SCENE_LOOP's scene for a character's retirement. Other scene numbers
 * appear as constants in the code that plays them. */
constexpr uint8_t SCENE_RETIRE = 0x23;

/* TUNE_PLAYER's tune numbers. The original passes the number times 2
 * in X, and tune_player takes the number itself. */
enum {
    TUNE_EMPTY = 0,
    TUNE_COMBAT_HIT = 1,            /* repeats `strength` times */
    TUNE_COMBAT_DAMAGE = 2,         /* repeats `strength` times */
    TUNE_ATTRACT = 3,               /* the title tune; only a key or button ends it */
    TUNE_MOB_DEATH = 4,
    TUNE_NPC_DEATH = 5,
    TUNE_COMBAT_AVOIDED = 6,
    TUNE_ENTITY_INTERACTION = 7,
    TUNE_AI_TURN = 8,
    TUNE_MENU_SELECTION = 9,
    TUNE_ENCOUNTER = 10,
    TUNE_MOVEMENT = 11,
};

/* ---- HRCG control characters ----------------------------------------- */

/* The control characters the game sends hrcg_cout, with the high bit
 * set as the game sends them. HRCG_CTRL_O makes the next control
 * character run its "shifted" variant. */
enum {
    HRCG_CTRL_A = 0x81,     /* the next character selects a font: '0' standard, '1'-'5' FONT_DATA */
    HRCG_CTRL_B = 0x82,     /* saves the parameters and the cursor */
    HRCG_CTRL_C = 0x83,     /* restores the column, with a line feed if it doesn't advance */
    HRCG_CTRL_D = 0x84,     /* restores the saved parameters */
    HRCG_CTRL_F = 0x86,     /* clears from the cursor to the end of the window */
    HRCG_CTRL_I = 0x89,     /* inverse video */
    HRCG_CTRL_K = 0x8B,     /* upper case */
    HRCG_CTRL_L = 0x8C,     /* lower case */
    HRCG_CTRL_N = 0x8E,     /* normal video */
    HRCG_CTRL_O = 0x8F,     /* shift the next control character */
    HRCG_CTRL_P = 0x90,     /* clears the window and homes the cursor */
    HRCG_CTRL_S = 0x93,     /* after HRCG_CTRL_O: scroll at the bottom of the window */
    HRCG_CTRL_V = 0x96,     /* sets the window's top left to the cursor */
    HRCG_CTRL_W = 0x97,     /* after HRCG_CTRL_O: wrap to the top of the window */
};

/* ---- Zero page -------------------------------------------------------- */

/*
    $00-$05 and $0A-$0F are reserved because the original reaches them
    through null pointers, and the C reproduces those accesses.
    PLAYER_ATTACK on an empty square writes through MOB_PTR = 0 or 1
    into $00-$03, and DRAW_MAP_ENTITIES and PICK_RANDOM_MEMBER read
    $02-$0C through MOB_PTR = 0. The compiler places nothing there.
*/
extern uint8_t NULL_PAGE_LOW [[c6502::zeropage(0x00)]][6];
extern uint8_t NULL_PAGE_HIGH [[c6502::zeropage(0x0A)]][6];

/* The byte stream TEXT_RENDERER reads: scene text, the status border,
 * and the title screen's text. */
extern const uint8_t *TEXT_STREAM_PTR [[c6502::zeropage(0x06)]];
/* The RWTS IOB, $B7E8, set once by INIT_DISK_IOB. */
extern struct rwts_iob *RWTS_IOB_PTR [[c6502::zeropage(0x08)]];

/* The Monitor's text window and cursor. The HRCG and the ROM routines
 * it calls read and write them, and so does the game. TEXT_COL and
 * TEXT_ROW are the game's names for the Monitor's CH and CV. c6502
 * rejects two objects at one address, so the Monitor's names aren't
 * declared, and hrcg.c uses the game's. */
extern uint8_t MONITOR_YSAV1_1F [[c6502::zeropage(0x1F)]];
extern uint8_t WNDLFT [[c6502::zeropage(0x20)]];
extern uint8_t WNDWDTH [[c6502::zeropage(0x21)]];
extern uint8_t WNDTOP [[c6502::zeropage(0x22)]];
extern uint8_t WNDBTM [[c6502::zeropage(0x23)]];
extern uint8_t TEXT_COL [[c6502::zeropage(0x24)]];
extern uint8_t TEXT_ROW [[c6502::zeropage(0x25)]];
/* $26-$27: GBASL/GBASH, which DOS's RWTS also uses. */
extern uint8_t MONITOR_GBAS [[c6502::zeropage(0x26)]][2];
/* The text-page line VTAB, VTABZ, HOME, CLREOP, and SCROLL leave. The
 * HRCG stores each character it draws at BASL[TEXT_COL] as well, which can
 * be several calls after the VTAB that set BASL. */
extern uint8_t *BASL [[c6502::zeropage(0x28)]];
/* $2A-$4F: the Monitor's BAS2L/BAS2H and the rest of its bytes, and
 * DOS's RWTS ($2A-$2F, $35, $38, $3C, $3E, $3F, $41, $44-$49).
 * RWTS_DISPATCH stores 0 to $48 after each RWTS call, which is
 * DOS_ZP[0x48 - 0x2A]. */
extern uint8_t DOS_ZP [[c6502::zeropage(0x2A)]][0x50 - 0x2A];
/* $50-$5F: the disk reader at $B300 swaps these with its own state. */
extern uint8_t DISK_VM_ZP [[c6502::zeropage(0x50)]][0x10];

/* Input's menu state. MENU_DESC_PTR is set by INPUT_LOOP and read by
 * the option handlers through STORE_SELECTION_TO_RECORD. */
extern struct menu *MENU_DESC_PTR [[c6502::zeropage(0xC0)]];
extern struct menu_option *MENU_OPTION_PTR [[c6502::zeropage(0xC2)]];
/* The tune player's position in the tune it plays. */
extern const uint8_t *TUNE_PC [[c6502::zeropage(0xE4)]];
/* The mob or event entry being looked at. Combat, the AI, and the
 * events point it at mobs, PICKUP_TREASURE at an event entry, and
 * PICK_RANDOM_MOB uses its low byte, *(uint8_t *)&MOB_PTR, as a counter
 * (MOB_PTR_LO in the original). */
extern struct mob *MOB_PTR [[c6502::zeropage(0xF4)]];
/* The mob whose turn it is, set by PROCESS_MOB_GROUPS. ENTER_LOCATION
 * points it at CANDIDATE_POS ($5A15), so that its pos byte is
 * PRNG_OUTPUT, and leaves it there. */
extern struct mob *ENTITY_PTR [[c6502::zeropage(0xF8)]];
/* The current location's record. */
extern struct location *CHAR_PTR [[c6502::zeropage(0xFA)]];

/* ---- The game's records, $4000-$59FF ------------------------------- */

/* The header's fields are read as ((struct location_header
 * *)LOCATIONS)->mob_base and so on. c6502 rejects a second object at
 * $4000, so the header has no name of its own. */
extern struct location LOCATIONS [[c6502::address(0x4000)]][63];
/* $FFFF after the last location. MAIN_ENTRY ends its walk when byte 1
 * of the record after the last ($4238) is $FF. */
extern const uint16_t LOCATIONS_END [[c6502::address(0x4237)]];
/* The danger setting, 0-$63. RANDOM_EVENTS fires an event when
 * PRNG_OUTPUT is below it. */
extern uint8_t EVENT_THRESHOLD [[c6502::address(0x4239)]];
extern uint8_t SCENE_COMBAT_FLAG [[c6502::address(0x423A)]];
extern uint8_t SCENE_P3_DONE [[c6502::address(0x423B)]];
extern uint8_t SCENE_P1_DONE [[c6502::address(0x423C)]];
extern uint8_t SCENE_HOSTILE_DONE [[c6502::address(0x423D)]];
extern uint8_t SCENE_EVENT_RESULT [[c6502::address(0x423E)]];
/* The mobs, $4C80-$547F. Their names follow at $5480. */
extern struct mob MOBS [[c6502::address(0x4C80)]][128];

/* ---- The variables of page $5A ----------------------------------- */

/*
    One declaration per address, since c6502 rejects two objects at one
    address. Where several EQU names share a byte, the declaration takes
    one of them, and the C uses it for all. The bytes UNITS.md lists as locals aren't
    declared: $5A11, $5A12, $5A14, $5A1A, $5A2A-$5A2C, $5A58, $5A60,
    $5A69, $5A6F, and $5A70. $5A2D-$5A50, $5A95, $5AA5, and $5AA8 hold
    leftover assembler text that nothing reads.
*/
extern uint8_t INPUT_MODE [[c6502::address(0x5A00)]];          /* 0 keyboard, 1 joystick */
extern uint8_t TOTAL_MOB_COUNT [[c6502::address(0x5A01)]];     /* player characters outside record 0 */
extern uint8_t CURRENT_PLAYER [[c6502::address(0x5A02)]];      /* location index being processed */
extern uint8_t CURRENT_ROOM [[c6502::address(0x5A03)]];        /* PROCESS_MOB_GROUPS's next mob index */
extern uint8_t CHAR_SPRITE [[c6502::address(0x5A04)]];         /* font character of ENTITY_PTR */
extern uint8_t CHAR_AI_MODE [[c6502::address(0x5A05)]];        /* font group of ENTITY_PTR, 0 = the player */
extern uint8_t AI_WAIT_FLAG [[c6502::address(0x5A06)]];
extern uint8_t BROWSE_CHAR_INDEX [[c6502::address(0x5A07)]];
extern uint8_t VIEW_MAX_COL [[c6502::address(0x5A08)]];
extern uint8_t VIEW_MAX_ROW [[c6502::address(0x5A09)]];
extern uint8_t VIEW_MIN_COL [[c6502::address(0x5A0A)]];
extern uint8_t VIEW_MIN_ROW [[c6502::address(0x5A0B)]];
/* RENDER_FONT_CHAR's cell and character. FONT_CHARSET 2 selects the
 * second pair of fonts. MANHATTAN_DISTANCE also writes FONT_COL and
 * FONT_ROW. */
extern uint8_t FONT_COL [[c6502::address(0x5A0C)]];
extern uint8_t FONT_ROW [[c6502::address(0x5A0D)]];
extern uint8_t FONT_CHARNUM [[c6502::address(0x5A0E)]];
extern uint8_t FONT_CHARSET [[c6502::address(0x5A0F)]];
extern uint8_t DAMAGE_AMOUNT [[c6502::address(0x5A10)]];
/* FIND_NEAREST_EVENT's result. When no entry qualifies it returns the
 * previous call's, so the byte carries state between calls. */
extern uint8_t NEAREST_POS [[c6502::address(0x5A13)]];
/* FIND_NEAREST_EVENT stores each candidate's position here. It is also
 * byte 0 of the record ENTER_LOCATION points ENTITY_PTR at, so that
 * the record's pos byte is PRNG_OUTPUT. */
extern uint8_t CANDIDATE_POS [[c6502::address(0x5A15)]];
extern uint8_t HEAL_NOTIFIED [[c6502::address(0x5A16)]];
/* Also ENTITY_SEARCH_POS, where the original's FIND_ENTITY_AT_POS
 * stores the position that its CHECK_AT_BOUNDARY reads. */
extern uint8_t LOCATION_ID [[c6502::address(0x5A17)]];
/* STEP_PRNG's state and output. Combat also stores RANDOM_IN_RANGE's
 * results in PRNG_OUTPUT ($1C13, $1DBB), which READ_KEYBOARD and
 * LOAD_SCENE_DATA read later. */
extern uint8_t PRNG_OUTPUT [[c6502::address(0x5A18)]];
extern uint8_t PRNG_STATE [[c6502::address(0x5A19)]];
extern uint8_t NUM_DIGIT [[c6502::address(0x5A1B)]];
extern uint16_t NUM_DIVISOR [[c6502::address(0x5A1C)]];
extern uint8_t NUM_LEADING [[c6502::address(0x5A1E)]];
extern uint8_t ENCOUNTER_RESULT [[c6502::address(0x5A1F)]];
extern uint8_t COLOCATE_LINK [[c6502::address(0x5A20)]];
extern uint8_t COLOCATE_POS [[c6502::address(0x5A21)]];
extern uint8_t COLOCATE_WRAPPED [[c6502::address(0x5A22)]];
extern uint8_t HOSTILE_APP [[c6502::address(0x5A23)]];
extern uint8_t HOSTILE_A [[c6502::address(0x5A24)]];
extern uint8_t HOSTILE_Y [[c6502::address(0x5A25)]];
extern uint8_t ADJACENT_THREAT [[c6502::address(0x5A26)]];
extern uint8_t ADJACENT_POS [[c6502::address(0x5A27)]];
/* REORDER_CHAR moves a mob from location SOURCE_CHAR to location
 * ACTIVE_CHAR. Callers set both just before the call. */
extern uint8_t SOURCE_CHAR [[c6502::address(0x5A28)]];
extern uint8_t ACTIVE_CHAR [[c6502::address(0x5A29)]];
extern uint8_t BOUNDS_MIN_COL [[c6502::address(0x5A51)]];
extern uint8_t BOUNDS_MAX_COL [[c6502::address(0x5A52)]];
extern uint8_t BOUNDS_MIN_ROW [[c6502::address(0x5A53)]];
extern uint8_t BOUNDS_MAX_ROW [[c6502::address(0x5A54)]];
/* The low byte of the CHAR_PTR that ENTER_LOCATION last drew. 0 makes
 * the next ENTER_LOCATION redraw. */
extern uint8_t LOCATION_POS [[c6502::address(0x5A55)]];
extern uint8_t LOCATION_STYLE [[c6502::address(0x5A56)]];
extern uint8_t MSG_LINE_COUNT [[c6502::address(0x5A57)]];
extern uint8_t EVENT_SLOT_BYTE [[c6502::address(0x5A59)]];
extern uint8_t EVENT_SUB_VALUE [[c6502::address(0x5A5A)]];
/* TRIGGER_SCENE_EVENT's copy of EVENT_PTR across COMMIT_MOVE
 * (RESIDUAL_5A_5A5B in main.asm). */
extern struct event *SAVED_EVENT_PTR [[c6502::address(0x5A5B)]];
extern uint8_t AI_PREV_DIR [[c6502::address(0x5A5D)]];
extern uint8_t EVENT_POS [[c6502::address(0x5A5E)]];
extern uint8_t EVENT_DEST_POS [[c6502::address(0x5A5F)]];
extern uint8_t TURN_START_COL [[c6502::address(0x5A61)]];
extern uint8_t TURN_START_ROW [[c6502::address(0x5A62)]];
extern uint8_t CURRENT_COL [[c6502::address(0x5A63)]];
extern uint8_t CURRENT_ROW [[c6502::address(0x5A64)]];
extern uint8_t MOVE_POINTS [[c6502::address(0x5A65)]];
extern uint8_t STEPS_TAKEN [[c6502::address(0x5A66)]];
extern uint8_t INPUT_DIR [[c6502::address(0x5A67)]];
extern uint8_t AI_BEST_DIST [[c6502::address(0x5A68)]];
/* NPC_MOVE_AI's score for each DIR_, lowest wins. */
extern uint8_t DIR_SCORES [[c6502::address(0x5A6A)]][5];
extern uint8_t AI_TARGET_POS [[c6502::address(0x5A71)]];
extern uint8_t AI_TARGET_COL [[c6502::address(0x5A72)]];
extern uint8_t AI_TARGET_ROW [[c6502::address(0x5A73)]];
extern uint8_t IS_PLAYER_TURN [[c6502::address(0x5A74)]];
extern uint8_t SAVED_MOB_LINK [[c6502::address(0x5A75)]];
extern uint8_t COMBAT_WILLINGNESS [[c6502::address(0x5A76)]];
extern uint8_t TARGET_DEFENSE [[c6502::address(0x5A77)]];
extern uint8_t COMBAT_STRENGTH [[c6502::address(0x5A78)]];
extern uint8_t MOB_DEFENSE [[c6502::address(0x5A79)]];
/* The drop-gold menu's amount and step. DROP_GOLD_EXEC compares the
 * amount as a signed number. */
extern uint16_t DROP_AMOUNT [[c6502::address(0x5A7A)]];
extern uint16_t DROP_STEP [[c6502::address(0x5A7C)]];
extern uint8_t STAT_SCALE [[c6502::address(0x5A7E)]];
extern uint8_t STAT_LEVEL_BITS [[c6502::address(0x5A7F)]];
/* The shop's state across its menu round trips. */
extern uint8_t SHOP_ITEM_MASK [[c6502::address(0x5A80)]];
extern uint16_t SHOP_PRICE [[c6502::address(0x5A81)]];
extern uint8_t SHOP_VALID_MASK [[c6502::address(0x5A83)]];
extern uint8_t LOCATION_STYLE_BITS [[c6502::address(0x5A84)]];   /* the shop's tier, 0-3 */
extern uint8_t SHOP_ITEM_OFFSET [[c6502::address(0x5A85)]];      /* slot * 5 */
extern struct mob *TARGET_PTR [[c6502::address(0x5A86)]];
extern uint8_t BEST_THREAT [[c6502::address(0x5A88)]];
extern uint8_t SCENE_FOUND_FLAG [[c6502::address(0x5A89)]];
extern uint8_t SCENE_HOSTILE_FLAG [[c6502::address(0x5A8A)]];
extern uint8_t SCENE_GROUP_IDX [[c6502::address(0x5A8B)]];
/* The number of ROM WAIT calls TIMED_WAIT makes. TUNE_ADVANCE halves
 * it at every step of a tune, and IDLE_ANIM_HANDLER counts it down. */
extern uint8_t WAIT_LOOP_COUNT [[c6502::address(0x5A8C)]];
extern uint8_t TUNE_LOOP_CTR [[c6502::address(0x5A8D)]];
extern const uint8_t *TUNE_LOOP_ADDR [[c6502::address(0x5A8E)]];
extern const uint8_t *TUNE_GOSUB_ADDR [[c6502::address(0x5A90)]];
extern uint8_t TUNE_PRNG_MASK [[c6502::address(0x5A92)]];
/* $7A7F (PLAY_HIGH_TONE) or $7A91 (PLAY_LOW_TONE), the original's tone
 * vector. sound.c stores those values and compares with them. */
extern uint16_t TUNE_ACTION_VEC [[c6502::address(0x5A93)]];
extern uint8_t TEXT_RIGHT_MARGIN [[c6502::address(0x5A96)]];
extern uint8_t TEXT_LEFT_MARGIN [[c6502::address(0x5A97)]];
extern uint8_t TEXT_STREAM_IDX [[c6502::address(0x5A98)]];
extern uint8_t SCENE_NUMBER [[c6502::address(0x5A99)]];
/* SCENE_LOOP's copies of ENTITY_PTR and MOB_PTR, which it restores
 * before it returns. */
extern struct mob *SAVED_ENTITY_PTR [[c6502::address(0x5A9C)]];
extern struct mob *SAVED_MOB_PTR [[c6502::address(0x5AA0)]];
/* The blinking cell. BLINK_COL of 20 or more turns the blink off. */
extern uint8_t BLINK_COL [[c6502::address(0x5AA2)]];
extern uint8_t BLINK_ROW [[c6502::address(0x5AA3)]];
extern uint8_t BLINK_CHAR [[c6502::address(0x5AA4)]];
/* START_MOB_TURN stores CHAR_AI_MODE here, and nothing reads it. */
extern uint8_t SAVED_AI_MODE [[c6502::address(0x5AA6)]];
extern uint8_t ATTRACT_FLAG [[c6502::address(0x5AA7)]];
/* $16 while COMMIT_MOVE's tune hasn't played. Initially $D9. */
extern uint8_t TURN_TUNE_GUARD [[c6502::address(0x5AA9)]];
extern uint8_t INPUT_OPTION_COUNT [[c6502::address(0x5AAA)]];
/* The highlighted option of the menu INPUT_LOOP runs, 1-based. */
extern uint8_t CURRENT_SELECTION [[c6502::address(0x5AAB)]];
/* The three menu rows, $5AAC-$5B23: 40 high-bit characters to a row,
 * which CLEAR_INPUT_BUFFER fills with $A0. FIND_PREV_PC's two scratch
 * bytes at [0] and [1] are locals in C. */
extern uint8_t INPUT_BUFFER [[c6502::address(0x5AAC)]][120];

/* ---- Other kept memory ------------------------------------------------ */

/* The reset vector and its check byte, which SETPWRC sets. */
extern uint16_t SOFTEV [[c6502::address(0x03F2)]];
extern uint8_t PWREDUP [[c6502::address(0x03F4)]];

/* The scene sector LOAD_SCENE_FROM_DISK reads, over the original's
 * border code. The disk-signature routines use its first bytes too.
 * Its last byte, SCENE_TEXT_DATA[0xFF] (SCENE_TRANSITION in the
 * original), is nonzero for a new scene. */
extern uint8_t SCENE_TEXT_DATA [[c6502::address(0x5C00)]][256];

/*
    The stats screen, $7D83-$7E00, a length byte ($7D) and 125
    characters, which DISPLAY_CHAR_STATS fills in and prints with
    print_string. Each array of 5 is where NUM_TO_DECIMAL writes a
    number, and only its last places are digits on the screen. The
    places before them hold the text in front of the number ("CE " of
    "LIFE FORCE" in `life`), which NUM_TO_DECIMAL leaves alone as long
    as the number has no digit there. The comments give the original's
    _OUT label of each array and the label of its first digit on the
    screen.
*/
struct stats_screen {
    uint8_t length;         /* $7D */
    uint8_t text_0[9];
    uint8_t life[5];        /* +$0A STATS_LIFE_OUT; [3] STATS_LIFE */
    uint8_t text_1[10];
    uint8_t strength[5];    /* +$19 STATS_STR_OUT; [3] STATS_STR */
    uint8_t text_2[21];
    uint8_t ducats[5];      /* +$33 STATS_DUCATS_OUT; [1] STATS_DUCATS */
    uint8_t text_3[11];
    uint8_t speed[5];       /* +$43 STATS_SPEED_OUT; [3] STATS_SPEED */
    uint8_t text_4[7];
    uint8_t sword[5];       /* +$4F STATS_SWORD_OUT; [3] STATS_SWORD */
    uint8_t text_5[10];
    uint8_t armor[5];       /* +$5E STATS_WEAP_OUT; [4] STATS_ARMOR */
    uint8_t text_6[10];
    uint8_t dexterity[5];   /* +$6D STATS_DEXT_OUT; [3] STATS_DEXT */
    uint8_t text_7[7];
    uint8_t dagger[5];      /* +$79 STATS_DAGGR_OUT; [3] STATS_DAGGR */
};
extern struct stats_screen STATS_TEMPLATE [[c6502::address(0x7D83)]];
extern const uint8_t STATS_PROMPT_KEYBOARD [[c6502::address(0x7E01)]][0x17];
extern const uint8_t STATS_PROMPT_JOYSTICK [[c6502::address(0x7E18)]][0x17];
/* The status bar's text stream, which DISPLAY_STATUS_BAR renders. */
extern const uint8_t STATUS_BORDER_DATA [[c6502::address(0x7E2F)]][0x7E97 - 0x7E2F];
extern const uint8_t S_PRESS_SPACE [[c6502::address(0x7E97)]][0x23];
extern const uint8_t S_PRESS_BUTTON [[c6502::address(0x7EBA)]][0x23];

/* The weapon shop (type $D5) and the armor shop ($D6). */
extern const struct shop SHOP_TABLE [[c6502::address(0x7EDD)]][2];
/* The base value APPLY_SHOP_EFFECT writes to sword or dagger, and the
 * value AWARD_XP writes to armor, for effects 0-4. */
extern const uint8_t SHOP_EFFECT_TABLE [[c6502::address(0x7F6D)]][5];
extern const uint8_t XP_AWARD_TABLE [[c6502::address(0x7FBE)]][5];

/* CLASSIFY_ENTITY_TYPE's nine names, each a length byte and high-bit
 * ASCII, one after another in MAP_OBJECT_NAMES, $7FC3-$8039. The
 * comments give each one's offset in the table. */
extern const uint8_t S_A_WALL [[c6502::address(0x7FC3)]][7];                /* +$00 */
extern const uint8_t S_A_STATUE [[c6502::address(0x7FCA)]][9];              /* +$07 */
extern const uint8_t S_A_TREASURE_CHEST [[c6502::address(0x7FD3)]][17];     /* +$10 */
extern const uint8_t S_EMPTY_SPACE [[c6502::address(0x7FE4)]][12];          /* +$21 */
extern const uint8_t S_AN_ILLUSION [[c6502::address(0x7FF0)]][12];          /* +$2D */
extern const uint8_t S_A_DOOR [[c6502::address(0x7FFC)]][7];                /* +$39 */
extern const uint8_t S_AN_INVISIBLE_WALL [[c6502::address(0x8003)]][19];    /* +$40 */
extern const uint8_t S_SOME_RUNE_WRITING [[c6502::address(0x8016)]][18];    /* +$53 */
extern const uint8_t S_A_TRADING_OUTPOST [[c6502::address(0x8028)]][18];    /* +$65 */
/* Combat's verbs, length-prefixed strings one after another. */
extern const uint8_t ATTACK_VERB_STRINGS [[c6502::address(0x803A)]][0x80A4 - 0x803A];

/* RENDER_FONT_CHAR's string, which draws a 2 x 2 sprite. The selector
 * and the glyphs are written before each print. The comments give the
 * original's labels. */
struct font_char_string {
    uint8_t length;         /* 9 */
    uint8_t ctrl_a;         /* HRCG_CTRL_A */
    uint8_t charset;        /* CHAR_CHARSET: the font selector */
    uint8_t ctrl_b;         /* HRCG_CTRL_B */
    uint8_t upper_left;     /* CHAR_UPPER_LEFT */
    uint8_t upper_right;    /* CHAR_UPPER_RIGHT */
    uint8_t ctrl_c;         /* HRCG_CTRL_C */
    uint8_t lower_left;     /* CHAR_LOWER_LEFT */
    uint8_t lower_right;    /* CHAR_LOWER_RIGHT */
    uint8_t ctrl_d;         /* HRCG_CTRL_D */
};
extern struct font_char_string s_PRINT_FONT_CHAR [[c6502::address(0x80A4)]];

/* The tunes' bytecode. TUNE_TABLE points at TUNE_0..TUNE_2 here and at
 * TUNE_3..TUNE_11 in $A300-$A440. TUNE_PLAYER writes the repeat count
 * into TUNE_1_COMBAT_HIT[14] ($80D5) and TUNE_2_COMBAT_DAMAGE[12]
 * ($80E3). */
extern const uint8_t *const TUNE_TABLE [[c6502::address(0x80AE)]][12];
extern uint8_t TUNE_0 [[c6502::address(0x80C6)]][1];
extern uint8_t TUNE_1_COMBAT_HIT [[c6502::address(0x80C7)]][16];
extern uint8_t TUNE_2_COMBAT_DAMAGE [[c6502::address(0x80D7)]][14];

/* Five fonts of 96 glyphs, 8 bytes each. The HRCG computes font n's
 * address as FONT_DATA + (n - 1) * $300 for selectors '1'-'9', so '6'
 * and up read whatever follows. */
extern const uint8_t FONT_DATA [[c6502::address(0x83A5)]][5][0x300];

/* The messages, length-prefixed strings that the location header's
 * msg_table points into.
 * integrity_fail writes a random byte at [1 + PRNG_OUTPUT]. */
extern uint8_t GAME_MESSAGE_STRINGS [[c6502::address(0x9AA5)]][0xA264 - 0x9AA5];

/* The disk reader's code, $B300-$B442 (DISK_IO_START to DISK_IO_END in
 * the original), which VERIFY_INTEGRITY checksums. DISK_READ_FIELD
 * below is its first routine. */
extern const uint8_t DISK_IO_CODE [[c6502::address(0xB300)]][0xB443 - 0xB300];

/* ---- DOS's RWTS and the disk reader ------------------------------------ */

enum {
    RWTS_SEEK = 0,
    RWTS_READ = 1,
    RWTS_WRITE = 2,
    RWTS_FORMAT = 4,
};

/* The RWTS input/output block at $B7E8. */
struct rwts_iob {
    uint8_t table_type;             /* 1 */
    uint8_t slot_times_16;
    uint8_t drive_number;
    uint8_t volume;                 /* +3: 0 for any */
    uint8_t track;                  /* +4 */
    uint8_t sector;                 /* +5 */
    void *dct_addr;
    uint8_t *buffer_addr;           /* +8 */
    uint8_t unused;
    uint8_t partial_byte_count;     /* +$0B */
    uint8_t command;                /* +$0C: RWTS_ */
    uint8_t ret_code;
    uint8_t last_volume;
    uint8_t last_slot_times_16;
    uint8_t last_drive_number;
};
extern struct rwts_iob RWTS_IOB [[c6502::address(0xB7E8)]];

/* DOS's RWTS, behind the routine that disables interrupts around it.
 * It takes the IOB's address with the low byte in Y and the high byte
 * in A, and sets the carry when the access failed. */
[[c6502::zp_abi, c6502::return_bool_in_carry, c6502::address(0xB7B5)]]
bool RWTS_ENTRY(struct rwts_iob *iob [[c6502::in_ya]]);

/* Reads and checks the copy-protected data field with the head on
 * track 5, after RWTS has seeked there. Returns true when the check
 * failed (the carry). It swaps $50-$5F with its own state and puts
 * them back. */
[[c6502::zp_abi, c6502::return_bool_in_carry, c6502::address(0xB300)]]
bool DISK_READ_FIELD(void);

/* The disk controller's boot ROM in slot 6, which reboots. */
[[noreturn, c6502::zp_abi, c6502::address(0xC600)]] void DISK_BOOT_ROM(void);

/* ---- The Monitor ROM ---------------------------------------------------- */

/* Sets PWREDUP from SOFTEV, so that RESET goes to SOFTEV. */
[[c6502::zp_abi, c6502::address(0xFB6F)]] void SETPWRC(void);
/* Waits (26 + 27a + 5a^2) / 2 cycles. CALL_ROM_WAIT passes $50, 17,093
 * cycles, the game's unit of time. */
[[c6502::zp_abi, c6502::preserves_x, c6502::preserves_y, c6502::address(0xFCA8)]]
void WAIT(register uint8_t a);
/* Sets BASL to the start of line TEXT_ROW. VTABZ takes the line in A. */
[[c6502::zp_abi, c6502::preserves_x, c6502::preserves_y, c6502::address(0xFC22)]]
void VTAB(void);
[[c6502::zp_abi, c6502::preserves_x, c6502::preserves_y, c6502::address(0xFC24)]]
void VTABZ(register uint8_t row);
/* Clears the text page from the cursor to the end of the window. */
[[c6502::zp_abi, c6502::preserves_x, c6502::address(0xFC42)]] void CLREOP(void);
/* Clears the window on the text page and homes the cursor. */
[[c6502::zp_abi, c6502::preserves_x, c6502::address(0xFC58)]] void HOME(void);
/* Scrolls the window on the text page up a line. */
[[c6502::zp_abi, c6502::preserves_x, c6502::address(0xFC70)]] void SCROLL(void);
/* Clears the text page from the cursor to the end of the line. Only
 * the HRCG's ctrl-E calls it, and the game never sends ctrl-E. */
[[c6502::zp_abi, c6502::preserves_x, c6502::address(0xFC9C)]] void CLREOL(void);

/* ---- Soft switches ------------------------------------------------------ */

/* Bit 7 set: a key is waiting, high-bit ASCII. */
extern volatile uint8_t KBD [[c6502::address(0xC000)]];
/* Any access clears the key. The game writes it with STA. */
extern volatile uint8_t KBDSTRB [[c6502::address(0xC010)]];
extern volatile uint8_t SPKR [[c6502::address(0xC030)]];
extern volatile uint8_t TXTCLR [[c6502::address(0xC050)]];
extern volatile uint8_t MIXCLR [[c6502::address(0xC052)]];
extern volatile uint8_t TXTPAGE1 [[c6502::address(0xC054)]];
extern volatile uint8_t HIRES [[c6502::address(0xC057)]];
/* Bit 7 set: button 0 or 1 is down. The original's EQU calls $C061
 * PADDL0. TUNE_ADVANCE reads both with LDA $C061,X. */
extern volatile uint8_t BUTN [[c6502::address(0xC061)]][2];
/* Bit 7 set until paddle 0's or 1's timer runs out after PTRIG. The C
 * reads the paddles with these in place of ROM PREAD. */
extern volatile uint8_t PADDL [[c6502::address(0xC064)]][2];
extern volatile uint8_t PTRIG [[c6502::address(0xC070)]];
/* Slot 6's drive motor on, which LOAD_SCENE_FROM_DISK reads. */
extern volatile uint8_t DISK_MOTOR_ON [[c6502::address(0xC0E9)]];

/* ---- main.c -------------------------------------------------------------- */

/* Runs the game: GAME_INIT, the attract screen, and the loop over the
 * locations, which never ends. MAIN_ENTRY, $0800, entered by the loader
 * with nothing in particular in the registers. It is in section .main,
 * which the linker script puts at $0800. */
[[c6502::section(".main")]] int main(void);
/* Flips SOUND_ON and the "SOUND IS ON" option's last two characters,
 * then returns CHAIN_MENU_GAME_SETTINGS. TOGGLE_SOUND $0984, a menu
 * exit (GAME_SETTINGS option 5). */
uint8_t toggle_sound(void);
/* Writes EVENT_THRESHOLD into the "DANGER :" option. SETTINGS_DISPLAY_INIT
 * $09AC, called by input's MENU_SETTINGS_INIT. */
void settings_display_init(void);
/* Step EVENT_THRESHOLD down or up by one, held within 0-$63.
 * DEC_THRESHOLD_WRAP $09CB and INC_THRESHOLD_WRAP $09CF, which JSR
 * DEC_EVENT_THRESHOLD and INC_EVENT_THRESHOLD. */
void dec_threshold_wrap(void);
void inc_threshold_wrap(void);

/* ---- init.c -------------------------------------------------------------- */

/* Sets up the machine and the game, runs the save/load menu, and counts
 * the player characters. GAME_INIT $78C7, called by MAIN_ENTRY. */
void game_init(void);
/* Draws one step of the title screen's border animation. The first call
 * starts it as GAME_ACTION_HANDLER does, and each later call draws with
 * the handler the previous call chose and chooses the next. tune_player
 * calls it after each note while BORDER_ANIM_ON is true, in place of
 * the original's JMP (DISPATCH_VEC) at $7A49. */
void border_anim_step(void);
/* True until init_game_state's title tune has played. Initially true. */
extern bool BORDER_ANIM_ON;

/* ---- turn.c -------------------------------------------------------------- */

/* Runs the chain from `state` until a routine returns CHAIN_RETURN.
 * Only start_mob_turn (main.c) and game_init (init.c) call it. */
void run_chain(uint8_t state);
/* Moves the current mob's sprite to CURRENT_COL, CURRENT_ROW, then does
 * reset_turn_pos. COMMIT_MOVE $1B52, which falls into RESET_TURN_POS. */
void commit_move(void);
/* Redraws the turn's starting cell, makes the current cell the starting
 * cell, and plays TUNE_MOVEMENT once after a commit_move.
 * RESET_TURN_POS $1B81. */
void reset_turn_pos(void);
/* Sets bits 1-0 of ENTITY_PTR->flags to `state`. SET_CHAR_STATE $1BE0,
 * state in A. */
void set_char_state(uint8_t state);
/* Clears or sets FLAGS_ENGAGED_BIT of ENTITY_PTR->flags.
 * CLEAR_RESTRICTED $1BEF, SET_RESTRICTED $1BF8. */
void clear_restricted(void);
void set_restricted(void);
/* 1 while a mob's turn is in progress, read by CONFIGURE_INPUT. It was a
 * code byte of INIT_GAME_STATE ($5B27) in the original. */
extern uint8_t TURN_ACTIVE;

/* ---- input.c ------------------------------------------------------------- */

/* Runs menu `m`: draws it, moves the highlight with the keys or the
 * joystick, and returns the chosen option's exit state without running
 * it. INPUT_LOOP $5F19, descriptor in X (high) and Y (low). */
uint8_t input_loop(struct menu *m);
/* Points MENU_DESC_PTR at `m`, runs its init, and draws its options with
 * none highlighted. SETUP_INPUT $5F60, descriptor in X and Y. */
void setup_input(struct menu *m);
/* Returns &GAME_INPUT_TABLE. GET_INPUT_TABLE_PTR $60CD (X high, Y low). */
struct menu *get_input_table_ptr(void);
/* Enables and disables the action menu's options for the current mob,
 * and returns the MOVE option's enable byte, which GAME_TURN_LOOP tests
 * next. CONFIGURE_INPUT $60D2, also GAME_INPUT_TABLE's init. */
uint8_t configure_input(void);
/* Waits for a direction or fire from the keyboard or the joystick,
 * running the idle animation and the integrity check meanwhile, and
 * returns DIR_NONE for fire or DIR_UP..DIR_RIGHT. READ_KEYBOARD $A44C,
 * result in A with Z set for fire. */
uint8_t read_keyboard(void);
/* Takes the current character out of play and plays SCENE_RETIRE.
 * RETIRE_CHARACTER $09E7, a menu exit (OTHER_OPTIONS option 5). */
void retire_character(void);

/* The chain routines of the menus. Each returns the next CHAIN_ state.
 * The labels and addresses are in UNITS.md. */
uint8_t teardown_input(void);           /* TEARDOWN_INPUT $60C7: the action menu */
uint8_t menu_see_your_lot(void);        /* MENU_SEE_YOUR_LOT $6101: the stats screen */
uint8_t menu_check_attack(void);        /* MENU_CHECK_ATTACK $6107 (and ENTER_ATTACK_MENU $6145) */
uint8_t menu_other_options(void);       /* MENU_OTHER_OPTIONS $618C */
uint8_t menu_game_settings(void);       /* MENU_GAME_SETTINGS $61F1, falls into _ENTER */
uint8_t menu_game_settings_enter(void); /* MENU_GAME_SETTINGS_ENTER $61F6 */
uint8_t settings_speed_up(void);        /* SETTINGS_SPEED_UP $6209 */
uint8_t settings_speed_down(void);      /* SETTINGS_SPEED_DOWN $6220 */
uint8_t settings_danger_up(void);       /* SETTINGS_DANGER_UP $622D */
uint8_t settings_danger_down(void);     /* SETTINGS_DANGER_DOWN $6236 */
uint8_t menu_attack_target(void);       /* MENU_ATTACK_TARGET $6261 */
uint8_t menu_drop_gold(void);           /* MENU_DROP_GOLD $62A2 */
uint8_t menu_drop_inc(void);            /* MENU_DROP_INC $62AD ("@") */
uint8_t menu_drop_dec(void);            /* MENU_DROP_DEC $62B3 ("#") */
uint8_t init_world(void);               /* INIT_WORLD $62E5: the save/load menu */
uint8_t slot_inc_enter(void);           /* SLOT_INC_ENTER $630F */
uint8_t slot_dec_enter(void);           /* SLOT_DEC_ENTER $6315 */
uint8_t enter_save_game_slot(void);     /* ENTER_SAVE_GAME_SLOT $636C */
uint8_t slot_dec_reenter(void);         /* SLOT_DEC_REENTER $6373 */
uint8_t slot_inc_reenter(void);         /* SLOT_INC_REENTER $6379 */
uint8_t enter_scenario_disk(void);      /* ENTER_SCENARIO_DISK $63A1 */
uint8_t enter_char_select(void);        /* ENTER_CHAR_SELECT $63E8 */
uint8_t char_select_init(void);         /* CHAR_SELECT_INIT $63EF */
uint8_t char_select_next(void);         /* CHAR_SELECT_NEXT $63FF */
uint8_t char_select_prev(void);         /* CHAR_SELECT_PREV $6408 */
uint8_t char_select_here(void);         /* CHAR_SELECT_HERE $6411 */
uint8_t char_select_home(void);         /* CHAR_SELECT_HOME $641A */
uint8_t start_game_input(void);         /* START_GAME_INPUT $6458: the shop's menu */
uint8_t continue_game(void);            /* CONTINUE_GAME $69C8 */
uint8_t save_game(void);                /* SAVE_GAME $6A47 */
uint8_t cancel_scenario(void);          /* CANCEL_SCENARIO $6A6E */
uint8_t format_scenario_disk(void);     /* FORMAT_SCENARIO_DISK $6A7F (and SCENARIO_INSERT_PROMPT) */
uint8_t use_scenario_disk(void);        /* USE_SCENARIO_DISK $6AA1 */

/* The menus, $607D-$6457 in the original. Their options are static in
 * input.c. Byte 1 (selection) and some options' enable bytes are
 * written at run time. */
extern struct menu GAME_INPUT_TABLE;
extern struct menu ATTACK_DIR_TABLE;
extern struct menu OTHER_OPTIONS_TABLE;
extern struct menu GAME_SETTINGS_TABLE;
extern struct menu ATTACK_TARGET_TABLE;
extern struct menu DROP_GOLD_TABLE;
extern struct menu SAVE_LOAD_TABLE;        /* WORLD_INIT_DATA in the original */
extern struct menu SAVE_GAME_SLOT_TABLE;
extern struct menu SCENARIO_DISK_TABLE;
extern struct menu CHAR_SELECT_TABLE;
extern struct menu ALT_INPUT_TABLE;

/*
    The menu strings other units write, each a length byte and high-bit
    ASCII. Most menu strings are fixed text and are plain uint8_t arrays
    in input.c. A string with a part that the program fills in before
    it's shown, such as a number or a name, is a struct instead. Its
    members are the parts that change and the fixed text around them,
    in the original's order, so the string's bytes are still one run
    that print_string prints by its length byte. Code that prints the
    whole string passes the struct's address as a uint8_t pointer, and
    code that fills in a part writes the member, where the original
    writes through a label inside the string. The member comments give
    those labels.

    The initial bytes of a part that changes are what the original's
    image holds there, which a menu shows until the part is written.
    NUM_TO_DECIMAL writes digits $B0-$B9 and skips leading zeros, so its
    callers first fill a number's 5 places with $20, a low-ASCII space.
    A number in a string that shows ducats is followed by a fixed "0",
    so the menu shows ten times the value written.
*/

/* The sound option of the game settings menu, "SOUND IS ON " or "SOUND
 * IS OFF". toggle_sound (main.c) writes the last two characters each
 * time the option is chosen. */
struct sound_is_on_string {     /* $7B04 */
    uint8_t text[11];           /* the length, 12, and "SOUND IS O" */
    uint8_t state[2];           /* S_SOUND_CHAR1 and S_SOUND_CHAR2: "N " or "FF" */
};
/* The danger setting in the game settings menu, "DANGER :" and
 * EVENT_THRESHOLD in decimal. settings_display_init (main.c) writes the
 * number each time the menu is entered. */
struct danger_string {          /* $7B1B "DANGER :   " */
    uint8_t text[7];            /* the length, 11, and "DANGER" */
    /* S_DANGER_OUT, NUM_TO_DECIMAL's 5 places. The danger is at most 99,
     * so [0] and [1] keep " :". */
    uint8_t number[5];
};
/* The character select menu's option to add a player character to the
 * party, "ADD" and the character's name. copy_pc_name_to_buf (util.c)
 * writes the name of the character the menu is showing. */
struct add_name_string {        /* $7B47 */
    uint8_t text[4];            /* the length, 24, and "ADD" */
    uint8_t name[21];           /* CHAR_SELECT_NAME: a space, then the name in [1..20] */
};
/* $7B84-$7BDA: "THE TREASURE CHEST OPENS UP", "TO REVEAL n0 GOLD
 * DUCATS.", "YOU NOW HAVE n0 DUCATS.", and a $00, printed as one group
 * by DISPLAY_MESSAGE_LOOP from `opens`. pickup_treasure (events.c)
 * writes the gold in the chest and the character's gold with it added
 * into the two sets of digits, after clear_treasure_digits fills them
 * with spaces. */
struct chest_message {
    uint8_t opens[28];          /* S_CHEST_OPENS: the length, 27, and the text */
    uint8_t reveal_gold[11];    /* S_REVEAL_GOLD: the length, 29, and "TO REVEAL " */
    uint8_t reveal_digits[5];   /* S_REVEAL_DIGITS, NUM_TO_DECIMAL's 5 places */
    uint8_t reveal_rest[14];    /* "0 GOLD DUCATS." */
    uint8_t now_have_gold[14];  /* S_NOW_HAVE_GOLD: the length, 27, and "YOU NOW HAVE " */
    uint8_t now_have_digits[5]; /* S_NOW_HAVE_DIGITS, NUM_TO_DECIMAL's 5 places */
    uint8_t now_have_rest[9];   /* "0 DUCATS." */
    uint8_t end;                /* 0, which ends the group */
};
/* The attack target menu's option to attack a mob, "ATTACK" and the
 * mob's name. init_attack_display (combat.c) writes MOB_PTR's name. */
struct attack_target_string {   /* $7C94 */
    uint8_t text[7];            /* the length, 27, and "ATTACK" */
    /* ATTACK_NAME_BUF: a space, then the name in [1..20], padded with
     * $20. */
    uint8_t name[21];
};
/* The amount in the drop gold menu, "DUCATS TO DROP : n0".
 * init_drop_display (events.c) writes DROP_AMOUNT each time the menu
 * is entered. */
struct ducats_to_drop_string {  /* $7CD9 */
    uint8_t text[18];           /* the length, 23, and "DUCATS TO DROP : " */
    uint8_t number[5];          /* DROP_DISPLAY_BUF, NUM_TO_DECIMAL's 5 places */
    uint8_t zero;               /* the "0" after them */
};
/* A shop's offer, "PAY n0 FOR " and an item's name. show_item (ai.c)
 * writes the item's price at this location, SHOP_PRICE, and its name,
 * after filling both with spaces. */
struct pay_for_string {         /* $7CFF */
    uint8_t text[5];            /* the length, 35, and "PAY " */
    uint8_t price[5];           /* SHOP_PRICE_DIGITS, NUM_TO_DECIMAL's 5 places */
    uint8_t text_2[6];          /* "0 FOR " */
    uint8_t item[20];           /* SHOP_ITEM_NAME */
};
extern struct sound_is_on_string S_SOUND_IS_ON;
extern struct danger_string S_DANGER;
extern struct add_name_string S_ADD_NAME;
extern struct chest_message S_CHEST_MESSAGE;
extern struct attack_target_string S_ATTACK_TARGET;
extern uint8_t S_MISSES[7];                             /* $7CD2 "MISSES" */
extern struct ducats_to_drop_string S_DUCATS_TO_DROP;
extern struct pay_for_string S_PAY_FOR;

/* The game disk's signature sector, track $11 sector 0. */
extern const uint8_t GAME_SIG_TRACK;
extern const uint8_t GAME_SIG_SECTOR;

/* ---- combat.c ------------------------------------------------------------ */

/* Sets ENCOUNTER_RESULT to 0 when no mob is at `pos`, 1 for a friendly
 * one, and 2 for a hostile one, and returns it. Leaves MOB_PTR on the mob
 * it found. CHECK_ENCOUNTER $0E25, pos in A. */
uint8_t check_encounter(uint8_t pos);
/* Returns true when MOB_PTR and `other` are of hostile factions.
 * CHECK_HOSTILE $0EB1, other in TARGET_REC ($BC), result 1 or 0 in A. */
bool check_hostile(const struct mob *other);
/* Sums the hostile strength in the four cells next to `pos` into
 * ADJACENT_THREAT, and returns 1 when ENTITY_PTR's defense is at least
 * that, else 0. CHECK_ADJACENT_THREATS $0EF1, pos in A. */
uint8_t check_adjacent_threats(uint8_t pos);
/* Points MOB_PTR at a random living player character of CHAR_PTR's
 * list, or at 0 when there is none. PICK_RANDOM_MEMBER $12D0. */
void pick_random_member(void);
/* Damages ENTITY_PTR by `damage`. APPLY_DAMAGE_TO_CURRENT $1425, damage
 * in A. */
void apply_damage_to_current(uint8_t damage);
/* Restores some of ENTITY_PTR's life at random when it is below 40.
 * AUTO_HEAL $143F, also a menu exit (GAME_INPUT option 6, REST). */
void auto_heal(void);
/* Checks the encounter at `pos`, sets the restricted flag from it, and
 * puts ENTITY_PTR in combat state 2. ENTER_COMBAT_STATE $14B7, also
 * GAME_INPUT option 3 (DEFEND), pos in A: 2 from
 * TURN_DISPATCH, 7 from MOVE_LOOP, and 0x14 from the menu, whose
 * JMP (INPUT_JMP_VEC) leaves the handler's high byte in A. */
void enter_combat_state(uint8_t pos);
/* Puts ENTITY_PTR in combat state 3 and resolves an attack on
 * TARGET_PTR, which select_combat_target chose. START_COMBAT $14CD. */
void start_combat(void);
/* Step CURRENT_COL or CURRENT_ROW one cell and go on to
 * find_attack_target. A step that leaves the byte negative falls into
 * the next entry instead, as in the original. ATTACK_RIGHT $14D8,
 * ATTACK_LEFT $14DD, ATTACK_UP $14E2, ATTACK_DOWN $14E7, menu exits. */
uint8_t attack_right(void);
uint8_t attack_left(void);
uint8_t attack_up(void);
uint8_t attack_down(void);
/* Chooses the mob to attack at CURRENT_COL, CURRENT_ROW.
 * FIND_ATTACK_TARGET $14EA. */
uint8_t find_attack_target(void);
/* Moves MOB_PTR to the next mob in the same cell and reopens the attack
 * menu. SELECT_SOMEONE_ELSE $155E, a menu exit. */
uint8_t select_someone_else(void);
/* Writes MOB_PTR's name into S_ATTACK_TARGET.name. INIT_ATTACK_DISPLAY $1537,
 * called by input's MENU_ATTACK_INIT. */
void init_attack_display(void);
/* Returns true when a hostile mob shares ENTITY_PTR's cell, leaving
 * MOB_PTR on it. CHECK_ATTACK_ELIGIBLE $1564, result in the carry. */
bool check_attack_eligible(void);
/* Resolves an attack between ENTITY_PTR and MOB_PTR.
 * RESOLVE_ATTACK $15AB, a menu exit (ATTACK_TARGET option 1). */
void resolve_attack(void);
/* Chooses an AI mob's action: 1 attack TARGET_PTR, which it sets,
 * 2 enter combat, 4 rest. SELECT_COMBAT_TARGET $18E4, result in A. */
uint8_t select_combat_target(void);
/* Handles walking into an encounter entry. HANDLE_ENCOUNTER $1C01, entry
 * in EVENT_PTR ($BE) from MOVE_LOOP. */
void handle_encounter(struct event *ev);
/* Prints ENTITY_PTR's name and the statue message, and sets its state
 * to 0. SHOW_LOCKED_MSG $1CF1. */
void show_locked_msg(void);
/* Handles walking into an object entry (type $FE). HANDLE_EVENT_SLOT
 * $1D4D, entry in EVENT_PTR from MOVE_LOOP. Returns a chain state. */
uint8_t handle_event_slot(struct event *ev);
/* Handles walking into a mob. HANDLE_MOB_ENCOUNTER $1E5A. */
uint8_t handle_mob_encounter(void);
/* Returns check_adjacent_threats at CURRENT_COL, CURRENT_ROW.
 * CHECK_THREATS_HERE $645F, result in A. */
uint8_t check_threats_here(void);
/* Adds `amount` to ENTITY_PTR's gold, or subtracts it, and recomputes
 * its stats. MODIFY_CURRENT_STATS $70D4, amount in NUM_VALUE ($BC),
 * add or subtract in Y. */
void modify_current_stats(uint16_t amount, bool add);

/* ---- ai.c ---------------------------------------------------------------- */

/* Returns the position of the entry of CHAR_PTR's event list, of a type
 * below $C0, nearest ENTITY_PTR->pos, or the previous call's result
 * when none qualifies. FIND_NEAREST_EVENT $138D, result in A. */
uint8_t find_nearest_event(void);
/* Returns |col(pos) - col(from)| + |row(pos) - row(from)|, and stores
 * pos's column and row in FONT_COL and FONT_ROW. MANHATTAN_DISTANCE
 * $13DB, pos in A, from in ENTITY_PTR->pos. */
uint8_t manhattan_distance(uint8_t pos, uint8_t from);
/* Chooses an AI mob's next step, DIR_NONE..DIR_RIGHT. NPC_MOVE_AI
 * $6742, result in A. */
uint8_t npc_move_ai(void);
/* Chooses an AI mob's target for its turn, or makes it join the party.
 * AI_CHOOSE_TARGET $68AB. */
void ai_choose_target(void);
/* Opens a shop entry's menu, or does nothing for an AI. DISPLAY_SHOP
 * $70DF, entry in EVENT_PTR from MOVE_LOOP. */
uint8_t display_shop(struct event *ev);
/* Shows the shop's next item. BROWSE_HANDLER $7194, a menu exit. */
uint8_t browse_handler(void);
/* Buys the item shown, if the gold covers it. SHOP_BUY_HANDLER $71AC, a
 * menu exit. */
uint8_t shop_buy_handler(void);

/* ---- events.c ------------------------------------------------------------ */

/* Prints whether ENTITY_PTR gets away, and returns true when it does.
 * CHECK_CAN_LEAVE $1231, result 1 or 0 in A. */
bool check_can_leave(void);
/* Adds `value` gold at `pos` to CHAR_PTR's event list. UPDATE_EVENT_LIST
 * $1260, pos in A, value in NUM_VALUE ($BC). */
void update_event_list(uint8_t pos, uint16_t value);
/* Plays a scene entry's scene. TRIGGER_SCENE_EVENT $1D04, entry in
 * EVENT_PTR from MOVE_LOOP. */
void trigger_scene_event(struct event *ev);
/* Opens a chest entry. PICKUP_TREASURE $646B, entry in EVENT_PTR from
 * MOVE_LOOP. Leaves MOB_PTR pointing at the entry. */
void pickup_treasure(struct event *ev);
/* Runs the random events of one pass over the locations. RANDOM_EVENTS
 * $65A9. Leaves CHAR_PTR and MOB_PTR changed. */
void random_events(void);
/* Plays the story's scenes when their conditions are met.
 * CHECK_SCENE_EVENTS $6DFA. */
void check_scene_events(void);
/* Fills S_DUCATS_TO_DROP.number with DROP_AMOUNT. INIT_DROP_DISPLAY $6F52,
 * called by input's MENU_DROP_GOLD_INIT. */
void init_drop_display(void);
/* The drop-gold and "DROP ARMOR" menu exits. FLUSH_PENDING_XP $6EE9,
 * INIT_DROP_GOLD $6F17, ADJ_DROP_INC $6F72, ADJ_DROP_DEC $6F76,
 * DROP_GOLD_EXEC $6FB5. */
uint8_t flush_pending_xp(void);
uint8_t init_drop_gold(void);
uint8_t adj_drop_inc(void);
uint8_t adj_drop_dec(void);
uint8_t drop_gold_exec(void);

/* ---- entities.c ---------------------------------------------------------- */

/* Prints ENTITY_PTR's name in the right-hand panel. LOAD_CHAR_NAME_PTR
 * $0925. */
void load_char_name_ptr(void);
/* Draws ENTITY_PTR's sprite at its position and makes it the blinking
 * cell. SETUP_CHAR_SPRITE $0933. */
void setup_char_sprite(void);
/* Returns what is at `pos` in CHAR_PTR's location, as an entity_ref,
 * and stores pos in LOCATION_ID. FIND_ENTITY_AT_POS $0B62, pos in
 * A, result in ENTITY_REC ($BE/$BF). */
entity_ref find_entity_at_pos(uint8_t pos);
/* Sets FONT_CHARSET to 1 and FONT_CHARNUM to the glyph for `e`, and
 * returns the glyph. GET_ENTITY_FONTCHAR $0C7E, e in ENTITY_REC. */
uint8_t get_entity_fontchar(entity_ref e);
/* Returns the glyph for an appearance in the low byte and its font
 * group (appearance / 21) in the high byte. APPEARANCE_TO_FONTCHAR
 * $0CF7, appearance in A, glyph in A, group in Y. */
uint16_t appearance_to_fontchar(uint8_t appearance);
/* Prints ENTITY_PTR's name on the bottom line. PRINT_MOB_NAME $0D8A. */
void print_mob_name(void);
/* Moves mob `m` from location SOURCE_CHAR's list to ACTIVE_CHAR's.
 * REORDER_CHAR $0F84, m in TARGET_REC ($BC). */
void reorder_char(struct mob *m);
/* Prints `m`'s name on the status line. PRINT_ENTITY_NAME $1142, m in
 * EFFECT_REC ($F6). */
void print_entity_name(const struct mob *m);
/* Redraws the cell at `pos`. DRAW_CHAR_AT_POS $1156, pos in A. */
void draw_char_at_pos(uint8_t pos);
/* Returns the name (S_A_WALL or one of the eight after it) of what `e` is.
 * CLASSIFY_ENTITY_TYPE $116B, e in ENTITY_REC, result in HANDLER_PTR
 * ($BC). */
const uint8_t *classify_entity_type(entity_ref e);
/* Point MOB_PTR at the first member of CHAR_PTR's list, or at the next
 * member after MOB_PTR, or at 0 at the end. With an empty list,
 * first_group_member does next_group_member from the old MOB_PTR,
 * which reads $0002 when MOB_PTR is 0. FIRST_GROUP_MEMBER $12FA (falls
 * into NEXT_GROUP_MEMBER $1300). */
void first_group_member(void);
void next_group_member(void);
/* Prints MOB_PTR's name on the bottom line. PRINT_MEMBER_NAME $1406. */
void print_member_name(void);
/* Moves ENTITY_PTR from location CURRENT_PLAYER to ACTIVE_CHAR.
 * REORDER_CURRENT_CHAR $1414. */
void reorder_current_char(void);
/* Makes MOB_PTR's cell the blinking cell. SET_BLINK_TARGET $16A3. */
void set_blink_target(void);
/* Points CHAR_PTR at location `index`. LOAD_CHAR_TO_FA $6EC9, index in A. */
void load_char_to_fa(uint8_t index);
/* Draw FONT_CHARNUM of FONT_CHARSET at FONT_COL, FONT_ROW
 * (render_font_char), at the text cursor (print_fontchar_at_text_pos),
 * or at the text cursor with `charset_sel` as the font selector for
 * glyphs 24 and up (render_font_char_direct). RENDER_FONT_CHAR $7489,
 * PRINT_FONTCHAR_AT_TEXT_POS $748C, and RENDER_FONT_CHAR_DIRECT $749A
 * (selector in X), which fall into each other. */
void render_font_char(void);
void print_fontchar_at_text_pos(void);
void render_font_char_direct(uint8_t charset_sel);
/* Draw the blinking cell with BLINK_ALT_CHAR or with BLINK_CHAR.
 * DRAW_BLINK_ALT $74CB, DRAW_BLINK_NORMAL $74DD. */
void draw_blink_alt(void);
void draw_blink_normal(void);
/* Sets TEXT_COL and TEXT_ROW to FONT_COL * 2 and FONT_ROW * 2.
 * FONT_POS_TO_TEXT_POS $7723. */
void font_pos_to_text_pos(void);

/* ---- map.c --------------------------------------------------------------- */

/* Shows the current mob's name and sprite at the start of its turn,
 * with TUNE_AI_TURN and a wait for a player's character
 * (CHAR_AI_MODE == 0). The original's comments say an AI's, and its
 * code does it for a player's. SETUP_SCENE_DISPLAY $0901. */
void setup_scene_display(void);
/* Moves the mob at MOB_PTR to a random position inside location
 * ACTIVE_CHAR's bounds, relinks it there, clears its gold, and draws it
 * when ACTIVE_CHAR is CURRENT_PLAYER. SCENE_SETUP $0A17. */
void scene_setup(void);
/* Returns ENTITY_EDGE when `pos` is on the viewport's edge, else
 * ENTITY_NONE. CHECK_AT_BOUNDARY $0BFA, pos in LOCATION_ID, result in
 * ENTITY_REC. */
entity_ref check_at_boundary(uint8_t pos);
/* Run RWTS on RWTS_IOB_PTR with a read, a write, or `command`, and
 * return true on an error. Each stores the command at IOB+$0C and 0 at
 * IOB+3, calls RWTS_ENTRY, and stores 0 to $48. DISK_READ $6C12,
 * DISK_WRITE $6C16, RWTS_DISPATCH $6C18 (command in A), carry out. */
bool disk_read(void);
bool disk_write(void);
bool rwts_dispatch(uint8_t command);
/* Plays scene `scene` from the disk, then redraws the location.
 * SCENE_LOOP $6C37, scene in A. Saves and restores ENTITY_PTR and
 * MOB_PTR in SAVED_ENTITY_PTR and SAVED_MOB_PTR. */
void scene_loop(uint8_t scene);
/* Writes a random byte into the messages. INTEGRITY_FAIL_ADDR $6DED,
 * the tail of LOAD_SCENE_DATA that VERIFY_INTEGRITY jumps to. */
void integrity_fail(void);
/* Draws CHAR_PTR's location if it isn't the one drawn last. Points
 * ENTITY_PTR at CANDIDATE_POS for find_nearest_event and leaves it
 * there. ENTER_LOCATION $72A7. */
void enter_location(void);
/* Fills the 20 x 10 map with MAP_FILL_CHAR's glyph. FILL_MAP $743B. */
void fill_map(void);
/* Prints CHAR_PTR's location name in the left-hand panel, with
 * setup_text_pos. CLEAR_MAP $76B0, whose name doesn't describe it. */
void clear_map(void);
/* The characters LOAD_MAP_TILES sets for the location's style, which
 * GET_ENTITY_FONTCHAR draws for an edge and for nothing. $7AC2 and
 * $7AC3 in the original. */
extern uint8_t DEFAULT_CHAR;
extern uint8_t BLINK_ALT_CHAR;

/* ---- display.c ----------------------------------------------------------- */

/* Draws the status bar from STATUS_BORDER_DATA. DISPLAY_STATUS_BAR $6D1F. */
void display_status_bar(void);
/* Print `name` centered in the 20-column panel on row 20 at column 0
 * (setup_text_pos) or 20 (setup_text_pos_col20). SETUP_TEXT_POS $76D2
 * and SETUP_TEXT_POS_COL20 $76D8, name in PRINT_STRING_ADDR ($BC). */
void setup_text_pos(const uint8_t *name);
void setup_text_pos_col20(const uint8_t *name);
/* Shows ENTITY_PTR's stats screen in STATS_TEMPLATE, waits for a key or
 * the button, and ends with clear_map. DISPLAY_CHAR_STATS $7780. */
void display_char_stats(void);

/* ---- util.c -------------------------------------------------------------- */

/* Raises MOB_PTR->life to its strength when it is lower.
 * CLAMP_CHAR_FIELD $0A93, mob in MOB_PTR. */
void clamp_char_field(void);
/* Finds the first player character in the list of mobs at no location,
 * points MOB_PTR at it, and copies its name into S_ADD_NAME.name. Returns
 * true when it found one (the original's carry clear). FIND_FIRST_PC
 * $0AAE. */
bool find_first_pc(void);
/* Step to the next or previous player character in that list.
 * FIND_NEXT_PC_WRAP $0AFE, FIND_PREV_PC $0B28. */
void find_next_pc_wrap(void);
void find_prev_pc(void);
/* Adds the selected character to ACTIVE_CHAR's location. INIT_CHAR_SELECT
 * $0B4F, a menu exit (CHAR_SELECT option 2). */
uint8_t init_char_select(void);
/* Returns pos % 20 in the low byte and pos / 20 in the high byte.
 * POS_TO_COLROW $0C25, pos in A, column in A, row in Y. */
uint16_t pos_to_colrow(uint8_t pos);
/* Returns mob `index`'s record, the location header's mob_base plus
 * 16 * (index - 1). GET_MOB_DATA $0C32, index in A, result in
 * MOB_DATA_PTR ($BA). */
struct mob *get_mob_data(uint8_t index);
/* Wait with the blink running: four anim_tick_and_wait calls
 * (delay_with_animation), or a timed wait and a frame check
 * (anim_tick_and_wait, which falls into anim_check_frame). The last two
 * return ANIM_FRAME_CTR, which DISPLAY_CHAR_STATS writes to KBDSTRB.
 * DELAY_WITH_ANIMATION $0D10, TICK_ANIM_COUNTER $0D1E,
 * ANIM_TICK_AND_WAIT $0D30, ANIM_CHECK_FRAME $0D36. */
void delay_with_animation(void);
void tick_anim_counter(void);
uint8_t anim_tick_and_wait(void);
uint8_t anim_check_frame(void);
/* Calls WAIT($50) WAIT_LOOP_COUNT times. TIMED_WAIT $0D49. */
void timed_wait(void);
/* Spends one poll's worth of READ_KEYBOARD's idle time.
 * IDLE_ANIM_HANDLER $0D5E. */
void idle_anim_handler(void);
/* Returns col + 20 * row. COLROW_TO_POS $0D98, col in A, row in Y. */
uint8_t colrow_to_pos(uint8_t col, uint8_t row);
/* Returns &LOCATIONS[index] for index 0-128, else &LOCATIONS[0].
 * GET_CHAR_RECORD $1053, index in A, result in CHAR_REC ($BE). */
struct location *get_char_record(uint8_t index);
/* Returns a random number from 0 to `limit` inclusive, stepping the PRNG
 * until a sample fits. The original's comments say 0 to limit - 1.
 * RANDOM_IN_RANGE $1317, limit in A. */
uint8_t random_in_range(uint8_t limit);
/* Waits a combat step: delay_with_animation with sound off, else
 * anim_tick_and_wait. DISPATCH_ON_MODE $1F0C. */
void dispatch_on_mode(void);
/* Steps the PRNG and returns the new PRNG_OUTPUT. STEP_PRNG $78A8. */
uint8_t step_prng(void);
/* Checksums the disk reader, $B300-$B442, and calls integrity_fail on a
 * mismatch. VERIFY_INTEGRITY $A4E6. */
void verify_integrity(void);
/* Does nothing. VERIFY_DISK_IO $A508 checksums 34 bytes of the
 * original's LOAD_SCENE_DATA, which nothing writes, so it always passes. */
void verify_disk_io(void);
/* Fills $0800-$A56E with each page's number and reboots through slot 6.
 * COLD_RESTART $A56F, where SOFTEV points, so it is in section
 * .cold_restart at $A56F. RESET enters it with any state, and it
 * overwrites the C program, so it uses no soft stack and no memory
 * outside zero page. */
[[noreturn, c6502::section(".cold_restart")]] void cold_restart(void);
/* The game speed, 1-5, initially 2. Input's settings change it. $7ABD
 * in the original. */
extern uint8_t ANIM_INDEX;

/* ---- text.c -------------------------------------------------------------- */

/* Writes `value` in decimal into out[0..4] as $B0-$B9, leaving the
 * suppressed leading zeros' bytes as the caller filled them.
 * NUM_TO_DECIMAL $0DA1, value in NUM_VALUE ($BC), out in
 * DECIMAL_OUT_PTR ($BA). */
void num_to_decimal(uint16_t value, uint8_t *out);
/* Render the byte stream at TEXT_STREAM_PTR. text_renderer handles the
 * $7E pause, which calls display_status_bar, and text_renderer_no_pause
 * is the same without it, for display_status_bar. TEXT_RENDERER $6C8E. */
void text_renderer(void);
void text_renderer_no_pause(void);
/* Prints message `index` from the location header's msg_table, a group
 * of strings ended by a length of 0. DISPLAY_MESSAGE $761E, index in A. */
void display_message(uint8_t index);
/* Prints the group of strings at `msg`. DISPLAY_MESSAGE_LOOP $764A, the
 * middle entry of DISPLAY_MESSAGE, msg in PRINT_STRING_ADDR and
 * NUM_VALUE. */
void display_message_loop(const uint8_t *msg);
/* Scrolls the status lines up one. SCROLL_STATUS_LINE $769B. */
void scroll_status_line(void);
/* Put the cursor at column 0 of row 21 (set_cursor_row21) or row 20
 * (set_text_window_bottom), select the standard font, and clear to the
 * end of the window. SET_CURSOR_ROW21 $76BE, SET_TEXT_WINDOW_BOTTOM
 * (also SET_TEXT_WINDOW) $76C2. */
void set_cursor_row21(void);
void set_text_window_bottom(void);
/* Print `s` centered on the bottom line, or centered from TEXT_COL.
 * PRINT_BOTTOM_CENTERED $7705, PRINT_CENTERED_PLUS_COL $7710, s in
 * PRINT_STRING_ADDR. */
void print_bottom_centered(const uint8_t *s);
void print_centered_plus_col(const uint8_t *s);
/* Send HRCG_CTRL_I (set_inverse_video), HRCG_CTRL_N (print_ctrl_n),
 * HRCG_CTRL_O HRCG_CTRL_W (set_text_window_wrap), or HRCG_CTRL_A '0'
 * (print_ctrl_ab). SET_INVERSE_VIDEO $7730, PRINT_CTRL_N $7735,
 * SET_TEXT_WINDOW_WRAP $773A, PRINT_CTRL_AB $774E. */
void set_inverse_video(void);
void print_ctrl_n(void);
void set_text_window_wrap(void);
void print_ctrl_ab(void);
/* Prints the length-prefixed string `s`. A length of 0 or above 128
 * prints nothing. PRINT_STRING $775C and PRINT_STRING_AT_ADDR $7758, s
 * in PRINT_STRING_ADDR. */
void print_string(const uint8_t *s);

/* ---- sound.c ------------------------------------------------------------- */

/* Plays tune `tune` (TUNE_), with `strength` repeats for
 * TUNE_COMBAT_HIT and TUNE_COMBAT_DAMAGE. TUNE_ATTRACT ends at a key, or
 * a button in joystick mode. TUNE_PLAYER $795B, tune * 2 in X, strength
 * in Y. */
void tune_player(uint8_t tune, uint8_t strength);
/* True when the speaker plays, initially true. toggle_sound (main.c)
 * flips it, and dispatch_on_mode (util.c) reads it, in place of the
 * RTS that TOGGLE_SOUND writes over PLAY_HIGH_TONE. */
extern bool SOUND_ON;

/* ---- hrcg.c -------------------------------------------------------------- */

/* Resets the HRCG and shows the high-resolution page 1, as patched by
 * INIT_HRCG. HRCG_INIT $92A8 (HRCG_INIT_BODY $92C5). */
void hrcg_init(void);
/* Draws or obeys character `c`. HRCG_COUT $93BE, reached through CSW
 * from ROM_COUT1, c in A, which preserves A, X, and Y. */
void hrcg_cout(uint8_t c);

#endif
