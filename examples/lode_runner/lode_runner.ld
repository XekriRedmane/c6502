/*
    Places the program where the original's code was, around the data
    that the original's memory image keeps in place (lode_runner.h).

    The original's code fills $5F00-$8E52, but $5F00-$5FFF is the last
    page of high-resolution page 2, which CLEAR_HGR2 fills along with
    the rest of the page. The original keeps only its loader and MAIN
    there, which have run by then, so the program starts at $6000.
    format_patch, in editor.o's section format_patch, is at $8E00,
    where the game's DOS jumps when the editor formats a disk, and the
    rest of $8E00-$8FFF follows it. The original keeps nothing in
    $8E53-$8FFF.

    The program doesn't fit there yet, so the units that .code and
    .high don't name go in .spare, at $9000-$9AFF, which the original
    never reads or writes. That range holds bytes left over in the
    original's disk image and lies just below DEMO_MOVES at $9B00. Its
    `*` patterns take everything not placed earlier, which is
    editor.o and the code of hiscore.o. To tighten the layout as the
    program shrinks, name more of a unit in .code or .high. The link
    fails with a message if a region overflows.

    Each output section takes all of a unit's code and data, so that
    moving a unit moves all of it. Data in .bss has no separate
    section and is loaded as zeros with the code.
*/
MEMORY
{
    code (rwx) : ORIGIN = 0x6000, LENGTH = 0x8E00 - 0x6000
    high (rwx) : ORIGIN = 0x8E00, LENGTH = 0x9000 - 0x8E00
    spare (rwx) : ORIGIN = 0x9000, LENGTH = 0x9B00 - 0x9000
}

SECTIONS
{
    .code : {
        main.o(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
        level.o(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
        player.o(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
        input.o(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
        guards.o(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
        timers.o(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
        screen.o(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
        sprites.o(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
        sound.o(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
        wipe.o(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
    } > code
    .format_patch 0x8E00 : { editor.o(format_patch) } > high
    .high : {
        hiscore.o(.rodata .rodata.* .data .data.* .bss .bss.*)
    } > high
    .spare : {
        *(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
    } > spare
}
