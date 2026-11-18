/*
    main.c -- the main loop, the start of each mob's turn, the attract
    screen, and the sound and danger settings.

    The game runs as passes over the locations. A pass visits every
    location that holds a player character, draws it, plays the
    story's scenes in locations 1 and 3, and gives each mob there its
    turn, in the order of the location's mob list. A mob whose life is
    below 3 rests, and an AI mob with nothing to do sits its turn out.
    After the last location the random events run, and the next pass
    starts. While no player character is in play, each pass starts
    with the attract screen.

    Departures from the original:

    - START_MOB_TURN jumps to GAME_TURN_LOOP, and the chain of turn and
      menu routines ends with an RTS to PROCESS_MOB_GROUPS. Here
      start_mob_turn calls run_chain(CHAIN_GAME_TURN_LOOP), which
      returns at that RTS.
    - TOGGLE_SOUND tests and patches the first bytes of PLAY_HIGH_TONE
      and PLAY_LOW_TONE, an RTS for sound off and an LDY for sound on.
      toggle_sound flips sound.c's SOUND_ON instead, which sound.c and
      util.c read in place of those bytes. It writes the option's two
      characters as the original does.
    - TOGGLE_SOUND ends with JMP MENU_GAME_SETTINGS, and toggle_sound
      returns CHAIN_MENU_GAME_SETTINGS.

    PROCESS_MOB_GROUPS's comment calls its test before VERIFY_DISK_IO
    a 1-in-24 chance. The test is PRNG_OUTPUT & $17, which is 0 for one
    value in 16, and the C makes the same test.

    Ali Baba, main.asm lines 1232-1287, 1329-1431, 1503-1625,
    1642-1663:

        ORG     $0800
    MAIN_ENTRY:
        SUBROUTINE MAIN_ENTRY
        ; Main program entry point and top-level game loop.
        ;
        ; Inputs:
        ;   TOTAL_MOB_COUNT  — 0 on first run, triggers attract screen
        ;   LOCATION_DATA    — ($4009) base of character record array
        ;
        ; Behavior:
        ;   Calls GAME_INIT, then sets CHAR_PTR to LOCATION_DATA.  If
        ;   TOTAL_MOB_COUNT is zero (first run), calls ATTRACT_SCREEN.
        ;   Main loop: sets CURRENT_PLAYER=1, iterates 9-byte character
        ;   records.  For each active record (MFIELD_XP != 0), calls
        ;   ENTER_LOCATION, dispatches scene events for players 1 and 3,
        ;   and calls PROCESS_MOB_GROUPS.  Terminates the pass when byte 1
        ;   of the next record equals $FF, then calls RANDOM_EVENTS and
        ;   restarts unconditionally.
        ;
        ; Modifies:
        ;   CHAR_PTR, CURRENT_PLAYER, and numerous others via subcalls
        ; Clobbers: A, X, Y
        JSR     GAME_INIT
    .restart:
        STOW    LOCATION_DATA,CHAR_PTR ;  $FA/$FB = LOCATION_DATA ($4009)
        LDA     TOTAL_MOB_COUNT    ; zero on first run (no mobs yet)
        BNE     .has_mobs          ; nonzero = game in progress
        JSR     ATTRACT_SCREEN
    .has_mobs:
        STOB    #$01,CURRENT_PLAYER ; start with player 1
    _MAIN_ENTRY.char_loop:
        LDY     #MFIELD_XP
        LDA     (CHAR_PTR),Y       ; char record byte 8 (status/group)
        BEQ     .next_char         ; zero = inactive, skip
        JSR     ENTER_LOCATION     ; draw map if location changed
        LDA     CURRENT_PLAYER
        BAEQ    #$01,_MAIN_ENTRY.run_scene    ; player 1?
        BANE    #$03,_MAIN_ENTRY.process_mobs ; player 3?
    _MAIN_ENTRY.run_scene:
        JSR     CHECK_SCENE_EVENTS ; scene event dispatch (players 1 & 3)
    _MAIN_ENTRY.process_mobs:
        JSR     PROCESS_MOB_GROUPS ; walk mob list, run turns
    .next_char:
        INC     CURRENT_PLAYER
        CLC
        LDA     #CHAR_RECORD_SIZE  ; advance to next 9-byte record
        ADC     CHAR_PTR
        STA     CHAR_PTR
        BCC     .no_carry
        INC     CHAR_PTR+1
    .no_carry:
        LDY     #$01
        LDA     (CHAR_PTR),Y       ; byte 1 of next record
        BANE    #$FF,_MAIN_ENTRY.char_loop    ; more characters -> loop
        JSR     RANDOM_EVENTS      ; between-round random event dispatch
        JMP     .restart           ; restart the game loop

    ...

        ORG     $0884
    PROCESS_MOB_GROUPS:
        SUBROUTINE PROCESS_MOB_GROUPS
        ; Walk the mob linked list for the current character and process each mob.
        ;
        ; Inputs:
        ;   CHAR_PTR  — pointer to current character record
        ;
        ; Behavior:
        ;   Reads CFIELD_NEXT from (CHAR_PTR) as the initial link and saves it
        ;   in CURRENT_ROOM.  Rolls a 1-in-24 PRNG chance to call the disk
        ;   checkpoint at [[VERIFY_DISK_IO]].  Then iterates the linked list: for each mob,
        ;   resolves via GET_MOB_DATA, copies to ENTITY_PTR, reads the next link.
        ;   HP < 3 (inactive): SHOW_IDLE_STATUS + AUTO_HEAL.
        ;   HP >= 3 (active): START_MOB_TURN, then reset BLINK_COL to MAP_WIDTH.
        ;   Loop continues until the link value is 0.
        ;
        ; Modifies:
        ;   CURRENT_ROOM, ENTITY_PTR, MOB_DATA_PTR, BLINK_COL
        ; Clobbers: A, X, Y
        ; Walk the mob linked list for the current character.
        ; For each group: inactive (type < 3) -> display message + auto-heal;
        ; active (type >= 3) -> set up scene and enter GAME_TURN_LOOP.
        LDY     #CFIELD_NEXT
        LDA     (CHAR_PTR),Y     ; char record byte 2 = first group link
        STA     CURRENT_ROOM     ; save as current link
        JSR     CLEAR_MAP
        JSR     STEP_PRNG
        LDA     PRNG_OUTPUT
        AND     #$17             ; 1-in-24 chance (bits 0-2,4)
        BNE     .walk_list
        JSR VERIFY_DISK_IO            ; periodic disk checkpoint
    .walk_list:
        LDA     CURRENT_ROOM     ; current link in chain
        BNE     .has_mob
        RTS                      ; zero = end of list
    .has_mob:
        JSR     GET_MOB_DATA     ; resolve link -> MOB_DATA_PTR/MOB_DATA_PTR+1
        MOVW    MOB_DATA_PTR,ENTITY_PTR ;  ENTITY_PTR/ENTITY_PTR+1 = mob data pointer
        LDY     #MFIELD_NEXT
        LDA     (ENTITY_PTR),Y   ; mob data byte 2 = next link
        STA     CURRENT_ROOM     ; save for next iteration
        LDY     #MFIELD_HP
        LDA     (ENTITY_PTR),Y   ; mob data byte 6
        AND     #HP_MASK   ; bits 5-0 = HP
        BAPL    #$03,_PROCESS_MOB_GROUPS.active_mob ; type >= 3?
        JSR     SHOW_IDLE_STATUS ; inactive: print name + message
        JSR     AUTO_HEAL        ; heal party members
        JMP     .walk_list
    _PROCESS_MOB_GROUPS.active_mob:
        JSR     START_MOB_TURN   ; active: set up sprite, enter turn loop
        STOB    #$14,BLINK_COL ; reset blink column to 20
        JMP     .walk_list
        ORG     $08D1
    SHOW_IDLE_STATUS:
        SUBROUTINE SHOW_IDLE_STATUS
        ; Display a brief status message for an inactive mob group.
        ;
        ; Inputs:
        ;   ENTITY_PTR  — pointer to current mob record (bytes 0-1 = name)
        ;
        ; Behavior:
        ;   Scrolls the status area, prints the mob's name via PRINT_MOB_NAME,
        ;   scrolls again, then displays MSG_REST_STRONGER ("resting, growing
        ;   stronger") via DISPLAY_MESSAGE.
        ; Clobbers: A, X, Y
        JSR     SCROLL_STATUS_LINE
        JSR     PRINT_MOB_NAME
        JSR     SCROLL_STATUS_LINE
        LDA     #MSG_REST_STRONGER                    ; message $20: idle status text
        JMP     DISPLAY_MESSAGE
        ORG     $08DF
    START_MOB_TURN:
        SUBROUTINE START_MOB_TURN
        ; Set up the sprite and enter the turn loop for an active mob.
        ;
        ; Inputs:
        ;   ENTITY_PTR  — pointer to current mob record
        ;
        ; Behavior:
        ;   Reads MFIELD_APPEARANCE (byte 4) from the mob record and converts
        ;   it to a font character via APPEARANCE_TO_FONTCHAR.  The font group
        ;   (in Y) doubles as the AI behavior mode, stored in CHAR_AI_MODE.
        ;   Calls CHECK_MOB_NEEDS_TURN; if carry clear, returns immediately.
        ;   Otherwise positions the cursor, sets up the scene display, and
        ;   jumps to GAME_TURN_LOOP.
        ;
        ; Modifies:
        ;   CHAR_SPRITE, CHAR_AI_MODE
        ; Clobbers: A, X, Y
        LDY     #MFIELD_APPEARANCE
        LDA     (ENTITY_PTR),Y       ; mob data byte 4 = appearance
        JSR     APPEARANCE_TO_FONTCHAR
        STA     CHAR_SPRITE          ; save font character
        STY     CHAR_AI_MODE         ; Y = font group -> AI behavior mode
        JSR     CHECK_MOB_NEEDS_TURN ; carry set = needs a turn
        BCS     .enter_turn
        RTS                          ; carry clear = skip this mob
    .enter_turn:
        JSR     SET_CURSOR_ROW21
        JSR     SETUP_SCENE_DISPLAY
        MOVB    CHAR_AI_MODE,$5AA6 ; save AI mode (unused in main.bin)
        JMP     GAME_TURN_LOOP

    ...

        ORG     $0958
    CHECK_MOB_NEEDS_TURN:
        SUBROUTINE CHECK_MOB_NEEDS_TURN
        ; Determine whether a mob group needs to take a game turn.
        ;
        ; Inputs:
        ;   CHAR_AI_MODE  — 0 = player (always gets turn), nonzero = AI
        ;   ENTITY_PTR    — pointer to current mob record
        ;
        ; Behavior:
        ;   Player-controlled characters always get a turn.  For AI mobs,
        ;   checks three conditions: (1) MFIELD_LEVEL_ENC low nibble nonzero
        ;   (pending action), (2) encounter at current MFIELD_POSITION, or
        ;   (3) adjacent threats.  If none apply, clears the restricted flag
        ;   and skips the turn.
        ;
        ; Outputs:
        ;   C flag  — set = needs turn, clear = skip
        ; Clobbers: A, Y
        LDA     CHAR_AI_MODE
        BEQ     .needs_turn      ; player-controlled always gets a turn
        LDY     #MFIELD_LEVEL_ENC
        LDA     (ENTITY_PTR),Y   ; mob data byte 12
        AND     #LOW_NIBBLE_MASK ; low nibble = pending action
        BNE     .needs_turn      ; nonzero = pending action -> turn
        LDY     #MFIELD_POSITION
        LDA     (ENTITY_PTR),Y   ; mob data byte 3 = position
        JSR     CHECK_ENCOUNTER  ; check for encounter at this position
        LDA     ENCOUNTER_RESULT
        BNE     .needs_turn      ; encounter found -> turn
        LDY     #MFIELD_POSITION
        LDA     (ENTITY_PTR),Y   ; position again
        JSR     CHECK_ADJACENT_THREATS
        LDA     ADJACENT_THREAT
        BNE     .needs_turn      ; threats nearby -> turn
        JSR     CLEAR_RESTRICTED ; no action needed -> clear restricted flag
        CLC                      ; carry clear = skip
        RTS
    .needs_turn:
        SEC                      ; carry set = needs turn
        RTS
        ORG     $0984
    TOGGLE_SOUND:
        SUBROUTINE TOGGLE_SOUND
        ; Sound toggle, settings display, threshold adjustment, and retirement.
        ;
        ; Entry points:
        ;   TOGGLE_SOUND          — toggles sound on/off by patching tone routines
        ;   SETTINGS_DISPLAY_INIT — formats danger level display, enters settings menu
        ;   DEC_THRESHOLD_WRAP    — decrements event threshold, returns to caller
        ;   INC_THRESHOLD_WRAP    — increments event threshold, returns to caller
        ;   DEC_EVENT_THRESHOLD   — decrements EVENT_THRESHOLD (clamps to 0)
        ;   INC_EVENT_THRESHOLD   — increments EVENT_THRESHOLD (clamps to MAX_THRESHOLD)
        ;
        ; Modifies: PLAY_HIGH_TONE, PLAY_LOW_TONE (patched), EVENT_THRESHOLD,
        ;           S_SOUND_CHAR1, S_SOUND_CHAR2, S_DANGER_OUT+2..+4
        ; Clobbers: A

        ; --- Toggle sound on/off ---
        LDA     #RTS_OPCODE                ; RTS opcode
        CMP     PLAY_HIGH_TONE             ; is sound disabled?
        BNE     .turn_off
        ; --- Enable sound ---
        LDA     #$CE                       ; 'N' (high-bit ASCII)
        STA     S_SOUND_CHAR1              ; patch "OFF" -> "ON "
        LDA     #LDY_OPCODE                ; ' ' (space) / LDY opcode
        STA     S_SOUND_CHAR2              ; trailing space
        LDA     #LDY_OPCODE                ; restore original entry byte
    .patch_tones:
        STA     PLAY_HIGH_TONE             ; \ patch tone routine entry bytes
        STA     PLAY_LOW_TONE              ; /
        JMP     MENU_GAME_SETTINGS         ; re-enter settings menu
    .turn_off:
        ; --- Disable sound ---
        LDA     #$C6                       ; 'F' (high-bit ASCII)
        STA     S_SOUND_CHAR1              ; patch "ON " -> "OFF"
        STA     S_SOUND_CHAR2              ;
        LDA     #RTS_OPCODE                ; RTS opcode
        BNE     .patch_tones               ; always taken

        ; --- Initialize settings display ---
    SETTINGS_DISPLAY_INIT:
        LDA     #$20                       ; \ clear danger digit area
        STA     S_DANGER_OUT+2             ;  |
        STA     S_DANGER_OUT+3             ;  |
        STA     S_DANGER_OUT+4             ; /
        LDA     #$00                       ; \ NUM_VALUE = EVENT_THRESHOLD
        STA     NUM_VALUE+1                ;  | (high byte = 0)
        LDA     EVENT_THRESHOLD            ;  |
        STA     NUM_VALUE                  ; /
        LDA     #<S_DANGER_OUT             ; \ DECIMAL_OUT_PTR -> S_DANGER_OUT
        STA     DECIMAL_OUT_PTR            ;  |
        LDA     #>S_DANGER_OUT             ;  |
        STA     DECIMAL_OUT_PTR+1          ; /
        JMP     NUM_TO_DECIMAL             ; format and return

        ; --- Decrement event threshold (wrapper for menu) ---
    DEC_THRESHOLD_WRAP:
        JSR     DEC_EVENT_THRESHOLD
        RTS

        ; --- Increment event threshold (wrapper for menu) ---
    INC_THRESHOLD_WRAP:
        JSR     INC_EVENT_THRESHOLD
        RTS

        ; --- Decrement EVENT_THRESHOLD (clamp to 0) ---
    DEC_EVENT_THRESHOLD:
        DEC     EVENT_THRESHOLD
        BPL     .dec_done                  ; >= 0 -> ok
        INC     EVENT_THRESHOLD            ; clamp: -1 -> 0
    .dec_done:
        RTS

        ; --- Increment EVENT_THRESHOLD (clamp to MAX_THRESHOLD) ---
    INC_EVENT_THRESHOLD:
        LDA     EVENT_THRESHOLD
        CMP     #MAX_THRESHOLD             ; 99 decimal
        BCS     .inc_done                  ; already at max
        INC     EVENT_THRESHOLD
    .inc_done:
        RTS


    ...

        ORG     $09F4
    ATTRACT_SCREEN:
        SUBROUTINE ATTRACT_SCREEN
        ; Initialize attract-mode demo screen.
        ;
        ; Behavior:
        ;   Sets ACTIVE_CHAR to 1, initializes mob data pointer for scene 1,
        ;   runs scene setup, clears location flag, and enters SCENE_LOOP
        ;   with scene $26 (attract demo).
        ;
        ; Modifies: ACTIVE_CHAR, SCENE_EVENT_RESULT, MOB_PTR, LOCATION_POS
        ; Clobbers: A, X, Y

        STOB    #$01,ACTIVE_CHAR           ; scene/character index = 1
        STA     SCENE_EVENT_RESULT         ; mark event as handled
        JSR     GET_MOB_DATA               ; scene 1 -> MOB_DATA_PTR
        MOVW    MOB_DATA_PTR,MOB_PTR       ; save pointer
        JSR     SCENE_SETUP
        STOB    #$00,LOCATION_POS          ; clear location-change flag
        JSR     SET_CURSOR_ROW21
        LDA     #$26                       ; scene $26 (attract demo)
        JMP     SCENE_LOOP                 ; tail-call
*/
#include "ali_baba.h"

