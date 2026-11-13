/*
    Puts the save code where Zork keeps the original, from the IOB at
    $1DAC, in a region that ends where Zork's code after it begins.
    Code, data, and constants all go there, in that order, the way
    Zork's own listing mixes them. dasm stops with a message if they
    don't fit.
*/
MEMORY
{
    save (rwx) : ORIGIN = 0x1DAC, LENGTH = 0x2200 - 0x1DAC
}

SECTIONS
{
    .text : { *(.text .text.*) } > save
    .rodata : { *(.rodata .rodata.*) } > save
    .data : { *(.data .data.*) } > save
    .bss : { *(.bss .bss.*) } > save
}
