/*
    turn.c -- a mob's turn: its moves across the map, what happens when
    it walks into something, and the end of the turn, and run_chain,
    which runs the routines of the turn and of the menus one after
    another.

    GAME_TURN_LOOP starts the turn of the mob at ENTITY_PTR. It notes
    the starting cell, marks the mob engaged ("restricted") when it is
    too weak or shares its cell with a hostile mob, and gives it its
    move points, which are cut at random when a hostile mob shares its
    cell or the hostile mobs around it outmatch it. MOVE_LOOP then takes one
    step at a time, from the keyboard or the joystick for the player
    and from NPC_MOVE_AI for an AI mob, until the points run out or
    the mob walks into something. Walking into a mob, a chest, a scene,
    a shop, or an object hands the turn to that thing's routine. An AI
    mob's NPC_MOVE_AI may also return a command to rest, fight, or
    attack in place of a direction. END_TURN gives the turn to the
    action menu or to the AI's choice of action in TURN_DISPATCH, or
    leaves the mob fighting when it has moved more steps than its speed
    allows.

    The original's turn and menus are routines that jump to each other
    at one stack level, until one returns to the caller of
    START_MOB_TURN or INIT_WORLD. Here each of those routines returns
    the CHAIN_ state of the routine that runs next, and run_chain calls
    them in a loop. ali_baba.h and UNITS.md describe the translation.

    Departures from the original:

    - GAME_TURN_LOOP falls into MOVE_LOOP, and game_turn_loop returns
      CHAIN_MOVE_LOOP. Each JMP of MOVE_LOOP, END_TURN, and
      TURN_DISPATCH to a routine of the chain returns its state, and
      each JMP to an ordinary routine is a call followed by
      CHAIN_RETURN, as UNITS.md sets out.
    - MOVE_LOOP leaves the entry it walked into in EVENT_PTR ($BE/$BF)
      for the routine it jumps to, and keeps it across COMMIT_MOVE with
      PSHW/PULW before DISPLAY_SHOP. move_loop passes the entry to the
      routine as its parameter.
    - MOVE_LOOP reads the MOVE option's enable byte, GAME_INPUT_TABLE+4,
      after it calls CONFIGURE_INPUT. move_loop takes the byte from
      configure_input's return value, because the menus are input.c's.
    - An AI's command is kept on the stack across CHECK_AND_RESTRICT by
      PHA and PLA. move_loop keeps it in a local.
    - SET_CHAR_STATE keeps its argument in $5A60 while it reads the
      flags byte. set_char_state keeps it in its parameter.
    - TUNE_PLAYER takes the tune number times 2 in X, and a repeat count
      in Y that matters only for TUNE_COMBAT_HIT and TUNE_COMBAT_DAMAGE.
      tune_player takes the tune number, and turn passes 0 for the
      count, where the original's Y is whatever the routines before
      left there.

    The original's comments disagree with its code about the meaning of
    CHECK_THREATS_HERE's result, the result of SELECT_COMBAT_TARGET that
    TURN_DISPATCH's BMI catches, and the direction of END_TURN's
    comparison. The comments at game_turn_loop, turn_dispatch, and
    end_turn describe what the code does.

    Ali Baba, main.asm lines 3676-3697 and 4566-4973:

        ORG     $14A3
    TURN_DISPATCH:
        SUBROUTINE TURN_DISPATCH
        ; Post-movement decision point.
        ;
        ; Inputs:
        ;   CHAR_AI_MODE  — 0 = player, nonzero = AI
        ;
        ; Behavior:
        ;   If player-controlled, jumps to TEARDOWN_INPUT to present the
        ;   action menu.  For AI, calls SELECT_COMBAT_TARGET: result 2
        ;   enters combat state, negative commits an attack via START_COMBAT,
        ;   otherwise attempts AUTO_HEAL.
        ; Clobbers: A
        LDA     CHAR_AI_MODE
        BEQ     .no_combat
        JSR     SELECT_COMBAT_TARGET
        BAEQ    #$02,ENTER_COMBAT_STATE
        BMI     START_COMBAT
        JMP     AUTO_HEAL
    .no_combat:
        JMP     TEARDOWN_INPUT
        ORG     $19AD
    GAME_TURN_LOOP:
        SUBROUTINE GAME_TURN_LOOP
        ; Execute one character's complete movement turn.
        ;
        ; Inputs:
        ;   ENTITY_PTR    — pointer to current entity record
        ;   CHAR_AI_MODE  — 0 = player (keyboard input), nonzero = AI
        ;
        ; Behavior:
        ;   Saves starting position, clears flags.  Checks HP (field 6 low
        ;   6 bits < 3 = restricted) and encounter type (2 = restricted).
        ;   For AI, calls AI_CHOOSE_TARGET.  Computes movement points from
        ;   field 12 low nibble, optionally reduced by threat count.
        ;   Main loop: reads direction (player keyboard via $A44C, or
        ;   NPC_MOVE_AI), applies to col/row, checks destination cell.
        ;   Empty = commit move + re-check threats.  Occupied = dispatch
        ;   by entity type: mob encounter, treasure, scene event, shop,
        ;   or empty-slot handler.  Exits when points exhausted or combat.
        ;
        ; Modifies:
        ;   TURN_ACTIVE, CURRENT_COL, CURRENT_ROW, TURN_START_COL,
        ;   TURN_START_ROW, STEPS_TAKEN, MOVE_POINTS, INPUT_DIR,
        ;   ENCOUNTER_RESULT
        ; Clobbers: A, X, Y
        ; --- setup: save position, clear flags ---
        STOB    #$01,TURN_ACTIVE
        LDY     #MFIELD_POSITION
        LDA     (ENTITY_PTR),Y           ; char field 3 = packed position
        JSR     POS_TO_COLROW
        STA     CURRENT_COL
        STA     TURN_START_COL
        STY     CURRENT_ROW
        STY     TURN_START_ROW
        STOB    #$00,STEPS_TAKEN
        JSR     CLEAR_RESTRICTED         ; clear restricted flag (bit 2 of field 15)

        ; --- check constitution and encounters ---
        LDY     #MFIELD_HP
        LDA     (ENTITY_PTR),Y           ; char field 6 (constitution/HP)
        AND     #HP_MASK
        BAPL    #$03,_GAME_TURN_LOOP.con_ok
        JSR     SET_RESTRICTED           ; set restricted (too weak)
    _GAME_TURN_LOOP.con_ok:
        LDY     #MFIELD_POSITION
        LDA     (ENTITY_PTR),Y
        JSR     CHECK_ENCOUNTER
        LDA     ENCOUNTER_RESULT
        BANE    #$02,_GAME_TURN_LOOP.enc_ok
        JSR     SET_RESTRICTED           ; set restricted (hostile territory)
    _GAME_TURN_LOOP.enc_ok:
        LDA     CHAR_AI_MODE
        BEQ     .get_speed
        JSR     AI_CHOOSE_TARGET         ; NPC: pick a destination

        ; --- determine movement points ---
    .get_speed:
        LDY     #MFIELD_LEVEL_ENC
        LDA     (ENTITY_PTR),Y           ; char field 12
        AND     #LOW_NIBBLE_MASK         ; low nibble = speed
        STA     MOVE_POINTS

        ; --- adjust points based on threats ---
        LDA     ENCOUNTER_RESULT
        BAEQ    #$02,_GAME_TURN_LOOP.chk_limit          ; hostile -> check if enough to limit
        JSR     CHECK_THREATS_HERE       ; check adjacent threats
        BAEQ    #$01,_GAME_TURN_LOOP.move_loop          ; exactly 1 threat -> keep full
        LDA     MOVE_POINTS
        BALT    #$02,_GAME_TURN_LOOP.move_loop ; < 2 points -> keep as is
        JSR     RANDOM_IN_RANGE          ; random(move_points)
        BAEQ    #$00,_GAME_TURN_LOOP.do_limit           ; random == 0 -> force limit
        STA     MOVE_POINTS              ; reduce randomly
        BNE     _GAME_TURN_LOOP.move_loop
    _GAME_TURN_LOOP.chk_limit:
        LDA     MOVE_POINTS
        BALT    #$02,_GAME_TURN_LOOP.move_loop ; < 2 -> use as is
    _GAME_TURN_LOOP.do_limit:
        LDA     #$01
        JSR     RANDOM_IN_RANGE          ; random(1) -> 0 or 1
        CLC
        ADC     #$01                     ; 1 or 2
        STA     MOVE_POINTS
        BNE     _GAME_TURN_LOOP.move_loop               ; always

        ; === MOVEMENT LOOP ===
    MOVE_LOOP:                     ; $1A31 - external entry point
    _GAME_TURN_LOOP.move_loop:
        LDA     #$00
        BANE    MOVE_POINTS,_GAME_TURN_LOOP.has_points
        BANE    STEPS_TAKEN,_GAME_TURN_LOOP.end_turn    ; out of points, took steps -> done
        BAEQ    CHAR_AI_MODE,_GAME_TURN_LOOP.has_points ; player with 0 points, 0 steps -> still prompt
    _GAME_TURN_LOOP.end_turn:
        JMP     END_TURN                 ; end turn

    _GAME_TURN_LOOP.has_points:
        BAEQ    CHAR_AI_MODE,_GAME_TURN_LOOP.player_input
        JSR     NPC_MOVE_AI              ; NPC movement AI
        JMP     .dispatch_cmd

        ; --- player input ---
    _GAME_TURN_LOOP.player_input:
        JSR     CONFIGURE_INPUT          ; set up input UI
        LDA     GAME_INPUT_TABLE+4       ; check if movement enabled
        BNE     .have_input
        JMP     TEARDOWN_INPUT           ; no input -> exit
    .have_input:
        JSR     GET_INPUT_TABLE_PTR
        JSR     SETUP_INPUT
        JSR READ_KEYBOARD                    ; read keyboard
        STA     INPUT_DIR
        BANE    #$00,_GAME_TURN_LOOP.do_move            ; player goes straight to movement
        JMP     TEARDOWN_INPUT           ; cancelled

        ; --- NPC command dispatch (player skips this) ---
    .dispatch_cmd:
        STA     INPUT_DIR
        BALT    #$05,_GAME_TURN_LOOP.do_move ; 1-4: movement direction
        BNE     .not_pass
        JMP     _GAME_TURN_LOOP.move_loop               ; 5: pass
    .not_pass:
        BANE    #$09,_GAME_TURN_LOOP.not_fight
        JSR     SET_RESTRICTED           ; 9: fight -> set restricted
        LDA     #$00
        JSR     SET_CHAR_STATE
        JMP     AUTO_HEAL
    _GAME_TURN_LOOP.not_fight:
        PHA
        JSR     CHECK_AND_RESTRICT       ; re-check encounter
        PLA
        BANE    #$07,_GAME_TURN_LOOP.not_cmd7
        JMP     ENTER_COMBAT_STATE       ; 7: action
    _GAME_TURN_LOOP.not_cmd7:
        BCS     .cmd8
        JMP     START_COMBAT             ; 6: action
    .cmd8:
        JMP     SET_STATE_FIGHT          ; 8: set state 1

        ; --- apply movement direction ---
    _GAME_TURN_LOOP.do_move:
        LDA     ENCOUNTER_RESULT
        BAMI    #$02,_GAME_TURN_LOOP.move_ok
        JSR     CHECK_CAN_LEAVE          ; can we leave hostile zone?
        BAEQ    #$01,_GAME_TURN_LOOP.move_ok
        JMP     END_TURN                 ; blocked -> end turn
    _GAME_TURN_LOOP.move_ok:
        STOB    #$00,ENCOUNTER_RESULT
        LDA     INPUT_DIR
        DEC     MOVE_POINTS
        INC     STEPS_TAKEN
        BAPL    #$02,_GAME_TURN_LOOP.not_north
        DEC     CURRENT_ROW              ; 1: north
        JMP     .check_dest
    _GAME_TURN_LOOP.not_north:
        BNE     .not_south
        INC     CURRENT_ROW              ; 2: south
        JMP     .check_dest
    .not_south:
        BANE    #$03,_GAME_TURN_LOOP.go_east
        DEC     CURRENT_COL              ; 3: west
        JMP     .check_dest
    _GAME_TURN_LOOP.go_east:
        INC     CURRENT_COL              ; 4: east

        ; --- check what's at the destination ---
    .check_dest:
        LDA     CURRENT_COL
        LDY     CURRENT_ROW
        JSR     COLROW_TO_POS
        JSR     FIND_ENTITY_AT_POS       ; -> ENTITY_REC/ENTITY_REC+1
        LDA     #$00
        BANE    ENTITY_REC+1,_GAME_TURN_LOOP.something  ; ENTITY_REC+1 != 0 -> occupied
        BAEQ    ENTITY_REC,_GAME_TURN_LOOP.empty        ; ENTITY_REC == 0 -> empty cell
        LDX     #$0E                     ; tune 7: entity interaction
        JSR     TUNE_PLAYER
        JMP     SHOW_LOCKED_MSG

    _GAME_TURN_LOOP.empty:
        JSR     COMMIT_MOVE              ; commit move (update pos, draw sprite)
        JSR     CHECK_THREATS_HERE       ; check adjacent threats
        BANE    #$00,_GAME_TURN_LOOP.keep_moving        ; threats remain -> continue
        STA     MOVE_POINTS              ; no threats -> stop
        STA     TURN_ACTIVE
    _GAME_TURN_LOOP.keep_moving:
        JMP     _GAME_TURN_LOOP.move_loop

        ; --- destination occupied: dispatch by type ---
    _GAME_TURN_LOOP.something:
        LDA     ENTITY_REC+1
        BAMI    #MOB_PTR_FLAG,_GAME_TURN_LOOP.event     ; ENTITY_REC+1 < $80 -> event entry
        JMP     HANDLE_MOB_ENCOUNTER     ; ENTITY_REC+1 >= $80 -> mob encounter

    _GAME_TURN_LOOP.event:
        LDY     #EVFIELD_TYPE
        LDA     (ENTITY_REC),Y           ; event byte 1
        TAX
        AND     #EVENT_ACTIVATED
        BAEQ    #EVENT_ACTIVATED,_GAME_TURN_LOOP.activated
        JMP     HANDLE_ENCOUNTER         ; non-activated -> encounter

    _GAME_TURN_LOOP.activated:
        TXA
        BAPL    #$D2,_GAME_TURN_LOOP.not_treasure
        JMP     PICKUP_TREASURE          ; $C0-$D1: treasure

    _GAME_TURN_LOOP.not_treasure:
        BAPL    #$D5,_GAME_TURN_LOOP.not_scene
        JMP     TRIGGER_SCENE_EVENT      ; $D2-$D4: scene event

    _GAME_TURN_LOOP.not_scene:
        BAPL    #$DB,_GAME_TURN_LOOP.not_map_event
        PSHW    ENTITY_REC               ; $D5-$DA: save ENTITY_REC/ENTITY_REC+1
        JSR     COMMIT_MOVE              ; commit move first
        PULW    ENTITY_REC               ; restore ENTITY_REC/ENTITY_REC+1
        JMP     DISPLAY_SHOP

    _GAME_TURN_LOOP.not_map_event:
        BAEQ    #EVENT_EMPTY,_GAME_TURN_LOOP.empty_slot
        JMP     TEARDOWN_INPUT           ; $DB-$FD: unknown

    _GAME_TURN_LOOP.empty_slot:
        JMP     HANDLE_EVENT_SLOT        ; EVENT_EMPTY ($FE): empty slot interaction
        ORG     $1B52
    COMMIT_MOVE:
        SUBROUTINE COMMIT_MOVE
        ; Finalize character movement on the map.
        ;
        ; Inputs:
        ;   CURRENT_COL, CURRENT_ROW  — new position
        ;   ENTITY_PTR                — pointer to entity record
        ;   CHAR_SPRITE               — font glyph for this character
        ;   TURN_START_COL/ROW        — position at start of turn
        ;
        ; Behavior:
        ;   Copies CURRENT_COL/ROW into font and blink cursor vars, packs
        ;   them via COLROW_TO_POS, stores in entity MFIELD_POSITION.
        ;   Draws the sprite via RENDER_FONT_CHAR.  Sets TURN_TUNE_GUARD
        ;   to $16 to arm the one-shot movement sound.  Falls through to
        ;   RESET_TURN_POS, which redraws the old cell and optionally
        ;   plays the movement tune.
        ;
        ; Modifies:
        ;   FONT_COL, FONT_ROW, FONT_CHARNUM, FONT_CHARSET,
        ;   BLINK_COL, BLINK_ROW, BLINK_CHAR, TURN_TUNE_GUARD,
        ;   TURN_START_COL, TURN_START_ROW
        ; Clobbers: A, X, Y
        MOVB    CURRENT_COL,FONT_COL
        STA     BLINK_COL
        LDY     CURRENT_ROW
        STY     FONT_ROW
        STY     BLINK_ROW
        JSR     COLROW_TO_POS
        LDY     #MFIELD_POSITION
        STA     (ENTITY_PTR),Y   ; update char packed position
        MOVB    CHAR_SPRITE,FONT_CHARNUM
        STA     BLINK_CHAR
        STOB    #$01,FONT_CHARSET
        JSR     RENDER_FONT_CHAR ; draw sprite
        LDX     #$16
        STX     TURN_TUNE_GUARD
        ; falls through to RESET_TURN_POS
    RESET_TURN_POS:
        ; Reset to turn start: convert start col/row to packed pos,
        ; call $1156, update current->start, conditionally play tune
        LDA     TURN_START_COL
        LDY     TURN_START_ROW
        JSR     COLROW_TO_POS
        JSR     DRAW_CHAR_AT_POS
        MOVB    CURRENT_COL,TURN_START_COL
        MOVB    CURRENT_ROW,TURN_START_ROW
        LDX     TURN_TUNE_GUARD ; X=$16 = tune 11: movement effect
        BXEQ    #$16,_COMMIT_MOVE.run_script
        RTS
    _COMMIT_MOVE.run_script:
        DEC     TURN_TUNE_GUARD
        JMP     TUNE_PLAYER     ; X still $16 from LDX above
        ORG     $1BA7
    END_TURN:
        SUBROUTINE END_TURN
        ; Wrap up the character's turn and check for bonus movement.
        ;
        ; Inputs:
        ;   ENTITY_PTR   — pointer to current entity record
        ;   STEPS_TAKEN  — steps consumed this turn
        ;
        ; Behavior:
        ;   Clears TURN_ACTIVE, calls CHECK_AND_RESTRICT to update the
        ;   restricted flag at the final position.  If no steps taken, loops
        ;   back to TURN_DISPATCH.  Otherwise extracts speed tier from
        ;   MFIELD_LEVEL_ENC bits 2-0 (after ROR) and compares against
        ;   STEPS_TAKEN; if level exceeds steps, grants another turn.
        ;
        ; Modifies:
        ;   TURN_ACTIVE
        ; Clobbers: A, Y
        ; End turn: clear TURN_ACTIVE, check encounter, compare level vs STEPS_TAKEN
        STOB    #$00,TURN_ACTIVE
        JSR     CHECK_AND_RESTRICT
        LDA     STEPS_TAKEN
        BNE     .check_level
        JMP     TURN_DISPATCH
    .check_level:
        LDY     #MFIELD_LEVEL_ENC
        LDA     (ENTITY_PTR),Y
        ROR
        AND     #$07
        BAMI    STEPS_TAKEN,_END_TURN.too_low
        JMP     TURN_DISPATCH
    _END_TURN.too_low:
        ORG     $1BC6
    SET_STATE_FIGHT:
        SUBROUTINE SET_STATE_FIGHT
        ; Set the character's turn state to 1 (fighting).
        ;
        ; Behavior:
        ;   Loads A=1 and tail-calls SET_CHAR_STATE.  State 1 means the
        ;   character is engaged in combat but has not committed to an attack.
        ; Clobbers: A, Y
        LDA     #$01
        JMP     SET_CHAR_STATE
        ORG     $1BCB
    CHECK_AND_RESTRICT:
        SUBROUTINE CHECK_AND_RESTRICT
        ; Evaluate the encounter at the entity's position and update restriction.
        ;
        ; Inputs:
        ;   ENTITY_PTR  — pointer to current entity record
        ;
        ; Behavior:
        ;   Reads MFIELD_POSITION from the entity record, calls CHECK_ENCOUNTER,
        ;   clears the restricted flag, then re-sets it only if ENCOUNTER_RESULT
        ;   is 2 (hostile present).
        ;
        ; Modifies:
        ;   Entity byte MFIELD_FLAGS (bit 2), ENCOUNTER_RESULT
        ; Clobbers: A, Y
        ; Check encounter at char position, clear restricted flag,
        ; set it if encounter type is 2.
        LDY     #MFIELD_POSITION
        LDA     (ENTITY_PTR),Y
        JSR     CHECK_ENCOUNTER
        JSR     CLEAR_RESTRICTED
        LDA     ENCOUNTER_RESULT
        BANE    #$02,_CHECK_AND_RESTRICT.done
        JSR     SET_RESTRICTED
    _CHECK_AND_RESTRICT.done:
        RTS
        ORG     $1BE0
    SET_CHAR_STATE:
        SUBROUTINE SET_CHAR_STATE
        ; Write a new turn-phase state into entity field 15.
        ;
        ; Inputs:
        ;   A           — new state value (0-3)
        ;   ENTITY_PTR  — pointer to current entity record
        ;
        ; Behavior:
        ;   Writes A into the low 2 bits of MFIELD_FLAGS (byte 15),
        ;   preserving the upper 6 bits.  States: 0=idle, 1=fighting,
        ;   2=entering combat, 3=attacking.
        ;
        ; Modifies:
        ;   Entity byte MFIELD_FLAGS (low 2 bits)
        ; Clobbers: A, Y
        ; Set char field 15 low 2 bits to A
        STA     $5A60
        LDY     #MFIELD_FLAGS
        LDA     (ENTITY_PTR),Y
        AND     #$FC              ; ~FLAGS_STATE_MASK: clear low 2 bits
        ORA     $5A60             ; set to new value
        STA     (ENTITY_PTR),Y
        RTS
        ORG     $1BEF
    CLEAR_RESTRICTED:
        SUBROUTINE CLEAR_RESTRICTED
        ; Clear the restricted-movement flag (bit 2 of MFIELD_FLAGS).
        ;
        ; Inputs:
        ;   ENTITY_PTR  — pointer to current entity record
        ;
        ; Modifies:
        ;   Entity byte MFIELD_FLAGS (clears FLAGS_ENGAGED_BIT)
        ; Clobbers: A, Y
        LDY     #MFIELD_FLAGS
        LDA     (ENTITY_PTR),Y
        AND     #$FB              ; ~FLAGS_ENGAGED_BIT
        STA     (ENTITY_PTR),Y
        RTS
        ORG     $1BF8
    SET_RESTRICTED:
        SUBROUTINE SET_RESTRICTED
        ; Set the restricted-movement flag (bit 2 of MFIELD_FLAGS).
        ;
        ; Inputs:
        ;   ENTITY_PTR  — pointer to current entity record
        ;
        ; Behavior:
        ;   Sets FLAGS_ENGAGED_BIT in MFIELD_FLAGS.  Restricted characters
        ;   cannot freely leave their cell — they must fight or flee.
        ;
        ; Modifies:
        ;   Entity byte MFIELD_FLAGS (sets FLAGS_ENGAGED_BIT)
        ; Clobbers: A, Y
        ; Set restricted flag (bit 2 of char field 15)
        LDY     #MFIELD_FLAGS
        LDA     (ENTITY_PTR),Y
        ORA     #FLAGS_ENGAGED_BIT
        STA     (ENTITY_PTR),Y
        RTS
*/
#include "ali_baba.h"