/* A pass over the locations ends at the record whose name pointer's
 * high byte is $FF, which is LOCATIONS_END's at $4237. The test reads
 * that one byte, as the original does. */
#define LOCATION_LIST_END 0xFF
#define NAME_HIGH_BYTE(loc) (((const uint8_t *)(loc))[1])

/* MAX_THRESHOLD in the original, the highest danger setting (99). */
#define MAX_THRESHOLD 0x63

/* The scene SCENE_LOOP plays on the attract screen. */
#define SCENE_ATTRACT 0x26

static void attract_screen(void);
static void process_mob_groups(void);

/* Runs GAME_INIT and then passes over the locations for ever. Each
 * pass runs the turns of the mobs in every location that holds a
 * player character, and then the random events. CHAR_PTR walks the
 * records, and the routines called on the way may move it, so the
 * walk reads and steps the global and not a copy, as the original
 * does. The attract screen plays at the start of every pass that
 * finds no player character in play. */
int main(void) {
    game_init();
    for (;;) {
        CHAR_PTR = &LOCATIONS[1];
        if (TOTAL_MOB_COUNT == 0)
            attract_screen();
        CURRENT_PLAYER = 1;
        do {
            if (CHAR_PTR->pc_count != 0) {
                enter_location();
                if (CURRENT_PLAYER == 1 || CURRENT_PLAYER == 3)
                    check_scene_events();
                process_mob_groups();
            }
            CURRENT_PLAYER++;
            CHAR_PTR++;
        } while (NAME_HIGH_BYTE(CHAR_PTR) != LOCATION_LIST_END);
        random_events();
    }
}

