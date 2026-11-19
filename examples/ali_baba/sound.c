/*
    sound.c -- the tune player and the two tones it clicks the speaker
    with.

    A tune is bytecode in the kept memory at $80C6-$80E4 and
    $A300-$A440, and TUNE_TABLE points at each of the twelve. A byte
    from $27 up is a note. Its frequency is the byte, with the low
    bits that TUNE_PRNG_MASK selects flipped at random, and the next
    byte is its duration. The bytes below $27 are commands that loop, mark a
    place to loop or jump back to, choose the high or the low tone,
    change the mask, or end the tune. After each note and most
    commands, the player steps past the byte, and during the title tune
    it returns as soon as a key or a joystick button is down. That step
    also halves WAIT_LOOP_COUNT, as the original's does.

    A tone clicks the speaker 10 times for each unit of its duration,
    with a delay loop of `freq` passes between clicks. A pass of the
    high tone takes 5 cycles and one of the low tone 10.

    Departures from the original:

    - TUNE_PLAYER takes the tune number times 2 in X. tune_player takes
      the tune number, and the strength that the original takes in Y.
    - The note jumps through TUNE_ACTION_VEC to the tone routine. The C
      compares TUNE_ACTION_VEC with the tone routines' addresses, which
      the HIGH and LOW commands store as the original's do, and calls
      play_low_tone for PLAY_LOW_TONE's and play_high_tone for any
      other value. Every tune chooses a tone before its first note, so
      the original never jumps through another value.
    - The original jumps through DISPATCH_VEC after each note's tone,
      where INIT_GAME_STATE's title tune runs a step of the border
      animation and later tunes find a JMP TUNE_ADVANCE that
      INIT_GAME_STATE patched in. tune_player calls border_anim_step
      while BORDER_ANIM_ON is true.
    - TOGGLE_SOUND turns sound off by writing an RTS over the first
      byte of both tone routines. Here it clears SOUND_ON, and
      tune_player skips the tone. The PRNG still steps for each note.
    - The tone routines start the first delay of a note from the X
      register that the tune player leaves, which is the previous
      note's frequency, the tune number times 2 before a tune's first
      note, or 1 after the title tune's check of the buttons. The C
      keeps that value in `x` and passes it to the tone. During
      INIT_GAME_STATE's tune the original's X is whatever the border
      animation's handler leaves, which this unit can't see, and the C
      uses the previous note's frequency there. This changes only the
      length of the gap after the note's first click.
    - PLAY_LOW_TONE spends the extra cycles of its delay loop with ROL
      TONE_PADDING, and the C with an increment of tone_padding, which
      is the same zero-page byte and compiles to INC, also 5 cycles.
      The loops count down in Y where the original counts in X. With
      this compiler's code, the gap between two clicks is 10X+27
      cycles in the low tone, as in the original, and 5X+17 in the
      high tone, where the original's is 5X+11. Every tenth gap is
      longer by 12 cycles where the original's is longer by 9. The
      harness compares the order of the clicks among the other events
      and not their timing.
    - The scratch bytes SOUND_FREQ ($E1) and SOUND_DURATION ($E2) are
      parameters, and TONE_PADDING ($E3) is the static tone_padding,
      pinned at $E3.

    Ali Baba, main.asm lines 15057-15311:

        ORG     $795B
    TUNE_PLAYER:
        SUBROUTINE TUNE_PLAYER
        ; Bytecode interpreter for sound effects and music.
        ;
        ; Input:  X = tune index (byte offset into TUNE_TABLE:
        ;             $00=empty, $02=hit, $04=damage, $06=attract,
        ;             $08=mob death, $0A=NPC death, $0C=avoided,
        ;             $0E=interact, $10=AI turn, $12=menu, $14=encounter,
        ;             $16=movement)
        ;         Y = combat strength (used only for X=$02 or X=$04,
        ;             patched into script loop count)
        ; Output: sound played; ATTRACT_FLAG set for index $06
        ; Behavior:
        ;   - Clears keyboard strobe
        ;   - Sets ATTRACT_FLAG = 1 for tune index $06, else 0
        ;   - For indices $02/$04, patches loop count from Y-1
        ;   - Loads script address from TUNE_TABLE into TUNE_PC
        ;   - Runs bytecode interpreter (opcodes $00-$26 = control,
        ;     $27+ = note with PRNG-randomized frequency)
        ;   - In attract mode, polls keyboard/joystick each step
        ; Clobbers: A, X, Y

        STA     $C010             ; clear keyboard strobe
        STOB    #$00,ATTRACT_FLAG ; attract_flag = 0
        BXNE    #$06,_TUNE_PLAYER.not_attract ; tune index 6 = attract
        INC     ATTRACT_FLAG      ; attract_flag = 1
    _TUNE_PLAYER.not_attract:

        BXEQ    #$02,_TUNE_PLAYER.setup_2     ; special setup for index 2
        BXEQ    #$04,_TUNE_PLAYER.setup_4     ; special setup for index 4
        JMP     .load_ptr

    _TUNE_PLAYER.setup_2:
        DEY
        STY     $80D5             ; patch byte in script 1 data
        JMP     .load_ptr
    _TUNE_PLAYER.setup_4:
        DEY
        STY     $80E3             ; patch byte in script 2 data
        ORG     $7980

    .load_ptr:
        LDA     TUNE_TABLE,X   ; low byte
        STA     TUNE_PC
        LDA     TUNE_TABLE+1,X ; high byte
        STA     TUNE_PC+1      ; TUNE_PC/TUNE_PC+1 = script start address
        ORG     $798A

        STOB    #$00,TUNE_LOOP_CTR ; loop counter = 0
        STOB    #$01,TUNE_PRNG_MASK ; PRNG mask = 1
    TUNE_PLAYER_FETCH:
    .fetch:
        LDY     #$00
        LDA     (TUNE_PC),Y    ; read script byte
        BALT    #$27,_TUNE_PLAYER.control ; < $27 -> control command
        JMP     .action          ; >= $27 -> action dispatch
        ORG     $799F

    _TUNE_PLAYER.control:
        BAEQ    #$25,_TUNE_PLAYER.cmd_25              ; set PRNG mask = $0F
        BCS     .cmd_26                   ; $26: PRNG mask = $00

        BAEQ    #$23,_TUNE_PLAYER.cmd_23              ; $23: HIGH (PLAY_HIGH_TONE)
        BCS     .cmd_24                   ; $24: END (return)

        BAEQ    #$21,_TUNE_PLAYER.cmd_21              ; $21: gosub
        BCS     .cmd_22                   ; $22: LOW (PLAY_LOW_TONE)

        BAEQ    #$1F,_TUNE_PLAYER.cmd_1F              ; set loop address
        BCS     .cmd_20                   ; set gosub address

        ; --- Loop/end-loop (byte < $1F) ---
        BAEQ    TUNE_LOOP_CTR,_TUNE_PLAYER.end_loop ; compare with loop counter
        BCC     _TUNE_PLAYER.end_loop

        INC     TUNE_LOOP_CTR           ; counter++
        MOVW    TUNE_LOOP_ADDR,TUNE_PC ;  TUNE_PC/TUNE_PC+1 = loop address
        JMP     .fetch

    _TUNE_PLAYER.end_loop:
        STOB    #$00,TUNE_LOOP_CTR ; reset loop counter
        JMP     TUNE_ADVANCE
        ORG     $79D6

    _TUNE_PLAYER.cmd_25:                            ; PRNG mask = $0F
        STOB    #$0F,TUNE_PRNG_MASK
        JMP     TUNE_ADVANCE

    .cmd_26:                            ; PRNG mask = $00
        STOB    #$00,TUNE_PRNG_MASK
        JMP     TUNE_ADVANCE

    _TUNE_PLAYER.cmd_23:                            ; HIGH ($23): action vector -> PLAY_HIGH_TONE
        STOW    PLAY_HIGH_TONE,TUNE_ACTION_VEC
        JMP     TUNE_ADVANCE

    .cmd_22:                            ; LOW ($22): action vector -> PLAY_LOW_TONE
        STOW    PLAY_LOW_TONE,TUNE_ACTION_VEC
        JMP     TUNE_ADVANCE

    .cmd_24:                            ; END ($24): return from tune
        RTS

    _TUNE_PLAYER.cmd_21:                            ; GOTO1 ($21): jump to saved address
        MOVW    TUNE_GOSUB_ADDR,TUNE_PC
        JMP     .fetch

    _TUNE_PLAYER.cmd_1F:                            ; set loop address = current+1
        JSR     TUNE_INC_PC
        MOVW    TUNE_PC,TUNE_LOOP_ADDR
        JMP     .fetch

    .cmd_20:                            ; set gosub address = current+1
        JSR     TUNE_INC_PC
        MOVW    TUNE_PC,TUNE_GOSUB_ADDR
        JMP     .fetch
        ORG     $7A2E

    .action:
        STA     SOUND_DURATION         ; save action byte
        JSR     STEP_PRNG
        LDA     PRNG_OUTPUT            ; PRNG output
        AND     TUNE_PRNG_MASK       ; apply mask
        EOR     SOUND_DURATION         ; XOR with action byte
        STA     SOUND_FREQ             ; modified action
        JSR     TUNE_INC_PC          ; advance tune pointer
        LDY     #$00
        LDA     (TUNE_PC),Y          ; read parameter byte
        STA     SOUND_DURATION
        JSR     TUNE_ACTION_INDIRECT ; -> JMP (TUNE_ACTION_VEC)
        JMP     (DISPATCH_VEC)         ; dispatch via action vector
        ORG     $7A4C
    TUNE_ACTION_INDIRECT:
        JMP     (TUNE_ACTION_VEC)
        ORG     $7A4F
    TUNE_ADVANCE:
        SUBROUTINE TUNE_ADVANCE
        ; Advance the tune player by one step.
        ;
        ; Behavior:
        ;   Increments the tune program counter, then checks for user
        ;   input if in attract mode. Returns early if input detected;
        ;   otherwise halves WAIT_LOOP_COUNT and jumps back to the
        ;   tune interpreter fetch loop.
        ;
        ; Modifies: TUNE_PC, WAIT_LOOP_COUNT
        ; Clobbers: A, X

        JSR     TUNE_INC_PC       ; advance tune pointer
        LDA     ATTRACT_FLAG        ; attract_flag
        BEQ     .continue           ; not attract mode -> skip input check

        ; Attract mode: check keyboard
        LDA     $C000               ; keyboard register
        BPL     .check_joy          ; no key -> check joystick
        RTS                         ; key pressed -> exit

    .check_joy:
        LDA     INPUT_MODE          ; joystick flag
        BEQ     .continue           ; no joystick -> continue script
        LDX     #$00
        LDA     $C061,X             ; button 0
        BPL     .btn1               ; not pressed
        RTS                         ; pressed -> exit
    .btn1:
        INX
        LDA     $C061,X             ; button 1
        BPL     .continue           ; not pressed -> continue
        RTS                         ; pressed -> exit

    .continue:
        LSR     WAIT_LOOP_COUNT     ; shift timing flag
        JMP     TUNE_PLAYER_FETCH ; loop back to interpreter
        ORG     $7A77
    TUNE_INC_PC:
        SUBROUTINE TUNE_INC_PC
        ; Increment the 16-bit tune program counter.
        ;
        ; Behavior:
        ;   Increments TUNE_PC low byte; if it wraps to zero,
        ;   increments TUNE_PC+1 high byte.
        ;
        ; Modifies: TUNE_PC
        ; Clobbers: none (flags only)

        INC     TUNE_PC
        BEQ     .carry
        RTS
    .carry:
        INC     TUNE_PC+1
        RTS
        ORG     $7A7F
    PLAY_HIGH_TONE:
        SUBROUTINE PLAY_HIGH_TONE
        ; Play a high-frequency tone via the speaker.
        ;
        ; Inputs:
        ;   SOUND_FREQ — pitch delay counter
        ;   SOUND_DURATION — number of full cycles to play
        ;
        ; Behavior:
        ;   Toggles the speaker with an inner delay loop. Frequency is
        ;   approximately 510204 / (5*X + 12) Hz.
        ;
        ; Clobbers: A, X, Y

    .cycle:
        LDY     #$0A           ; 10 speaker toggles per cycle
    .toggle:
        LDA     $C030          ; toggle speaker
    .delay:
        DEX
        BNE     .delay         ; inner delay loop (no toggle)
        LDX     SOUND_FREQ     ; reload pitch counter
        DEY
        BNE     .toggle        ; next toggle
        DEC     SOUND_DURATION ; repeat whole cycle
        BNE     .cycle
        RTS
        ORG     $7A91
    PLAY_LOW_TONE:
        SUBROUTINE PLAY_LOW_TONE
        ; Play a low-frequency tone via the speaker.
        ;
        ; Inputs:
        ;   SOUND_FREQ — pitch delay counter
        ;   SOUND_DURATION — number of full cycles to play
        ;
        ; Behavior:
        ;   Same structure as PLAY_HIGH_TONE but with extra cycle padding,
        ;   producing a lower octave. Frequency is approximately
        ;   510204 / (10*X + 31) Hz.
        ;
        ; Clobbers: A, X, Y

    .cycle:
        LDY     #$0A           ; 10 toggles per cycle
    .toggle:
        LDA     $C030          ; toggle speaker
    .delay:
        ROL     TONE_PADDING   ; cycle padding (5 cycles)
        DEX
        BNE     .delay         ; inner delay loop (no toggle)
        LDX     SOUND_FREQ     ; reload pitch counter
        ROL     TONE_PADDING   ; \ more cycle padding
        ROR     TONE_PADDING   ; / (ROL+ROR cancels out)
        NOP                    ; \
        NOP                    ;  | timing padding
        NOP                    ; /
        DEY
        BNE     .toggle        ; next toggle
        DEC     SOUND_DURATION ; repeat whole cycle
        BNE     .cycle
        RTS
*/
#include "ali_baba.h"

