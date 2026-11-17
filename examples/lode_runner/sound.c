/*
    sound.c -- the speaker: the queue of notes, the falling tone, the
    beep, and the delays that pace the game.

    A note is played by clicking the speaker, with a count of passes
    between clicks for its pitch, until 256 times its duration in
    passes have gone by. The notes to play are queued in SOUND_PITCH
    and SOUND_DURATION, and play_sound plays one each turn, the last
    queued first. A turn with no note queued plays the falling tone
    while the player falls and is otherwise a delay, so that every
    turn takes about the same time.

    Departures from the original:

    - LOAD_SOUND_DATA reads its notes from the bytes after its JSR,
      and returns past them. load_sound_data takes a pointer to them,
      and queues each one with append_note, which stores the pitch and
      the duration in the other order. Nothing else reads them in
      between.
    - PLAY_NOTE counts down from the Y its caller leaves. play_note
      takes it as `y`, and play_sound passes what the original's Y is
      on each path: NOTE_INDEX for a queued note and 0 for the falling
      tone.

    Lode Runner, main.asm lines 6859-6901, 7222-7345:

        ORG    $86B1
    SOUND_DELAY:
        SUBROUTINE

        LDA     SOUND_DELAY_AMOUNTS,X
        TAX

    SOUND_DELAY1:
        LDY     #$B4         ; 180
    .loop:
        DEY                  ; 2 cycles
        BNE     .loop        ; 3 cycles
        DEX                  ; 2 cycles
        BNE     SOUND_DELAY1 ; 3 cycles
        RTS

        ORG    $86BE
    SOUND_DELAY_AMOUNTS:
        HEX     02 04 06 08 0A 0C 0E 10 12 14 16 18 1A 1C 1E 20
        ORG    $86CE
    BEEP:
        SUBROUTINE

        LDY     #$C0

    .loop:
        ; From here to click is 651 cycles. Additional 5 cycles afterwards.
        LDX     #$80            ; 2 cycles

        ; delay 640 cycles
    .loop2:
        DEX                     ; 2 cycles
        BNE     .loop2          ; 3 cycles

        LDA     ENABLE_SOUND    ; 3 cycles
        BEQ     .next           ; 3 cycles
        LDA     SPKR            ; 3 cycles

    .next:
        DEY                     ; 2 cycles
        BNE     .loop           ; 3 cycles
        RTS

    ...


        ; Keyboard routines

        ORG    $87BA
    PLAY_NOTE:
        SUBROUTINE

        STA     TMP_PTR
        STX     TMP_PTR+1

    .loop:
        LDA     ENABLE_SOUND
        BEQ     .decrement_counter
        LDA     SPKR

    .decrement_counter:
        DEY
        BNE     .counter_decremented
        DEC     TMP_PTR+1
        BEQ     .end

    .counter_decremented:
        DEX
        BNE     .decrement_counter
        LDX     TMP_PTR
        JMP     .loop

    .end:
        RTS

        ORG    $87D5
    APPEND_NOTE:
        SUBROUTINE

        INC     NOTE_INDEX
        LDY     NOTE_INDEX
        STA     SOUND_PITCH,Y
        TXA
        STA     SOUND_DURATION,Y
        RTS

        ORG    $87E1
    LOAD_SOUND_DATA:
        SUBROUTINE

        PLA
        STA     SAVED_RET_ADDR
        PLA
        STA     SAVED_RET_ADDR+1
        BNE     .next

    .loop:
        LDY     #$00
        LDA     (SAVED_RET_ADDR),Y
        BEQ     .end
        INC     NOTE_INDEX
        LDX     NOTE_INDEX
        STA     SOUND_DURATION,X
        INY
        LDA     (SAVED_RET_ADDR),Y
        STA     SOUND_PITCH,X

        INC     SAVED_RET_ADDR
        BNE     .next
        INC     SAVED_RET_ADDR+1

    .next:
        INC     SAVED_RET_ADDR
        BNE     .loop
        INC     SAVED_RET_ADDR+1
        BNE     .loop

    .end:
        LDA     SAVED_RET_ADDR+1
        PHA
        LDA     SAVED_RET_ADDR
        PHA
        RTS

        ORG    $8811
    PLAY_SOUND:
        SUBROUTINE

        LDY     NOTE_INDEX
        BEQ     .no_more_notes
        LDA     SOUND_PITCH,Y
        LDX     SOUND_DURATION,Y
        JSR     PLAY_NOTE

        LDY     NOTE_INDEX              ; Y = NOTE_INDEX
        DEC     NOTE_INDEX              ; NOTE_INDEX--
        LDA     FRAME_PERIOD
        SEC
        SBC     SOUND_DURATION,Y        ; A = FRAME_PERIOD - SOUND_DURATION[Y]
        BEQ     .done
        BCC     .done                   ; If A <= 0, done.
        TAX
        JSR     SOUND_DELAY1

    .done:
        SEC
        RTS

    .no_more_notes:
        LDA     $9B
        BNE     .end
        LDA     $A4
        LSR                     ; pitch = $A4 >> 1
        INC     $A4             ; $A4++
        LDX     FRAME_PERIOD
        INX
        INX                     ; duration = FRAME_PERIOD + 2
        JSR     PLAY_NOTE

        CLC
        RTS

    .end:
        LDX     FRAME_PERIOD
        JSR     SOUND_DELAY

        CLC
        RTS
*/
#include "lode_runner.h"