/* Prints ENTITY_PTR's name and "resting, growing stronger" for a mob
 * that is out of action. */
static void show_idle_status(void) {
    scroll_status_line();
    print_mob_name();
    scroll_status_line();
    display_message(MSG_REST_STRONGER);
}

/* Returns true when ENTITY_PTR has something to do this turn. The
 * player's characters always do. An AI mob does when it has move
 * points, when it is on an encounter, or when hostile mobs are next to
 * it, and otherwise loses its restricted flag and sits the turn out. */
static bool check_mob_needs_turn(void) {
    if (CHAR_AI_MODE == 0)
        return true;
    if ((ENTITY_PTR->speed & LOW_NIBBLE_MASK) != 0)
        return true;
    if (check_encounter(ENTITY_PTR->pos) != 0)
        return true;
    check_adjacent_threats(ENTITY_PTR->pos);
    if (ADJACENT_THREAT != 0)
        return true;
    clear_restricted();
    return false;
}

/* Runs ENTITY_PTR's turn, if it has one. The original jumps to
 * GAME_TURN_LOOP, and the chain's last RTS returns to
 * PROCESS_MOB_GROUPS. */
static void start_mob_turn(void) {
    uint16_t glyph_group = appearance_to_fontchar(ENTITY_PTR->appearance);
    CHAR_SPRITE = (uint8_t)glyph_group;
    CHAR_AI_MODE = (uint8_t)(glyph_group >> 8);
    if (!check_mob_needs_turn())
        return;
    set_cursor_row21();
    setup_scene_display();
    SAVED_AI_MODE = CHAR_AI_MODE;
    run_chain(CHAIN_GAME_TURN_LOOP);
}