/* CHECK_ENCOUNTER's results, as ENCOUNTER_RESULT keeps them. */
enum {
    ENCOUNTER_NONE = 0,
    ENCOUNTER_FRIENDLY = 1,
    ENCOUNTER_HOSTILE = 2,
};

/* SELECT_COMBAT_TARGET's results. */
enum {
    ACTION_ATTACK = 1,
    ACTION_ENTER_COMBAT = 2,
};

/* The mob states of bits 1-0 of a mob's flags that turn sets. */
enum {
    STATE_IDLE = 0,
    STATE_FIGHTING = 1,
};

/* NPC_MOVE_AI's commands after the directions. */
enum {
    AI_PASS = 5,
    AI_ATTACK = 6,
    AI_ENTER_COMBAT = 7,
    AI_REST = 9,
};

/* The position ENTER_COMBAT_STATE is given from the menu, where the
 * original's JMP (INPUT_JMP_VEC) leaves the high byte of the handler's
 * address, $14B7, in A. */
constexpr uint8_t MENU_COMBAT_POS = 0x14;

/* TURN_TUNE_GUARD while COMMIT_MOVE's tune hasn't played: TUNE_MOVEMENT
 * times 2, which the original passes to TUNE_PLAYER from the guard. */
constexpr uint8_t TURN_TUNE_ARMED = TUNE_MOVEMENT * 2;