/* The tune bytecode's commands. A byte from TUNE_OP_NOTE up is a note,
 * and the byte after it is the note's duration. */
enum {
    TUNE_OP_LOOP_MAX = 0x1E,    /* 0-$1E: repeat from MARK2's place */
    TUNE_OP_MARK2 = 0x1F,       /* the next byte is where LOOP repeats from */
    TUNE_OP_MARK1 = 0x20,       /* the next byte is where GOTO1 goes */
    TUNE_OP_GOTO1 = 0x21,
    TUNE_OP_LOW = 0x22,         /* the notes that follow use the low tone */
    TUNE_OP_HIGH = 0x23,        /* ... or the high tone */
    TUNE_OP_END = 0x24,
    TUNE_OP_HIRAND = 0x25,      /* flip up to 4 low bits of a note at random */
    TUNE_OP_NORAND = 0x26,      /* play the notes as written */
    TUNE_OP_NOTE = 0x27,
};

/* The original's tone routines, whose addresses TUNE_ACTION_VEC
 * holds. */
constexpr uint16_t PLAY_HIGH_TONE_ADDR = 0x7A7F;
constexpr uint16_t PLAY_LOW_TONE_ADDR = 0x7A91;

/* Each unit of a note's duration is this many clicks. */
constexpr uint8_t CLICKS_PER_UNIT = 10;