/* Runs a turn for each mob in CHAR_PTR's location, in list order. A
 * mob whose life is below 3 rests instead. CURRENT_ROOM holds the next
 * mob's index, read before the turn, since the turn can move the mob
 * to another list. When the PRNG's output has bits 0-2 and 4 clear,
 * which is 1 time in 16, the original also checksums the scene loader
 * (VERIFY_DISK_IO). verify_disk_io does nothing in C, and the test and
 * the call are kept. */
static void process_mob_groups(void) {
    CURRENT_ROOM = CHAR_PTR->mob_head;
    clear_map();
    if ((step_prng() & 0x17) == 0)
        verify_disk_io();
    while (CURRENT_ROOM != 0) {
        ENTITY_PTR = get_mob_data(CURRENT_ROOM);
        CURRENT_ROOM = ENTITY_PTR->next;
        if ((uint8_t)(ENTITY_PTR->life & HP_MASK) < 3) {
            show_idle_status();
            auto_heal();
        } else {
            start_mob_turn();
            BLINK_COL = MAP_WIDTH;
        }
    }
}

uint8_t toggle_sound(void) {
    if (!SOUND_ON) {
        S_SOUND_CHAR1 = 0xCE;   /* 'N' */
        S_SOUND_CHAR2 = 0xA0;   /* ' ' */
        SOUND_ON = true;
    } else {
        S_SOUND_CHAR1 = 0xC6;   /* 'F' */
        S_SOUND_CHAR2 = 0xC6;   /* 'F' */
        SOUND_ON = false;
    }
    return CHAIN_MENU_GAME_SETTINGS;
}