uint8_t TURN_ACTIVE = 0x4B;

static uint8_t turn_dispatch(void);
static uint8_t game_turn_loop(void);
static uint8_t move_loop(void);
static uint8_t end_turn(void);
static void set_state_fight(void);
static void check_and_restrict(void);

void run_chain(uint8_t state) {
    while (state != CHAIN_RETURN) {
        switch (state) {
        /* turn.c */
        case CHAIN_GAME_TURN_LOOP: state = game_turn_loop(); break;
        case CHAIN_MOVE_LOOP: state = move_loop(); break;
        case CHAIN_END_TURN: state = end_turn(); break;
        case CHAIN_TURN_DISPATCH: state = turn_dispatch(); break;
        case CHAIN_SET_STATE_FIGHT:
            set_state_fight();
            state = CHAIN_RETURN;
            break;
        /* main.c */
        case CHAIN_TOGGLE_SOUND: state = toggle_sound(); break;
        /* util.c */
        case CHAIN_INIT_CHAR_SELECT: state = init_char_select(); break;
        /* combat.c */
        case CHAIN_AUTO_HEAL:
            auto_heal();
            state = CHAIN_RETURN;
            break;
        case CHAIN_ENTER_COMBAT_STATE:
            enter_combat_state(MENU_COMBAT_POS);
            state = CHAIN_RETURN;
            break;
        case CHAIN_ATTACK_RIGHT: state = attack_right(); break;
        case CHAIN_ATTACK_LEFT: state = attack_left(); break;
        case CHAIN_ATTACK_UP: state = attack_up(); break;
        case CHAIN_ATTACK_DOWN: state = attack_down(); break;
        case CHAIN_FIND_ATTACK_TARGET: state = find_attack_target(); break;
        case CHAIN_SELECT_SOMEONE_ELSE: state = select_someone_else(); break;
        case CHAIN_RESOLVE_ATTACK:
            resolve_attack();
            state = CHAIN_RETURN;
            break;
        case CHAIN_HANDLE_MOB_ENCOUNTER: state = handle_mob_encounter(); break;
        /* input.c */
        case CHAIN_TEARDOWN_INPUT: state = teardown_input(); break;
        case CHAIN_MENU_SEE_YOUR_LOT: state = menu_see_your_lot(); break;
        case CHAIN_MENU_CHECK_ATTACK: state = menu_check_attack(); break;
        case CHAIN_MENU_OTHER_OPTIONS: state = menu_other_options(); break;
        case CHAIN_MENU_GAME_SETTINGS: state = menu_game_settings(); break;
        case CHAIN_MENU_GAME_SETTINGS_ENTER:
            state = menu_game_settings_enter();
            break;
        case CHAIN_SETTINGS_SPEED_UP: state = settings_speed_up(); break;
        case CHAIN_SETTINGS_SPEED_DOWN: state = settings_speed_down(); break;
        case CHAIN_SETTINGS_DANGER_UP: state = settings_danger_up(); break;
        case CHAIN_SETTINGS_DANGER_DOWN: state = settings_danger_down(); break;
        case CHAIN_MENU_ATTACK_TARGET: state = menu_attack_target(); break;
        case CHAIN_MENU_DROP_GOLD: state = menu_drop_gold(); break;
        case CHAIN_MENU_DROP_INC: state = menu_drop_inc(); break;
        case CHAIN_MENU_DROP_DEC: state = menu_drop_dec(); break;
        case CHAIN_INIT_WORLD: state = init_world(); break;
        case CHAIN_SLOT_INC_ENTER: state = slot_inc_enter(); break;
        case CHAIN_SLOT_DEC_ENTER: state = slot_dec_enter(); break;
        case CHAIN_ENTER_SAVE_GAME_SLOT: state = enter_save_game_slot(); break;
        case CHAIN_SLOT_INC_REENTER: state = slot_inc_reenter(); break;
        case CHAIN_SLOT_DEC_REENTER: state = slot_dec_reenter(); break;
        case CHAIN_ENTER_SCENARIO_DISK: state = enter_scenario_disk(); break;
        case CHAIN_ENTER_CHAR_SELECT: state = enter_char_select(); break;
        case CHAIN_CHAR_SELECT_INIT: state = char_select_init(); break;
        case CHAIN_CHAR_SELECT_NEXT: state = char_select_next(); break;
        case CHAIN_CHAR_SELECT_PREV: state = char_select_prev(); break;
        case CHAIN_CHAR_SELECT_HERE: state = char_select_here(); break;
        case CHAIN_CHAR_SELECT_HOME: state = char_select_home(); break;
        case CHAIN_START_GAME_INPUT: state = start_game_input(); break;
        case CHAIN_CONTINUE_GAME: state = continue_game(); break;
        case CHAIN_SAVE_GAME: state = save_game(); break;
        case CHAIN_CANCEL_SCENARIO: state = cancel_scenario(); break;
        case CHAIN_FORMAT_SCENARIO_DISK: state = format_scenario_disk(); break;
        case CHAIN_USE_SCENARIO_DISK: state = use_scenario_disk(); break;
        case CHAIN_RETIRE_CHARACTER:
            retire_character();
            state = CHAIN_RETURN;
            break;
        /* events.c */
        case CHAIN_FLUSH_PENDING_XP: state = flush_pending_xp(); break;
        case CHAIN_INIT_DROP_GOLD: state = init_drop_gold(); break;
        case CHAIN_ADJ_DROP_INC: state = adj_drop_inc(); break;
        case CHAIN_ADJ_DROP_DEC: state = adj_drop_dec(); break;
        case CHAIN_DROP_GOLD_EXEC: state = drop_gold_exec(); break;
        /* ai.c */
        case CHAIN_SHOP_BUY_HANDLER: state = shop_buy_handler(); break;
        case CHAIN_BROWSE_HANDLER: state = browse_handler(); break;
        default: state = CHAIN_RETURN; break;
        }
    }
}