bool SOUND_ON = true;

/* Bumped only to spend 5 cycles in each pass of the low tone's delay
 * loop, which it does as a zero-page byte. Nothing reads it. It is the
 * original's TONE_PADDING. */
static uint8_t tone_padding [[c6502::zeropage(0xE3)]];

/* Clicks the speaker 10 times `duration` times (0 meaning 256), with
 * `first` passes of 5 cycles before the next click the first time, and
 * `freq` passes after that. A count of 0 means 256 passes. The count
 * is a register local, so that a pass of the delay loop is DEY and BNE,
 * 5 cycles, as the original's DEX and BNE are. Without `register` the
 * count goes to zero page, and a pass takes 8 cycles, which lowers the
 * pitch. */
static void play_high_tone(uint8_t freq, uint8_t duration, uint8_t first) {
    register uint8_t x = first;
    do {
        uint8_t clicks = CLICKS_PER_UNIT;
        do {
            (void)SPKR;
            /* DEY; BNE: 5 cycles a pass. */
            [[c6502::keep]] for (; --x;) {
            }
            x = freq;
        } while (--clicks);
    } while (--duration);
}

/* The same with passes of 10 cycles, an octave lower. */
static void play_low_tone(uint8_t freq, uint8_t duration, uint8_t first) {
    register uint8_t x = first;
    do {
        uint8_t clicks = CLICKS_PER_UNIT;
        do {
            (void)SPKR;
            /* INC zp; DEY; BNE: 10 cycles a pass. */
            [[c6502::keep]] for (; tone_padding++, --x;) {
            }
            /* The original's ROL and ROR of TONE_PADDING, 10 cycles,
             * which make the gap between clicks 10X+27 cycles as in
             * the original. */
            tone_padding++;
            tone_padding++;
            x = freq;
        } while (--clicks);
    } while (--duration);
}

