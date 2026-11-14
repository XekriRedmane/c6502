#include <stdint.h>
#include <apple2/monitor_io.h>

/*
    dump_buffer_to_screen and dump_buffer_to_printer, from the Zork I
    (r15) Z-machine interpreter for the Apple II, as reverse-engineered
    in https://github.com/XekriRedmane/zorki-r15-reveng (main.nw, "The
    text buffer").

    Zork breaks lines between words, so it collects output in
    BUFF_AREA ($0200), with the offset of the next free byte in
    BUFF_END ($EB). dump_buffer_to_screen prints the collected line
    with the Monitor's COUT1 and empties the buffer.
    dump_buffer_to_printer prints it with COUT while CSW points at the
    printer card's output routine, whose address Zork keeps in
    PRINTER_CSW ($EE), and leaves the buffer as it is. The first time,
    it sends the card ctrl-I "80N", which sets 80-column output, and
    stores $91 at $0779, which is slot 1's scratchpad RAM. Why it
    stores that byte isn't known.

    COUT, COUT1, and CSW come from <apple2/monitor_io.h>, which also
    keeps the runtime's zero page off the Monitor's. It declares COUT1
    `preserves_x`, so dump_buffer_to_screen keeps its index in X across
    the call and compiles to the same loop as the original. COUT goes
    wherever CSW points and promises nothing, so dump_buffer_to_printer
    saves X before each call and reloads it after.

    The originals:

    dump_buffer_to_screen:
        LDX  #$00
    .loop:
        CPX  BUFF_END
        BEQ  .done
        LDA  BUFF_AREA,X
        JSR  COUT1
        INX
        JMP  .loop
    .done:
        LDX  #$00
        STX  BUFF_END
        RTS

    printer_card_initialized_flag:
        BYTE 00

    dump_buffer_to_printer:
        PSHW CSW
        MOVW PRINTER_CSW, CSW
        LDX  #$00
        LDA  printer_card_initialized_flag
        BNE  .loop
        INC  printer_card_initialized_flag
        LDA  #$09                ; ctrl-I
        JSR  COUT
        STOB #$91, $0779
        LDA  #$B8                ; '8'
        JSR  COUT
        LDA  #$B0                ; '0'
        JSR  COUT
        LDA  #$CE                ; 'N'
        JSR  COUT
    .loop:
        CPX  BUFF_END
        BEQ  .done
        LDA  BUFF_AREA,X
        JSR  COUT
        INX
        JMP  .loop
    .done:
        MOVW CSW, PRINTER_CSW
        PULW CSW
        RTS

    PSHW and PULW push and pull a 16-bit value on the hardware stack,
    MOVW copies one (source first), and STOB stores a byte.
*/

/* Zork's own variables, which the rest of the interpreter owns. */
extern uint8_t BUFF_END [[c6502::zeropage(0xEB)]];
extern uint16_t PRINTER_CSW [[c6502::zeropage(0xEE)]];
extern uint8_t BUFF_AREA [[c6502::address(0x0200)]][256];

/* Slot 1's scratchpad RAM, a screen hole. */
extern volatile uint8_t SLOT1_SCRATCH [[c6502::address(0x0779)]];

static uint8_t printer_card_initialized_flag;

void dump_buffer_to_screen(void) {
    for (uint8_t x = 0; x != BUFF_END; x++)
        COUT1(BUFF_AREA[x]);
    BUFF_END = 0;
}

void dump_buffer_to_printer(void) {
    uint16_t saved_csw = CSW;
    CSW = PRINTER_CSW;
    if (!printer_card_initialized_flag) {
        printer_card_initialized_flag = 1;
        COUT(0x09);                     /* ctrl-I */
        SLOT1_SCRATCH = 0x91;
        COUT(0xB8);                     /* '8' */
        COUT(0xB0);                     /* '0' */
        COUT(0xCE);                     /* 'N' */
    }
    for (uint8_t x = 0; x != BUFF_END; x++)
        COUT(BUFF_AREA[x]);
    PRINTER_CSW = CSW;
    CSW = saved_csw;
}