/*
    Opens the action menu for the player, and carries out
    SELECT_COMBAT_TARGET's choice for an AI mob: an attack, entering
    combat, or a rest. The original tests the choice with `CMP #2`,
    BEQ, and then BMI, so that the BMI is taken for a result whose
    difference from 2 is negative as a signed byte. Of the results 1,
    2, and 4 that SELECT_COMBAT_TARGET returns, that is 1, where the
    original's comment says "negative".
*/
static uint8_t turn_dispatch(void) {
    uint8_t action;

    if (CHAR_AI_MODE == 0)
        return CHAIN_TEARDOWN_INPUT;
    action = select_combat_target();
    if (action == ACTION_ENTER_COMBAT)
        enter_combat_state(ACTION_ENTER_COMBAT);
    else if (action < ACTION_ENTER_COMBAT)
        start_combat();
    else
        auto_heal();
    return CHAIN_RETURN;
}

/*
    Starts ENTITY_PTR's turn. A mob that shares its cell with a hostile
    mob, or is outmatched, gets fewer move points. In a hostile mob's
    cell, it keeps at most 1 or 2, at random. Outmatched
    (CHECK_THREATS_HERE returns 0, for a defense below the hostile
    strength next to it), it gets a random number from 1 to its points,
    where a draw of 0 gives it 1 or 2. A mob with fewer than 2 points
    keeps them. The original's comment describes the 1 from
    CHECK_THREATS_HERE as one threat, where it means the mob is safe.
*/
static uint8_t game_turn_loop(void) {
    uint16_t colrow;
    uint8_t col, row;

    TURN_ACTIVE = 1;
    colrow = pos_to_colrow(ENTITY_PTR->pos);
    col = (uint8_t)colrow;
    row = (uint8_t)(colrow >> 8);
    CURRENT_COL = col;
    TURN_START_COL = col;
    CURRENT_ROW = row;
    TURN_START_ROW = row;
    STEPS_TAKEN = 0;
    clear_restricted();
    if ((uint8_t)(ENTITY_PTR->life & HP_MASK) < 3)
        set_restricted();
    if (check_encounter(ENTITY_PTR->pos) == ENCOUNTER_HOSTILE)
        set_restricted();
    if (CHAR_AI_MODE != 0)
        ai_choose_target();

    MOVE_POINTS = ENTITY_PTR->speed & LOW_NIBBLE_MASK;
    if (ENCOUNTER_RESULT == ENCOUNTER_HOSTILE) {
        if (MOVE_POINTS >= 2)
            MOVE_POINTS = random_in_range(1) + 1;
    } else if (check_threats_here() != 1 && MOVE_POINTS >= 2) {
        uint8_t points = random_in_range(MOVE_POINTS);
        if (points != 0)
            MOVE_POINTS = points;
        else
            MOVE_POINTS = random_in_range(1) + 1;
    }
    return CHAIN_MOVE_LOOP;
}