void settings_display_init(void) {
    S_DANGER_OUT[2] = 0x20;
    S_DANGER_OUT[3] = 0x20;
    S_DANGER_OUT[4] = 0x20;
    num_to_decimal(EVENT_THRESHOLD, S_DANGER_OUT);
}

/* Steps EVENT_THRESHOLD down, holding it at 0. The original decrements
 * and then increments again when the result is negative, so it
 * writes $FF and then 0 at the bottom, and so does this. */
static void dec_event_threshold(void) {
    EVENT_THRESHOLD--;
    if (EVENT_THRESHOLD & 0x80)
        EVENT_THRESHOLD++;
}

/* Steps EVENT_THRESHOLD up, holding it at MAX_THRESHOLD. */
static void inc_event_threshold(void) {
    if (EVENT_THRESHOLD < MAX_THRESHOLD)
        EVENT_THRESHOLD++;
}

void dec_threshold_wrap(void) {
    dec_event_threshold();
}

void inc_threshold_wrap(void) {
    inc_event_threshold();
}

/* Plays the attract scene. Mob 1 is placed at a random position in
 * location 1 first, and LOCATION_POS is cleared so that the next
 * enter_location redraws. */
static void attract_screen(void) {
    ACTIVE_CHAR = 1;
    SCENE_EVENT_RESULT = 1;
    MOB_PTR = get_mob_data(1);
    scene_setup();
    LOCATION_POS = 0;
    set_cursor_row21();
    scene_loop(SCENE_ATTRACT);
}
