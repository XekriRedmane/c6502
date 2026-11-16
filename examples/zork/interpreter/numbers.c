/*
    numbers.c -- arithmetic that isn't an add, a subtract, or a logical
    operation, and printing a number.

    The Z-machine's numbers are signed 16-bit words. mul, div, and mod
    work on their magnitudes and then set the sign. They're loops of
    shifts and adds here, as in the original, so that the program calls
    none of the runtime's multiply and divide helpers.

    The original's check_sign has a quirk that this keeps. It is
    meant to take the magnitudes of both operands, SCRATCH2 and
    SCRATCH1, counting the negative ones in SIGN_BIT. But its helper
    flip_sign negates SCRATCH2 whichever operand it checked, so a
    negative second operand negates the first again and keeps its own
    sign. A product is still right, since a product's low 16 bits
    don't depend on whether the operands are read as signed. A
    quotient isn't: 7 / -2 divides -7 by -2 as unsigned numbers and
    gives 0. mod, which sets no sign, gives the remainder of the
    magnitudes for a negative first operand: -7 % 2 is 1.

    Departures from the original:

    - The operands and results are arguments and return values, where
      the original passes them in SCRATCH1 and SCRATCH2. divu16
      returns its quotient and leaves the remainder in `remainder`.
    - mulu16 computes only the product's low 16 bits, which is all
      that's used, and stops when no bits of the multiplier are left.
      The original also computes the high 16 bits, in SCRATCH1.
    - print_number keeps its digits in a local array where the
      original pushes them on the 6502's stack.
    - get_random rotates RNDH left through the carry, which is always
      clear when instr_random calls it. Here that's a shift.

    Zork I (r15), output/main.asm lines 1474-1537, 2032-2083,
    2096-2190, 3791-3796:

    instr_mul:
        MOVW     OPERAND0, SCRATCH2
        MOVW     OPERAND1, SCRATCH1
        JSR      check_sign
        LDA      SCRATCH1+1
        BNE      .do_mult
        LDA      SCRATCH1
        CMP      #$02
        BEQ      .shortcut_x2
        CMP      #$04
        BEQ      .shortcut_x4
    .do_mult:
        JSR      mulu16
    stretch_set_sign:
        JSR      set_sign
        JMP      store_and_next
    .shortcut_x4:
        ASL      SCRATCH2
        ROL      SCRATCH2+1
    .shortcut_x2:
        ASL      SCRATCH2
        ROL      SCRATCH2+1
        JMP      stretch_set_sign
    instr_div:
        MOVW     OPERAND0, SCRATCH2
        MOVW     OPERAND1, SCRATCH1
        JSR      check_sign
        LDA      SCRATCH1+1
        BNE      .do_div
        LDA      SCRATCH1
        CMP      #$02
        BEQ      .shortcut_div2
        CMP      #$04
        BEQ      .shortcut_div4
    .do_div:
        JSR      divu16
        JMP      stretch_set_sign
    .shortcut_div4:
        LSR      SCRATCH2+1
        ROR      SCRATCH2
    .shortcut_div2:
        LSR      SCRATCH2+1
        ROR      SCRATCH2
        JMP      stretch_set_sign
    instr_mod:
        MOVW     OPERAND0, SCRATCH2
        MOVW     OPERAND1, SCRATCH1
        JSR      check_sign
        JSR      divu16
        MOVW     SCRATCH1, SCRATCH2
        JMP      store_and_next
    print_number:
        LDA      SCRATCH2+1
        BPL      .print_positive
        JSR      print_negative_num
    .print_positive:
        STOB     #$00, SCRATCH3
    .loop:
        LDA      SCRATCH2+1
        ORA      SCRATCH2
        BEQ      .is_zero
        STOW     #$000A, SCRATCH1
        JSR      divu16
        LDA      SCRATCH1
        PHA
        INC      SCRATCH3
        JMP      .loop
    .is_zero:
        LDA      SCRATCH3
        BEQ      .print_0
    .print_digit:
        PLA
        CLC
        ADC      #$30           ; '0'
        JSR      buffer_char
        DEC      SCRATCH3
        BNE      .print_digit
        RTS
    .print_0:
        LDA      #$30           ; '0'
        JMP      buffer_char
    print_negative_num:
        LDA      #$2D           ; '-'
        JSR      buffer_char
        JMP      negate
    instr_random:
        MOVW     OPERAND0, SCRATCH1
        JSR      get_random
        JSR      divu16
        MOVW     SCRATCH1, SCRATCH2
        INCW     SCRATCH2
        JMP      store_and_next
    mulu16:
        PSHW     SCRATCH3
        STOW     #$0000, SCRATCH3
        LDX      #$10
    .loop:
        LDA      SCRATCH1
        CLC
        AND      #$01
        BEQ      .next_bit
        ADDWC    SCRATCH2, SCRATCH3, SCRATCH3
    .next_bit:
        RORW      SCRATCH3
        RORW      SCRATCH1
        DEX
        BNE      .loop
        MOVW     SCRATCH1, SCRATCH2
        MOVW     SCRATCH3, SCRATCH1
        PULW     SCRATCH3
        RTS
    divu16:
        PSHW     SCRATCH3
        MOVW     SCRATCH2, SCRATCH3 ; SCRATCH3 is the dividend
        STOW     #$0000, SCRATCH2   ; SCRATCH2 is the remainder
        LDX      #$11
    .loop:
        SEC                     ; carry = "not borrow"
        LDA      SCRATCH2       ; Remainder minus divisor (low byte)
        SBC      SCRATCH1
        TAY
        LDA      SCRATCH2+1
        SBC      SCRATCH1+1
        BCC      .skip          ; Divisor did not fit
        ; At this point carry is set, which will affect
        ; the ROLs below.
        STA      SCRATCH2+1     ; Save remainder
        TYA
        STA      SCRATCH2
    .skip:
        ROLW     SCRATCH3       ; Shift carry into divisor/quotient left
        ROLW     SCRATCH2       ; Shift divisor/remainder left
        DEX
        BNE      .loop          ; loop end
        CLC                     ; SCRATCH1 = SCRATCH2 >> 1
        LDA      SCRATCH2+1
        ROR
        STA      SCRATCH1+1
        LDA      SCRATCH2
        ROR
        STA      SCRATCH1           ; remainder
        MOVW     SCRATCH3, SCRATCH2 ; quotient
        PULW     SCRATCH3
        RTS
    check_sign:
        STOB     #$00, SIGN_BIT
        LDA      SCRATCH2+1
        JSR      flip_sign
        LDA      SCRATCH1+1
        JSR      flip_sign
        RTS
    set_sign:
        LDA      SIGN_BIT
        AND      #$01
        BNE      negate
        RTS
    negate:
        SUBWL    #$0000, SCRATCH2, SCRATCH2
        RTS
    flip_sign:
        ORA      #$00
        BMI      .do_negate
        RTS
    .do_negate:
        INC      SIGN_BIT
        JMP      negate
    get_random:
        ROL      RANDOM_VAL+1
        MOVW     RANDOM_VAL, SCRATCH2
        RTS
*/
#include "zork.h"