/*
    Takes the mob's steps. A player with no move points left still gets
    a step when it hasn't taken one yet, and its MOVE_POINTS then
    counts down from 255.

    An AI's command 0 moves up, as 1 does, because the original tests
    for up with `CMP #2` and BPL. After a step to an empty cell, the
    turn stops when CHECK_THREATS_HERE finds the mob outmatched there.
*/
static uint8_t move_loop(void) {
    for (;;) {
        uint8_t dir;
        entity_ref e;
        struct event *ev;
        uint8_t type;

        if (MOVE_POINTS == 0 && (STEPS_TAKEN != 0 || CHAR_AI_MODE != 0))
            return CHAIN_END_TURN;

        if (CHAR_AI_MODE != 0) {
            dir = npc_move_ai();
            INPUT_DIR = dir;
            if (dir == AI_PASS)
                continue;
            if (dir == AI_REST) {
                set_restricted();
                set_char_state(STATE_IDLE);
                auto_heal();
                return CHAIN_RETURN;
            }
            if (dir > AI_PASS) {
                check_and_restrict();
                if (dir == AI_ENTER_COMBAT)
                    enter_combat_state(AI_ENTER_COMBAT);
                else if (dir == AI_ATTACK)
                    start_combat();
                else
                    set_state_fight();
                return CHAIN_RETURN;
            }
        } else {
            if (configure_input() == 0)
                return CHAIN_TEARDOWN_INPUT;
            setup_input(get_input_table_ptr());
            dir = read_keyboard();
            INPUT_DIR = dir;
            if (dir == DIR_NONE)
                return CHAIN_TEARDOWN_INPUT;
        }

        /* A mob that shares its cell with a hostile mob has to get
         * away first. */
        if (ENCOUNTER_RESULT >= ENCOUNTER_HOSTILE && !check_can_leave())
            return CHAIN_END_TURN;
        ENCOUNTER_RESULT = ENCOUNTER_NONE;
        MOVE_POINTS--;
        STEPS_TAKEN++;
        if (dir <= DIR_UP)
            CURRENT_ROW--;
        else if (dir == DIR_DOWN)
            CURRENT_ROW++;
        else if (dir == DIR_LEFT)
            CURRENT_COL--;
        else
            CURRENT_COL++;

        e = find_entity_at_pos(colrow_to_pos(CURRENT_COL, CURRENT_ROW));
        if (e == ENTITY_NONE) {
            commit_move();
            if (check_threats_here() == 0) {
                MOVE_POINTS = 0;
                TURN_ACTIVE = 0;
            }
            continue;
        }
        if ((e >> 8) == 0) {
            /* the edge of the map */
            tune_player(TUNE_ENTITY_INTERACTION, 0);
            show_locked_msg();
            return CHAIN_RETURN;
        }
        if (ENTITY_IS_MOB(e))
            return CHAIN_HANDLE_MOB_ENCOUNTER;

        ev = ENTITY_EVENT(e);
        type = ev->type;
        if ((type & EVENT_ACTIVATED) != EVENT_ACTIVATED) {
            handle_encounter(ev);
            return CHAIN_RETURN;
        }
        if (type < EVENT_TYPE_SCENE) {
            pickup_treasure(ev);
            return CHAIN_RETURN;
        }
        if (type < EVENT_TYPE_WEAPON_SHOP) {
            trigger_scene_event(ev);
            return CHAIN_RETURN;
        }
        if (type <= EVENT_TYPE_SHOP_LAST) {
            commit_move();
            return display_shop(ev);
        }
        if (type == EVENT_TYPE_OBJECT)
            return handle_event_slot(ev);
        return CHAIN_TEARDOWN_INPUT;
    }
}

