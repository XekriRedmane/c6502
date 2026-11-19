/*
    Places the program where the original's code and the tables among
    it were, around the data that the original's memory image keeps in
    place (ali_baba.h).

    The regions are the spans of main.bin that the original's code
    fills and nothing else reads:

      low     $0800-$1FFF  the main loop, combat, the turn, the
                           entities, and most of util. $1F19-$1FFF is
                           the dead ORPHANED_ENCOUNTER_HANDLER, reused.
      page5b  $5B24-$5BFF  the rest of INIT_GAME_STATE and the border
                           animation's code. $5AA9-$5B23 holds page
                           $5A's last variables and INPUT_BUFFER, and
                           $5C00-$5CFF is the scene buffer the original
                           reads its scenes into, over its border code.
      mid     $5D00-$777F  the menus, the events, the AI, the shop, the
                           map, and their tables. $776E-$777F is the
                           dead SET_CURSOR_COMPRESSED, reused.
      stats   $7780-$7D82  DISPLAY_CHAR_STATS, init's code at $78C7, the
                           sound unit, the $7AAC variables, and the
                           menu strings.
      hrcg    $92A5-$9AA4  the HRCG library and its standard font.
      disk    $A441-$A56E  READ_KEYBOARD, the scene loader, and the
                           integrity checks. $A409-$A440 holds tunes
                           that TUNE_TABLE points at, so the region
                           starts after them.
      high    $A56F-$A805  COLD_RESTART at $A56F, where SOFTEV points,
                           then the dead APPLE_ASM_RESIDUAL at
                           $A5BC-$A805, reused. EALDR follows at $A806.

    Each output section takes all of a unit's code and data, so that
    moving a unit moves all of it, except that the small units' data
    goes in page5b, and util.c and map.c put the routines that sat in
    $A441-$A56E in sections of their own (UNITS.md). Data in .bss has no
    separate section and is loaded as zeros with the code.

    The original's code and tables in these spans come to about 16,400
    bytes once its dead and replaced code is left out, and the regions
    above hold 17,700, which leaves almost no room for C that runs
    larger than the original. So the script also uses

      spare   $A900-$B2FF  RUNTIME_DATA_AREA, zeros in the image. No
                           instruction of the original addresses it,
                           and no immediate operand of the original
                           names a page in $A9-$B2, so the original
                           neither reads nor writes it. $A8F2-$A8FF
                           holds 14 nonzero bytes and is left alone.

    low takes main, the turn, combat, and the entities. mid takes
    input, events, the AI, and display, and spare takes map, util,
    text, and everything the script doesn't name (the linker's
    __zp_init and __spill among them, and the runtime library's
    helpers, runtime.o, when a unit calls one). The C units don't come out the
    size of the original's code in each span, so the assignment is by
    what fits. The link fails with a message when a region overflows.

    main is in section .main at $0800, where the loader enters the
    original, and cold_restart in section .cold_restart at $A56F.
*/
MEMORY
{
    low (rwx) : ORIGIN = 0x0800, LENGTH = 0x2000 - 0x0800
    page5b (rwx) : ORIGIN = 0x5B24, LENGTH = 0x5C00 - 0x5B24
    mid (rwx) : ORIGIN = 0x5D00, LENGTH = 0x7780 - 0x5D00
    stats (rwx) : ORIGIN = 0x7780, LENGTH = 0x7D83 - 0x7780
    hrcg (rwx) : ORIGIN = 0x92A5, LENGTH = 0x9AA5 - 0x92A5
    disk (rwx) : ORIGIN = 0xA441, LENGTH = 0xA56F - 0xA441
    high (rwx) : ORIGIN = 0xA56F, LENGTH = 0xA806 - 0xA56F
    spare (rwx) : ORIGIN = 0xA900, LENGTH = 0xB300 - 0xA900
}

SECTIONS
{
    .main 0x0800 : { main.o(.main) } > low
    .cold_restart 0xA56F : { util.o(.cold_restart) } > high

    /* The routines and data that were in $A441-$A56E: util.c's
       verify_integrity, verify_disk_io, compute_checksum, and
       INTEGRITY_DATA, and map.c's load_scene_from_disk. */
    .disk : {
        util.o(.text.integrity .rodata.integrity)
        map.o(.text.scene_disk)
    } > disk

    /* The small units' variables and tables. */
    .page5b : {
        main.o(.rodata .rodata.* .data .data.* .bss .bss.*)
        turn.o(.rodata .rodata.* .data .data.* .bss .bss.*)
        combat.o(.rodata .rodata.* .data .data.* .bss .bss.*)
        entities.o(.rodata .rodata.* .data .data.* .bss .bss.*)
        text.o(.rodata .rodata.* .data .data.* .bss .bss.*)
        display.o(.rodata .rodata.* .data .data.* .bss .bss.*)
        util.o(.rodata .rodata.* .data .data.* .bss .bss.*)
        map.o(.rodata .rodata.* .data .data.* .bss .bss.*)
        sound.o(.rodata .rodata.* .data .data.* .bss .bss.*)
        events.o(.rodata .rodata.* .data .data.* .bss .bss.*)
        ai.o(.rodata .rodata.* .data .data.* .bss .bss.*)
    } > page5b

    .low : {
        main.o(.text .text.*)
        turn.o(.text .text.*)
        combat.o(.text .text.*)
        entities.o(.text .text.*)
    } > low

    .stats : {
        init.o(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
    } > stats

    .hrcg : {
        hrcg.o(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
    } > hrcg

    .high : {
        sound.o(.text .text.*)
    } > high

    .mid : {
        input.o(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
        events.o(.text .text.*)
        ai.o(.text .text.*)
        display.o(.text .text.*)
    } > mid

    .spare : {
        map.o(.text .text.*)
        util.o(.text .text.*)
        text.o(.text .text.*)
        *(.text .text.* .rodata .rodata.* .data .data.* .bss .bss.*)
    } > spare
}
