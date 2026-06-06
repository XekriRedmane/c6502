// paint_strip_reg.c — register-passed leaf helper demo.
//
// Demonstrates the `register` keyword on a 1-byte-typed leaf function
// with two parameters and a 1-byte return, plus a `register` local.
// Register placement is POSITIONAL: the first register arg-byte goes
// in A, the second in X. So `x_pixel` arrives in A and `color` in X;
// the 1-byte return value comes back in A. The `register` local
// `tmp` is pinned to Y (locals may only use Y).
//
// Eligibility (enforced by passes/abi_selection.py):
//   - Char / SChar / UChar parameters and return type fit the 2-byte
//     (A, X) arg budget; a `register` local fits its single byte (Y).
//   - No pointers in registers.
//   - Function must be zp_abi-eligible: leaf or only-zp_abi-callees,
//     no recursion, address not taken, params fit the ZP window.
//   - No `&` on a `register` parameter or local (C99 §6.5.3.2.1).
//
// Generated code (under --optimize): caller emits `LDA #<x_pixel>;
// LDX #<color>; JSR paint_strip_reg`. Inside the function, the entry
// stub copies A / X into the function's ZP slots so the rest of the
// body reads them like any other zp_abi byte; the result is returned
// in A. The `register` local rides in Y across its lifetime.

static unsigned char hud_buf[40];

unsigned char paint_strip_reg(
    register unsigned char x_pixel,
    register unsigned char color)
{
    register unsigned char tmp = color;
    hud_buf[x_pixel] = tmp;
    return tmp;
}

int main(void) {
    paint_strip_reg(3, 0x7F);
    paint_strip_reg(7, 0x40);
    return hud_buf[3] + hud_buf[7];
}