/* sound_delay's count of 180-pass loops for each FRAME_PERIOD. */
static const uint8_t SOUND_DELAY_AMOUNTS[16] = {
    0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10,
    0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20,
};

/* Waits SOUND_DELAY_AMOUNTS[index] times 180 passes. */
void sound_delay(uint8_t index) {
    sound_delay1(SOUND_DELAY_AMOUNTS[index]);
}

/* Waits `count` times 180 passes, and 256 times for a count of 0. */
void sound_delay1(uint8_t count) {
    do {
#pragma c6502 loop keep
        for (uint8_t y = 0xB4; --y;) {
        }
    } while (--count);
}

/* Clicks the speaker 192 times, 128 passes apart, if sound is on. */
void beep(void) {
    for (uint8_t n = 0xC0; n != 0; n--) {
#pragma c6502 loop keep
        for (uint8_t x = 0x80; --x;) {
        }
        if (ENABLE_SOUND)
            (void)SPKR;
    }
}

/* The first click is followed by `duration` passes, where every later
 * one is followed by `pitch` passes, as in the original. Every pass
 * counts `y` down, and each time it reaches 0 counts the duration
 * down, so the note lasts y + 256 * (duration - 1) passes, where a
 * `y` or a `duration` of 0 counts as 256. */
void play_note(uint8_t pitch, uint8_t duration, uint8_t y) {
    uint8_t x = duration;
    for (;;) {
        if (ENABLE_SOUND)
            (void)SPKR;
        do {
            if (--y == 0 && --duration == 0)
                return;
        } while (--x);
        x = pitch;
    }
}

/* Queues a note to play before the ones already queued. */
void append_note(uint8_t pitch, uint8_t duration) {
    NOTE_INDEX++;
    SOUND_PITCH[NOTE_INDEX] = pitch;
    SOUND_DURATION[NOTE_INDEX] = duration;
}

void load_sound_data(const uint8_t *notes) {
    for (; notes[0] != 0; notes += 2)
        append_note(notes[1], notes[0]);
}

/* A queued note is followed by a delay of what's left of the turn,
 * FRAME_PERIOD less the note's duration in 180-pass loops. The
 * falling tone lasts FRAME_PERIOD + 2, and its clicks move one pass
 * further apart every other turn. */
bool play_sound(void) {
    uint8_t i = NOTE_INDEX;
    if (i != 0) {
        play_note(SOUND_PITCH[i], SOUND_DURATION[i], i);
        NOTE_INDEX--;
        if (FRAME_PERIOD > SOUND_DURATION[i])
            sound_delay1(FRAME_PERIOD - SOUND_DURATION[i]);
        return true;
    }
    if (NOT_FALLING) {
        sound_delay(FRAME_PERIOD);
        return false;
    }
    play_note(FALL_PITCH >> 1, FRAME_PERIOD + 2, 0);
    FALL_PITCH++;
    return false;
}
