// paint_strip_reg.c — register-passed leaf helper demo.
//
// Demonstrates `__attribute__((reg("..")))` on a 1-byte-typed leaf
// function with two parameters and a 1-byte return. The arguments
// arrive in X (`x_pixel`) and Y (`color`); the return value comes
// back in A (the default, made explicit here for readability).
//
// Eligibility (enforced by passes/abi_selection.py):
//   - Char / SChar / UChar parameters and return type — single 6502
//     register holds at most one byte.
//   - Function must be zp_abi-eligible: leaf or only-zp_abi-callees,
//     no recursion, address not taken, params fit the ZP window.
//   - No `&` on a reg-qualified parameter (C99 §6.5.3.2.1 — same
//     constraint as the bare `register` keyword).
//
// Generated code (under --optimize, with the runtime's HUD_BASE ZP
// pair already initialized): caller emits `LDX #<x_pixel>; LDY
// #<color>; JSR paint_strip_reg`. Inside the function, the entry
// stub copies X / Y into the function's ZP slots so the rest of the
// body reads them like any other zp_abi byte; the trailing `LDA`
// of the result satisfies the implicit A-return. Cost vs. the
// default zp_abi: caller saves `STA __zpabi_<fn>__<param>` per
// arg (2 bytes / 3 cycles) for a one-time `STX / STY <slot>` at
// function entry (3 bytes / 4 cycles). Worth it when the caller
// is hot and the callee is cold.

static unsigned char hud_buf[40];

unsigned char paint_strip_reg(
    unsigned char x_pixel __attribute__((reg("X"))),
    unsigned char color __attribute__((reg("Y"))))
{
    hud_buf[x_pixel] = color;
    return color;
}

int main(void) {
    paint_strip_reg(3, 0x7F);
    paint_strip_reg(7, 0x40);
    return hud_buf[3] + hud_buf[7];
}