/* How many of mul's, div's, or mod's operands check_sign found
 * negative. */
static uint8_t sign_bit;
/* The remainder of the last divu16. */
static uint16_t remainder;

/* Returns the low 16 bits of a * b. */
static uint16_t mulu16(uint16_t a, uint16_t b) {
    uint16_t product = 0;
    while (b != 0) {
        if (b & 1)
            product += a;
        a <<= 1;
        b >>= 1;
    }
    return product;
}

/* Returns dividend / divisor, unsigned, and leaves the remainder in
 * `remainder`. This is the original's loop step for step: seventeen
 * steps, each of which subtracts the divisor from the partial
 * remainder if it fits and shifts the result bit into the quotient.
 * The bit shifted out of the partial remainder's top is lost, as in
 * the original, which matters only for divisors above $8000. */
static uint16_t divu16(uint16_t dividend, uint16_t divisor) {
    uint16_t quotient = dividend;
    uint16_t partial = 0;
    for (uint8_t i = 17; i != 0; i--) {
        uint8_t fits = 0;
        if (partial >= divisor) {
            partial -= divisor;
            fits = 1;
        }
        uint8_t out = (uint8_t)(quotient >> 15);
        quotient = (quotient << 1) | fits;
        partial = (partial << 1) | out;
    }
    remainder = partial >> 1;
    return quotient;
}

/* Returns `a` with its sign taken off, counting the negative operands
 * in sign_bit, with the original's quirk: a negative `b` negates `a`
 * again, and stays negative. */
static uint16_t check_sign(uint16_t a, uint16_t b) {
    sign_bit = 0;
    if ((int16_t)a < 0) {
        sign_bit++;
        a = -a;
    }
    if ((int16_t)b < 0) {
        sign_bit++;
        a = -a;
    }
    return a;
}

/* Stores `n`, negated if an odd number of operands were negative. */
static void store_with_sign(uint16_t n) {
    if (sign_bit & 0x01)
        n = -n;
    store_result(n);
}

void instr_mul(void) {
    uint16_t b = OPERANDS[1];
    uint16_t a = check_sign(OPERANDS[0], b);
    uint16_t product;
    if (b == 2)
        product = a << 1;
    else if (b == 4)
        product = a << 2;
    else
        product = mulu16(a, b);
    store_with_sign(product);
}

void instr_div(void) {
    uint16_t b = OPERANDS[1];
    uint16_t a = check_sign(OPERANDS[0], b);
    uint16_t quotient;
    if (b == 2)
        quotient = a >> 1;
    else if (b == 4)
        quotient = a >> 2;
    else
        quotient = divu16(a, b);
    store_with_sign(quotient);
}

void instr_mod(void) {
    uint16_t b = OPERANDS[1];
    divu16(check_sign(OPERANDS[0], b), b);
    store_result(remainder);
}

/* Stores a random number from 1 to OPERAND0. The Monitor's KEYIN
 * counts RND up while it waits for a key. */
void instr_random(void) {
    /* RNDH, RND's high byte, alone. */
    ((volatile uint8_t *)&RND)[1] <<= 1;
    divu16(RND, OPERANDS[0]);
    store_result(remainder + 1);
}

/* Adds the signed number `n` to the line buffer in decimal. */
void print_number(uint16_t n) {
    if ((int16_t)n < 0) {
        buffer_char('-');
        n = -n;
    }
    uint8_t digits[5];
    uint8_t count = 0;
    while (n != 0) {
        n = divu16(n, 10);
        digits[count] = (uint8_t)remainder;
        count++;
    }
    if (count == 0) {
        buffer_char('0');
        return;
    }
    do {
        count--;
        buffer_char(digits[count] + '0');
    } while (count != 0);
}