/* Draws the mob at CURRENT_COL, CURRENT_ROW, makes that cell the
 * blinking cell, and stores it as the mob's position. reset_turn_pos
 * then plays the step's tune. */
void commit_move(void) {
    uint8_t col = CURRENT_COL;
    uint8_t row = CURRENT_ROW;

    FONT_COL = col;
    BLINK_COL = col;
    FONT_ROW = row;
    BLINK_ROW = row;
    ENTITY_PTR->pos = colrow_to_pos(col, row);
    FONT_CHARNUM = CHAR_SPRITE;
    BLINK_CHAR = CHAR_SPRITE;
    FONT_CHARSET = 1;
    render_font_char();
    TURN_TUNE_GUARD = TURN_TUNE_ARMED;
    reset_turn_pos();
}

void reset_turn_pos(void) {
    draw_char_at_pos(colrow_to_pos(TURN_START_COL, TURN_START_ROW));
    TURN_START_COL = CURRENT_COL;
    TURN_START_ROW = CURRENT_ROW;
    if (TURN_TUNE_GUARD == TURN_TUNE_ARMED) {
        TURN_TUNE_GUARD--;
        tune_player(TUNE_MOVEMENT, 0);
    }
}

/*
    Gives the turn to TURN_DISPATCH, unless the mob has taken more
    steps than bits 3-1 of its speed byte allow, which leaves it
    fighting. The original compares by `CMP STEPS_TAKEN` and BMI, so
    "more" is the sign of the difference as a signed byte. Its header
    comment says that a level above the steps grants another turn.
*/
static uint8_t end_turn(void) {
    int8_t margin;

    TURN_ACTIVE = 0;
    check_and_restrict();
    if (STEPS_TAKEN == 0)
        return CHAIN_TURN_DISPATCH;
    margin = (int8_t)(((ENTITY_PTR->speed >> 1) & 7) - STEPS_TAKEN);
    if (margin >= 0)
        return CHAIN_TURN_DISPATCH;
    set_state_fight();
    return CHAIN_RETURN;
}

static void set_state_fight(void) {
    set_char_state(STATE_FIGHTING);
}

/* Marks the mob engaged when a hostile mob shares its cell, and clears
 * the mark otherwise. */
static void check_and_restrict(void) {
    uint8_t result = check_encounter(ENTITY_PTR->pos);

    clear_restricted();
    if (result == ENCOUNTER_HOSTILE)
        set_restricted();
}

void set_char_state(uint8_t state) {
    ENTITY_PTR->flags = (ENTITY_PTR->flags & ~FLAGS_STATE_MASK) | state;
}

void clear_restricted(void) {
    ENTITY_PTR->flags &= ~FLAGS_ENGAGED_BIT;
}

void set_restricted(void) {
    ENTITY_PTR->flags |= FLAGS_ENGAGED_BIT;
}
