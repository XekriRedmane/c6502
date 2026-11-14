/*
    Puts each unit where Zork keeps the original routines: the page
    cache and the z-string decoder from $17DD, the line buffer from
    $1B90, and the disk routines, after their IOB and DCT, from $1DAC.
    print_string_literal, which Zork keeps with its instruction
    handlers at $0C5B, is in section .text.literal. Each region runs to
    the start of the next, over Zork's routines that this example
    leaves out, and dasm stops with a message if a unit doesn't fit.
    Anything else, such as a test's main, goes after the disk routines.
*/
MEMORY
{
    literal (rx) : ORIGIN = 0x0C5B, LENGTH = 0x17DD - 0x0C5B
    zstring (rx) : ORIGIN = 0x17DD, LENGTH = 0x1B90 - 0x17DD
    output (rwx) : ORIGIN = 0x1B90, LENGTH = 0x1DAC - 0x1B90
    disk (rwx) : ORIGIN = 0x1DAC, LENGTH = 0x2200 - 0x1DAC
}

SECTIONS
{
    .literal : { *(.text.literal) } > literal
    .pages : { pages.asm(.text .text.* .rodata .rodata.*) } > zstring
    .zstring : { zstring.asm(.text .text.* .rodata .rodata.*) } > zstring
    .output : {
        output.asm(.text .text.*)
        output.asm(.rodata .rodata.* .data .data.* .bss .bss.*)
    } > output
    .disk : {
        disk.asm(.data .data.*)
        disk.asm(.text .text.*)
    } > disk
    .text : { *(.text .text.*) } > disk
    .rodata : { *(.rodata .rodata.*) } > disk
    .data : { *(.data .data.*) } > disk
    .bss : { *(.bss .bss.*) } > disk
}