void tune_player(uint8_t tune, uint8_t strength) {
    /* The original's X, which the tone routines start their first
     * delay from. */
    uint8_t x = 2 * tune;

    KBDSTRB = 0;
    ATTRACT_FLAG = tune == TUNE_ATTRACT;
    /* The combat tunes repeat their sound `strength` times. */
    if (tune == TUNE_COMBAT_HIT)
        TUNE_1_COMBAT_HIT[14] = strength - 1;
    else if (tune == TUNE_COMBAT_DAMAGE)
        TUNE_2_COMBAT_DAMAGE[12] = strength - 1;
    TUNE_PC = TUNE_TABLE[tune];
    TUNE_LOOP_CTR = 0;
    TUNE_PRNG_MASK = 1;

    for (;;) {
        uint8_t op = *TUNE_PC;
        if (op >= TUNE_OP_NOTE) {
            uint8_t freq = op ^ (step_prng() & TUNE_PRNG_MASK);
            uint8_t duration = *++TUNE_PC;
            if (SOUND_ON) {
                if (TUNE_ACTION_VEC == PLAY_LOW_TONE_ADDR)
                    play_low_tone(freq, duration, x);
                else
                    play_high_tone(freq, duration, x);
                x = freq;
            }
            if (BORDER_ANIM_ON)
                border_anim_step();
        } else {
            switch (op) {
            case TUNE_OP_MARK2:
                TUNE_PC++;
                TUNE_LOOP_ADDR = TUNE_PC;
                continue;
            case TUNE_OP_MARK1:
                TUNE_PC++;
                TUNE_GOSUB_ADDR = TUNE_PC;
                continue;
            case TUNE_OP_GOTO1:
                TUNE_PC = TUNE_GOSUB_ADDR;
                continue;
            case TUNE_OP_LOW:
                TUNE_ACTION_VEC = PLAY_LOW_TONE_ADDR;
                break;
            case TUNE_OP_HIGH:
                TUNE_ACTION_VEC = PLAY_HIGH_TONE_ADDR;
                break;
            case TUNE_OP_END:
                return;
            case TUNE_OP_HIRAND:
                TUNE_PRNG_MASK = 0x0F;
                break;
            case TUNE_OP_NORAND:
                TUNE_PRNG_MASK = 0x00;
                break;
            default:
                /* LOOP n repeats the bytes after MARK2 n more times. */
                if (op > TUNE_LOOP_CTR) {
                    TUNE_LOOP_CTR++;
                    TUNE_PC = TUNE_LOOP_ADDR;
                    continue;
                }
                TUNE_LOOP_CTR = 0;
                break;
            }
        }

        /* TUNE_ADVANCE. During the title tune, a key or a joystick
         * button ends the tune. */
        TUNE_PC++;
        if (ATTRACT_FLAG) {
            if (KBD & 0x80)
                return;
            if (INPUT_MODE) {
                if (BUTN[0] & 0x80)
                    return;
                x = 1;
                if (BUTN[1] & 0x80)
                    return;
            }
        }
        WAIT_LOOP_COUNT >>= 1;
    }
}
