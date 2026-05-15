#include <stdint.h>

/* Speaker-click pointer (low byte at ZP_SFX_CLICK in the asm).
* Set by the input handler to either:
*   $C030 (SW_SPEAKER) -- reads toggle the speaker membrane.
*   $C020 (SW_CASSOUT) -- reads are harmlessly silent.
* Only the low byte toggles when Ctrl-S mutes/unmutes; the high
* byte is always $C0 (the I/O soft-switch page). */
extern const volatile uint8_t *sfx_click_ptr;

/**
* Speaker-click tone / delay generator.
*
* Emits `duration` speaker clicks (or silent reads when sound is muted)
* with a `pitch`-cycle delay between successive clicks.  The delay is a
* tight decrement-to-zero inner loop, so the wall-clock period of each
* click scales linearly with pitch -- larger pitch yields a lower
* frequency.
*
* The hardware click is performed by a volatile read through
* sfx_click_ptr; the input handler swaps that pointer between the
* speaker and the silent cassette-output soft-switch to mute/unmute.
*
* @param pitch     Inner-delay iteration count.  0 is treated as 256
*                  because the decrement-to-zero loop wraps through $FF
*                  on the first iteration (matches the 6502 DEY/BNE
*                  idiom of the original).
* @param duration  Outer click count.  0 is likewise treated as 256 by
*                  the same wrap-around rule.
*/
__attribute__((zp_abi))
void sfx_tone(uint8_t pitch, uint8_t duration)
{
    do {
        volatile uint8_t y = pitch;
        while (--y != 0) { }       /* inner delay: count y down to 0 */
        (void)*sfx_click_ptr;      /* volatile read = click or silent */
    } while (--duration != 0);
}
