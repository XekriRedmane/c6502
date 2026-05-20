#include <stdint.h>

/* Speaker-click pointer.  Set by the input handler to either:
*   $C030 (SW_SPEAKER) -- reads toggle the speaker membrane.
*   $C020 (SW_CASSOUT) -- reads are harmlessly silent.
* Only the low byte toggles when Ctrl-S mutes/unmutes; the high
* byte is always $C0 (the I/O soft-switch page). */
extern const volatile uint8_t *sfx_click_ptr;

/**
* Speaker-click emitter with ascending pitch (perceived: descending tone).
*
* Mirror of sfx_tone / snd_delay_down: emits `clicks` speaker clicks
* with the delay before each click one unit longer than the previous,
* so the gap between clicks grows and the perceived pitch falls
* across the call.
*
* The hardware click is performed by a volatile read through
* sfx_click_ptr; that pointer is swapped between the speaker and the
* silent cassette-output soft-switch by the input handler to mute /
* unmute.
*
* Note: the starting pitch wraps modulo 256 across the call.  After
* 256-`pitch` clicks, the delay counter wraps from $FF back to $00
* (treated as 256 by the same wrap-around rule used by the inner
* loop), so very long calls cycle audibly.
*
* @param pitch   Starting inner-delay iteration count for the first
*                click.  0 is treated as 256 (the decrement-to-zero
*                inner loop wraps through $FF on the first iteration,
*                matching the 6502 DEY/BNE idiom of the original).
*                Each subsequent click uses pitch+1, pitch+2, ...
*                (mod 256).
* @param clicks  Number of clicks to emit.  0 is likewise treated as
*                256 by the same wrap-around rule.
*/
void snd_delay_up(uint8_t pitch __attribute__((reg("A"))), uint8_t clicks __attribute__((reg("X"))))
{
    do {
        /* volatile is essential: without it the compiler would observe
        * that the inner loop has no externally visible side effects
        * and delete it, collapsing the audible delay to nothing.
        * The volatile-qualified accesses are required side effects
        * per C99 6.7.3, so the read-modify-write must execute. */
        volatile uint8_t y __attribute__((reg("Y"))) = pitch;
        pitch = (uint8_t)(pitch + 1);   /* next click's delay grows by 1 */
        while (--y != 0) { }            /* inner delay: count y down to 0 */
        (void)*sfx_click_ptr;           /* volatile read = click or silent */
    } while (--clicks != 0);
}
