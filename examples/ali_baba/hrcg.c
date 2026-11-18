/*
    hrcg.c -- the Hi-Res Character Generator, which draws every
    character the game prints on the high-resolution screen.

    The HRCG is Applesoft Toolkit's "HI-RES CHAR GEN VERSION 1.0",
    carried in the game's own image. A character from $20 to $7F,
    after its high bit is dropped, is drawn as an 8-line glyph of the
    current font at the text cursor CH, CV inside the Monitor's text
    window, and the cursor moves right. A control character changes the
    font, the video, the window, or the cursor. The HRCG keeps the text
    page in step with the high-resolution page as well. Each character
    drawn is also stored on the text page at BASL[CH], and a clear, a
    home, or a scroll of the high-resolution window is followed by the
    Monitor's CLREOP, HOME, or SCROLL.

    A font is 96 glyphs of 8 bytes, for the characters $20-$7F. A
    ctrl-A followed by '1'-'5' chooses one of FONT_DATA's five fonts,
    which hold the map's icons, the border's tiles, and the scenes'
    ornate letters, and a ctrl-A followed by '0' chooses the standard
    font, STD_FONT_DATA.

    Departures from the original:

    - The game reaches HRCG_COUT through CSW, the Monitor's output hook,
      from ROM_COUT1. The units call hrcg_cout directly (see
      ali_baba.h), so hrcg_init doesn't set CSW and KSW, and hrcg_cout
      doesn't save and restore A, X, and Y, which no C caller relies on.
    - hrcg_init does what HRCG_INIT_BODY does after INIT_HRCG's two
      patches. HRCG_DISPLAY_PAGE is only its read of TXTPAGE1, and the
      call of DOS's $03EA is gone. HRCG_RESET and HRCG_RESET_WINDOW,
      which only hrcg_init calls, are written into it.
    - HRCG_DISPATCH_TBL, the handlers' addresses that the original
      reaches with the RTS trick, is a switch in hrcg_control and an if
      in hrcg_shifted_control, because c6502 can't call through a
      pointer. The carry that tells a handler it runs after a ctrl-O is
      the choice between the two functions.
    - Only the control characters the game sends are translated. They
      are ctrl-A, B, C, D, F, I, K, L, N, O, P, and V, and ctrl-S and
      ctrl-W after a ctrl-O. In C every other code does nothing. In the
      original, ctrl-E, G, H, J, M, Q, R, T, Y, and Z, the normal ctrl-S
      and ctrl-W, and the shifted variants of the other handlers have
      code of their own, which a coverage run of the original never
      reached and which this file leaves out.
    - HRCG_GETKEY, the keyboard hook that KSW points at, is left out,
      because nothing in the program reads KSW. So are
      HRCG_ORIGINAL_ENTRY and HRCG_PRINT_VERSION, which nothing calls,
      HRCG_CLREOL, which only HRCG_GETKEY and ctrl-E call, and the
      version string.
    - HRCG_VIDEO_MODE, HRCG_DRAW_MODE, and HRCG_PAGE_MASK keep the
      values $20, 0, and 0 that HRCG_RESET gives them, since only the
      handlers left out change them. The HRCG therefore always draws on
      page 1 and stores each glyph byte, and its OR, AND, and XOR modes
      aren't translated. HRCG_INVERT_FLAG's one-shot value $C0, which
      only the normal ctrl-S sets, is left out with them, so the flag is
      the bool hrcg.lowercase.
    - The parameters ctrl-B saves are a struct, which ctrl-B and ctrl-D
      copy whole where the original copies 8 bytes in a loop.
      HRCG_SAVED_PARAMS+8, the flag that a copy is saved, is
      hrcg_saved_valid. HRCG_MODE_FLAGS is hrcg_next, which hrcg_cout
      sets to HRCG_NEXT_PLAIN before it handles a character, where the
      original shifts it left twice.
    - HRCG_ROW_TABLE, twelve bytes indexed by row / 2 and halved with a
      ROR, is HRCG_ROW_BASE, three bytes indexed by row / 8. The
      original reads HRCG_TRAILING_DATA for rows past 23, which never
      occur.
    - STD_FONT_DATA is a const array in this file, with the 768 bytes
      the image holds at $97A5. Nothing else in the program computes
      its address.

    Ali Baba, main.asm lines 15801-15881 and 15967-16617 (lines
    15882-15966, HRCG_GETKEY, are left out here):

    ; High Resolution Character Generator
        ORG     $92A5
    HRCG_ORIGINAL_ENTRY:
        JMP     HRCG_PRINT_VERSION     ; dead: original entry before patch
        ORG     $92A8
    HRCG_INIT:
    ; --- HRCG entry jump table ---
        JMP     HRCG_INIT_BODY ; main init entry
    HRCG_NUM_CHARSETS:
        DC.B    $0A            ; 10 custom charsets
    HRCG_FONT_BASE:
        DC.W    FONT_DATA      ; font base address ($83A5)
    HRCG_ENTRY2:
        JMP     HRCG_RTS       ; secondary entry (disabled)
    HRCG_ENTRY3:
        JMP     HRCG_RTS       ; secondary entry (disabled)
    ; --- HRCG_PRINT_VERSION ---
    HRCG_PRINT_VERSION:
        SUBROUTINE HRCG_PRINT_VERSION
        JSR     HRCG_INIT_BODY
        LDY     #$00
    .loop:
        LDA     HRCG_VERSION_STR,Y
        JSR     ROM_COUT1
        INY
        CPY     #$1D           ; 29 chars
        BCC     .loop
        RTS
    ; --- HRCG_INIT_BODY ---
    HRCG_INIT_BODY:
        SUBROUTINE HRCG_INIT_BODY
        STOW    HRCG_GETKEY,$38 ; KSW low
        STOW    HRCG_COUT,$36 ; CSW low
        JSR     $03EA          ; reconnect DOS (patched to NOPs by game)
        BIT     $C052          ; full-screen mode (no mixed)
        BIT     $C057          ; hi-res mode
        JSR     HRCG_RESET
        JSR     HRCG_DISPLAY_PAGE
        BIT     $C050          ; graphics mode on
        RTS
        ORG     $92E8
    HRCG_VERSION_STR:
    ; "HI-RES CHAR GEN VERSION 1.0" with ctrl-P prefix and CR suffix (high-bit ASCII)
        HEX     90 C8 C9 AD D2 C5 D3 A0 C3 C8 C1 D2 A0 C7 C5 CE
        HEX     A0 D6 C5 D2 D3 C9 CF CE A0 B1 AE B0 8D
        ORG     $9305
    HRCG_SHIFT_FLAG:
        DC.B    $00                             ; $00=normal, $40=next-shifted, $80=charset-select
        ORG     $9306
    HRCG_MODE_FLAGS:
        DC.B    $00                             ; bit 7/6 shifted by ASL during dispatch
        ORG     $9307
    HRCG_VIDEO_MODE:
        DC.B    $20                             ; $20=replace, $40=OR, $80=AND, $C0=XOR
        ORG     $9308
    HRCG_MASK:
        DC.B    $00                             ; $00=normal, $7F=inverse
        ORG     $9309
    HRCG_DRAW_MODE:
        DC.B    $00                             ; video mode for drawing ($00/$80/$C0)
    HRCG_PAGE_MASK:
        DC.B    $00                             ; $00=page1, $60=page2
        ORG     $930B
    HRCG_WRAP_FLAG:
        DC.B    $00                             ; $00=scroll, $FF=wrap
        ORG     $930C
    HRCG_INVERT_FLAG:
        DC.B    $00                             ; $00=normal, $80=lowercase, $C0=XOR-invert
        ORG     $930D
    HRCG_FONT_PTR_LO:
        DC.B    $A5                             ; font data pointer low
    HRCG_FONT_PTR_HI:
        DC.B    $97                             ; font data pointer high ($97A5 = standard font)
        ORG     $930F
    HRCG_SAVED_PARAMS:
        DC.B    $20,$00,$00,$00,$00,$00,$A5,$97 ; saved copy of video mode..font ptr (8 bytes)
        DC.B    $FF                             ; HRCG_SAVED_FLAG ($00=not saved, $FF=saved)
    HRCG_SAVED_CH:
        DC.B    $01                             ; saved cursor X
    HRCG_SAVED_CV:
        DC.B    $17                             ; saved cursor Y (23)

    ...

        ORG     $93BE
    HRCG_COUT:
        SUBROUTINE HRCG_COUT
    ; --- CSW handler: save regs, process char, restore ---
        STA     $FF                   ; save A
        STX     $EB                   ; save X
        STY     $35                   ; save Y
        JSR     HRCG_PROCESS_CHAR
        LDY     $35                   ; restore Y
        LDX     $EB                   ; restore X
        LDA     $FF                   ; restore A
        RTS
    ; --- HRCG_PROCESS_CHAR ---
    HRCG_PROCESS_CHAR:
        AND     #$7F                  ; strip high bit
        ASL     HRCG_MODE_FLAGS       ; shift mode bit 7 into carry
        BCS     HRCG_CHARSET_SELECT   ; if set -> charset select
        BALT    #$20,_HRCG_COUT.ctrl_dispatch ; printable char (>= space)?
        JMP     HRCG_DRAW_CHAR
    _HRCG_COUT.ctrl_dispatch:
        BAGE    #$1B,_HRCG_COUT.shift_and_rts ; >= ESC ($1B)?
        ASL                           ; char * 2 for table index
        TAX
        LDA     HRCG_DISPATCH_TBL+1,X ; high byte of handler-1
        PHA
        LDA     HRCG_DISPATCH_TBL,X   ; low byte of handler-1
        PHA
    _HRCG_COUT.shift_and_rts:
        ASL     HRCG_MODE_FLAGS       ; shift mode bit 6 into carry
        RTS                           ; RTS-trick dispatch
        ORG     $93EE
    HRCG_DISPATCH_TBL:
    ; Control character dispatch table (address-1 for RTS trick)
    ; ctrl-@: no-op
        DC.W    HRCG_CTRL_NOP-1
    ; ctrl-A: set shift/charset flag
        DC.W    HRCG_CTRL_A-1
    ; ctrl-B: save state
        DC.W    HRCG_CTRL_B_SAVE-1
    ; ctrl-C: restore cursor / XOR mode
        DC.W    HRCG_CTRL_C-1
    ; ctrl-D: restore state / display page
        DC.W    HRCG_CTRL_D-1
    ; ctrl-E: clear EOL
        DC.W    HRCG_CTRL_E-1
    ; ctrl-F: clear to end of screen
        DC.W    HRCG_CTRL_F-1
    ; ctrl-G: bell
        DC.W    $FBDC
    ; ctrl-H: backspace
        DC.W    HRCG_CTRL_H-1
    ; ctrl-I: inverse video
        DC.W    HRCG_CTRL_I-1
    ; ctrl-J: line feed
        DC.W    HRCG_LINE_FEED-1
    ; ctrl-K: clear invert flag
        DC.W    HRCG_CTRL_K-1
    ; ctrl-L: set invert flag
        DC.W    HRCG_CTRL_L-1
    ; ctrl-M: carriage return
        DC.W    HRCG_CTRL_M-1
    ; ctrl-N: normal video
        DC.W    HRCG_CTRL_N-1
    ; ctrl-O: next-shifted / OR mode
        DC.W    HRCG_CTRL_O-1
    ; ctrl-P: home / clear draw mode
        DC.W    HRCG_CTRL_P-1
    ; ctrl-Q: goto saved cursor
        DC.W    HRCG_CTRL_Q-1
    ; ctrl-R: XOR draw mode
        DC.W    HRCG_CTRL_R-1
    ; ctrl-S: XOR invert / clear wrap
        DC.W    HRCG_CTRL_S-1
    ; ctrl-T: AND draw mode
        DC.W    HRCG_CTRL_T-1
    ; ctrl-U: no-op
        DC.W    HRCG_CTRL_NOP-1
    ; ctrl-V: set text window
        DC.W    HRCG_CTRL_V-1
    ; ctrl-W: set margins
        DC.W    HRCG_CTRL_W-1
    ; ctrl-X: no-op
        DC.W    HRCG_CTRL_NOP-1
    ; ctrl-Y: clear screen
        DC.W    HRCG_CTRL_Y-1
    ; ctrl-Z: reset
        DC.W    HRCG_CTRL_Z-1
        ORG     $9424
    HRCG_CTRL_A:
        SUBROUTINE HRCG_CTRL_A
        ;
        ; Ctrl-A handler: charset selector.
        ;
        ; Normal mode (carry clear):
        ;   Sets HRCG_MODE_FLAGS bit 7 so that the NEXT character output
        ;   will be routed to HRCG_CHARSET_SELECT instead of being printed.
        ;   The next byte is interpreted as a charset number ('1'-'9').
        ;
        ; Shifted mode (carry set, from ctrl-A ctrl-A):
        ;   Resets the video mode to replace ($20).
        ;
    ; --- Normal: arm charset-select for next character ---
        BCS     HRCG_SET_REPLACE  ; shifted -> set replace mode
        STOB    #$80,HRCG_MODE_FLAGS ; set bit 7: next char -> charset select
        RTS
        ;
        ; --- Charset select (entered when HRCG_MODE_FLAGS bit 7 was set) ---
        ;
        ; Input:  A = charset selector byte ('1'-'9' = $B1-$B9 with high
        ;             bit already stripped by HRCG_PROCESS_CHAR)
        ;
        ; Charset memory layout:
        ;   Each charset occupies 3 pages (768 bytes = 96 glyphs * 8 bytes).
        ;   Charset 1 starts at HRCG_FONT_BASE ($83A5).
        ;   Charset N starts at HRCG_FONT_BASE + (N-1) * $300.
        ;
        ;   Charset  Selector  Address  Chars   Contents
        ;   -------  --------  -------  -----   --------
        ;      1       $B1     $83A5    0-23    Game map icons
        ;      2       $B2     $86A5    24-47   Border tiles
        ;      3       $B3     $89A5    48-71   Scene text (ornate font)
        ;      4       $B4     $8CA5    72-95   Scene text cont.
        ;      5       $B5     $8FA5    96-119  Scene text cont.
        ;    0 or >9   ---     $97A5    ---     Standard 7x8 font
        ;
        ;   Each charset holds 96 glyphs (768 bytes).  Charsets overlap:
        ;   charset N+1 starts 24 characters (768 bytes) after charset N.
        ;   Scene text (group 2) starts at char 48 = charset 3.
        ;
        ; Output: HRCG_FONT_PTR set to the charset's base address.
        ;
    HRCG_CHARSET_SELECT:
        SBC     #$31              ; A = selector - '1' (carry set from caller)
                                  ; charset '1' -> 0, '2' -> 1, ... '9' -> 8
        BAGE    #$09,HRCG_SET_STD_FONT ; >= 9 -> invalid, use standard font
        ;
        ; Compute font address: HRCG_FONT_BASE + index * 3 pages
        ;   index * 3 = index * 2 + index (via ASL + ADC)
        ;   Result is the HIGH BYTE of the font address.
        ;   Low byte is always HRCG_FONT_BASE low ($A5).
        ;
        LDX     HRCG_FONT_BASE    ; X = font base low ($A5)
        STA     $EE               ; save index
        ASL                       ; A = index * 2
        ADC     $EE               ; A = index * 3 (pages)
        ADC     HRCG_FONT_BASE+1  ; A = $83 + index*3 = font high byte
        BNE     .store_font       ; always taken (result never zero)
        ;
        ; Standard font fallback (charset 0 or invalid selector)
        ;
    HRCG_SET_STD_FONT:
        LDX     #<STD_FONT_DATA   ; X = $A5 (standard font low)
        LDA     #>STD_FONT_DATA   ; A = $97 (standard font high -> $97A5)
    .store_font:
        STX     HRCG_FONT_PTR_LO  ; update font pointer low byte
        STA     HRCG_FONT_PTR_HI  ; update font pointer high byte
        RTS
        ORG     $944A
    HRCG_SET_REPLACE:
    ; --- ctrl-A shifted: set replace video mode ---
        STOB    #$20,HRCG_VIDEO_MODE ; video mode = replace ($20)
        RTS
    ; --- ctrl-B: save params + cursor ---
    HRCG_CTRL_B_SAVE:
        SUBROUTINE HRCG_CTRL_B_SAVE
        BCS     .set_or             ; double-shifted -> set OR mode
        BIT     HRCG_SAVED_PARAMS+8 ; already saved?
        BMI     .save_cursor        ; yes -> just update cursor
        STOB    #$FF,HRCG_SAVED_PARAMS+8 ; mark as saved
        LDY     #$07                ; 8 bytes to save
    .save_loop:
        LDA     HRCG_VIDEO_MODE,Y   ; active params
        STA     HRCG_SAVED_PARAMS,Y ; saved copy
        DEY
        BPL     .save_loop
    .save_cursor:
        MOVB    $24,HRCG_SAVED_CH ; cursor X (CH)
        MOVB    $25,HRCG_SAVED_CV ; cursor Y (CV)
        RTS
    ; --- ctrl-B double-shifted: OR video mode ---
    .set_or:
        STOB    #$40,HRCG_VIDEO_MODE ; video mode = OR ($40)
        RTS
        ORG     $9478
    HRCG_CTRL_C:
        SUBROUTINE HRCG_CTRL_C
    ; --- ctrl-C normal: restore cursor position ---
        BCS     .set_xor_draw  ; shifted -> XOR draw mode
        LDA     HRCG_SAVED_CH  ; saved CH
        CMP     $24            ; compare to current CH
        STA     $24            ; restore CH
        BEQ     .do_lf         ; same -> just do line feed
        BCS     HRCG_CTRL_NOP  ; saved > current -> RTS
    .do_lf:
        JMP     HRCG_LINE_FEED
    ; --- ctrl-C shifted: XOR draw mode ---
    .set_xor_draw:
        STOB    #$C0,HRCG_DRAW_MODE ; draw mode = XOR ($C0)
        STOB    #$00,HRCG_PAGE_MASK ; page mask = 0
        RTS
        ORG     $9493
    HRCG_CTRL_D:
        SUBROUTINE HRCG_CTRL_D
    ; --- ctrl-D normal: restore saved params ---
        BCS     HRCG_DISPLAY_PAGE   ; shifted -> display page
        BIT     HRCG_SAVED_PARAMS+8 ; params saved?
        BPL     HRCG_CTRL_NOP       ; no -> NOP return
        LDY     #$07                ; 8 bytes
    .restore_loop:
        LDA     HRCG_SAVED_PARAMS,Y ; saved params
        STA     HRCG_VIDEO_MODE,Y   ; restore to active
        DEY
        BPL     .restore_loop
        STOB    #$00,HRCG_SAVED_PARAMS+8 ; clear saved flag
        STA     HRCG_SAVED_CH       ; clear saved CH
        MOVB    $22,HRCG_SAVED_CV ; WNDTOP
        RTS
    ; --- HRCG_DISPLAY_PAGE: page flip based on video mode ---
    HRCG_DISPLAY_PAGE:
        BIT     $C054               ; select page 1
        LDA     HRCG_VIDEO_MODE     ; video mode
        BANE    #$40,HRCG_CTRL_NOP  ; no -> return (stay on page 1)
        BIT     $C055               ; yes -> select page 2
        RTS
        ORG     $94C1
    HRCG_CTRL_E:
    ; --- ctrl-E normal: clear to end of line ---
        BCS     HRCG_CTRL_NOP ; shifted -> NOP
        JSR     HRCG_CLREOL
        JMP     $FC9C         ; ROM CLREOLZ
    ; --- ctrl-F normal: clear to end of screen ---
    HRCG_CTRL_F:
        BCS     HRCG_CTRL_NOP ; shifted -> NOP
        JSR     HRCG_CLRLINE
        JMP     $FC42         ; ROM CLREOP
    ; --- ctrl-H: backspace ---
    HRCG_CTRL_H:
        SUBROUTINE HRCG_CTRL_H
        DEC     $24           ; CH--
        BPL     HRCG_CTRL_NOP ; still >= 0 -> done
        MOVB    $21,$24 ; WNDWDTH
        DEC     $24           ; CH = WNDWDTH - 1
        LDA     $22           ; WNDTOP
        BAGE    $25,HRCG_CTRL_NOP ; CV >= WNDTOP?
        DEC     $25           ; CV--
        JMP     $FC22         ; ROM VTAB
    ; --- ctrl-I: inverse video ---
    HRCG_CTRL_I:
        BCS     HRCG_CTRL_NOP ; shifted -> NOP
        STOB    #$7F,HRCG_MASK ; mask = $7F (invert all 7 pixels)
    HRCG_CTRL_NOP:
        RTS                   ; shared NOP return
        ORG     $94EE
    HRCG_CTRL_K:
    ; --- ctrl-K: clear invert flag ---
        BCS     HRCG_CTRL_NOP    ; shifted -> NOP
        STOB    #$00,HRCG_INVERT_FLAG ; clear invert flag
        RTS
    ; --- ctrl-L: set invert flag ---
    HRCG_CTRL_L:
        BCS     HRCG_CTRL_NOP    ; shifted -> NOP
        STOB    #$80,HRCG_INVERT_FLAG ; invert flag = $80
        RTS
    ; --- ctrl-M: carriage return (with pause check) ---
    HRCG_CTRL_M:
        SUBROUTINE HRCG_CTRL_M
        LDY     $C000            ; check keyboard
        BPL     _HRCG_CTRL_M.no_pause        ; no key -> skip pause
        BYNE    #$93,_HRCG_CTRL_M.no_pause   ; ctrl-S (pause)?
        BIT     $C010            ; clear strobe
    .wait_key:
        LDY     $C000            ; wait for key
        BPL     .wait_key
        BYEQ    #$83,_HRCG_CTRL_M.no_pause   ; ctrl-C (cancel)?
        BIT     $C010            ; clear strobe
    _HRCG_CTRL_M.no_pause:
        JMP     HRCG_CR_ENTRY    ; -> set CH=0, line feed
        ORG     $9519
    HRCG_CTRL_N:
    ; --- ctrl-N: normal video ---
        BCS     HRCG_CTRL_NOP   ; shifted -> NOP
        STOB    #$00,HRCG_MASK ; clear mask (normal video)
        RTS
    ; --- ctrl-O: set next-shifted / OR mode ---
    HRCG_CTRL_O:
        SUBROUTINE HRCG_CTRL_O
        BCS     .set_or_draw    ; shifted -> OR draw mode
        STOB    #$40,HRCG_MODE_FLAGS ; next ctrl -> shifted variant
        RTS
    .set_or_draw:
        STOB    #$80,HRCG_DRAW_MODE ; draw mode = OR ($80)
        STOB    #$00,HRCG_PAGE_MASK ; page mask = 0
        RTS
    ; --- ctrl-P: home cursor ---
    HRCG_CTRL_P:
        SUBROUTINE HRCG_CTRL_P
        BCS     .clear_draw     ; shifted -> clear draw mode
        JSR     HRCG_HOME
        JMP     $FC58           ; ROM HOME (clear text screen)
    .clear_draw:
        STOB    #$00,HRCG_DRAW_MODE ; draw mode = 0
        STA     HRCG_PAGE_MASK  ; page mask = 0
        RTS
    ; --- ctrl-Q: goto saved cursor ---
    HRCG_CTRL_Q:
        BCS     HRCG_CTRL_NOP   ; shifted -> NOP
        MOVB    HRCG_SAVED_CH,$24 ; saved CH
        MOVB    HRCG_SAVED_CV,$25 ; saved CV
        JMP     $FC24           ; ROM VTAB
        ORG     $9554
    HRCG_CTRL_R:
    ; --- ctrl-R shifted: XOR draw + page2 ---
        BCC     HRCG_RTS         ; normal -> RTS (no-op)
        STOB    #$C0,HRCG_DRAW_MODE ; draw mode = XOR ($C0)
        STOB    #$60,HRCG_PAGE_MASK ; page mask = $60 (page 2)
        RTS
    ; --- ctrl-S: XOR invert / clear wrap ---
    HRCG_CTRL_S:
        SUBROUTINE HRCG_CTRL_S
        BCS     .clear_wrap      ; shifted -> clear wrap
        STOB    #$C0,HRCG_INVERT_FLAG ; invert = XOR ($C0)
        RTS
    .clear_wrap:
        STOB    #$00,HRCG_WRAP_FLAG ; wrap = 0 (scroll mode)
        RTS
    ; --- ctrl-T shifted: AND draw mode ---
    HRCG_CTRL_T:
        BCC     HRCG_RTS         ; normal -> RTS (no-op)
        STOB    #$80,HRCG_DRAW_MODE ; draw mode = AND ($80)
        STOB    #$60,HRCG_PAGE_MASK ; page mask = $60
        RTS
        ORG     $957C
    HRCG_CTRL_V:
        SUBROUTINE HRCG_CTRL_V
    ; --- ctrl-V: set text window ---
        BCS     HRCG_RTS            ; shifted -> RTS
        LDA     $24                 ; CH
        ADC     $20                 ; + WNDLFT
        BALT    #$28,_HRCG_CTRL_V.store_lft ; >= 40?
        LDA     #$27                ; clamp to 39
    _HRCG_CTRL_V.store_lft:
        STA     $20                 ; WNDLFT = CH + old WNDLFT
        SEC
        LDA     $21                 ; WNDWDTH
        SBC     $24                 ; - CH
        STA     $21                 ; new WNDWDTH
        STOB    #$00,$24 ; CH = 0
        LDA     $25                 ; CV
        BALT    #$18,_HRCG_CTRL_V.store_top ; >= 24?
        LDA     #$17                ; clamp to 23
    _HRCG_CTRL_V.store_top:
        STA     $22                 ; WNDTOP = CV
        BIT     HRCG_SAVED_PARAMS+8 ; params saved?
        BMI     .do_vtab            ; yes -> skip saving CV
        STA     HRCG_SAVED_CV       ; saved CV = WNDTOP
    .do_vtab:
        JMP     $FC22               ; ROM VTAB
    ; --- ctrl-W: set right margin + bottom ---
    HRCG_CTRL_W:
        SUBROUTINE HRCG_CTRL_W
        BCS     .set_wrap           ; shifted -> set wrap
        MOVB    $24,$21 ; CH
        ADC     $20                 ; + WNDLFT
        BALT    #$28,_HRCG_CTRL_W.width_ok ; >= 40?
        LDA     #$27                ; clamp to 39
        SBC     $20                 ; - WNDLFT
        STA     $21                 ; WNDWDTH = 39 - WNDLFT
    _HRCG_CTRL_W.width_ok:
        INC     $21                 ; WNDWDTH + 1
        LDA     $25                 ; CV
        BALT    #$18,_HRCG_CTRL_W.bottom_ok ; >= 24?
        LDA     #$17                ; clamp to 23
    _HRCG_CTRL_W.bottom_ok:
        STA     $23                 ; WNDBTM = CV
        INC     $23                 ; WNDBTM + 1
    HRCG_RTS:
        RTS                         ; shared RTS
    ; --- ctrl-W shifted: set wrap mode ---
    .set_wrap:
        STOB    #$FF,HRCG_WRAP_FLAG ; wrap flag = $FF (wrap mode)
        RTS
        ORG     $95D1
    HRCG_CTRL_Y:
        SUBROUTINE HRCG_CTRL_Y
    ; --- ctrl-Y normal: clear screen (reset window to full) ---
        BCS     .jump_entry2        ; shifted -> JMP HRCG_ENTRY2
    HRCG_RESET_WINDOW:
    ; --- shared: reset window to full screen ---
        LDA     $20                 ; WNDLFT
        ADC     $24                 ; + CH
        STA     $24                 ; CH = absolute column
        STOB    #$00,$20 ; WNDLFT = 0
        STA     $22                 ; WNDTOP = 0
        BIT     HRCG_SAVED_PARAMS+8 ; params saved?
        BMI     .set_dims           ; yes -> skip
        STA     HRCG_SAVED_CH       ; saved CH = 0
        STA     HRCG_SAVED_CV       ; saved CV = 0
    .set_dims:
        STOB    #$28,$21 ; 40
        STOB    #$18,$23 ; 24
        JMP     $FC22               ; ROM VTAB
    .jump_entry2:
        JMP     HRCG_ENTRY2         ; -> secondary entry (disabled)
    ; --- ctrl-Z: full reset ---
    HRCG_CTRL_Z:
        SUBROUTINE HRCG_CTRL_Z
        BCS     .jump_entry3        ; shifted -> JMP HRCG_ENTRY3
    HRCG_RESET:
        LDA     #$00
        LDY     #$12                ; 19 bytes (indices 0-18)
    .clear_loop:
        STA     HRCG_VIDEO_MODE,Y   ; clear param block
        DEY
        BPL     .clear_loop
        JSR     HRCG_RESET_WINDOW   ; reset window
        JSR     HRCG_SET_STD_FONT   ; set standard font ($97A5)
        JSR     HRCG_SET_REPLACE    ; set replace video mode ($20)
        STOB    #<HRCG_RTS,HRCG_ENTRY2+1 ; patch entry2 low
        STA     HRCG_ENTRY3+1       ; patch entry3 low
        STOB    #>HRCG_RTS,HRCG_ENTRY2+2 ; patch entry2 high
        STA     HRCG_ENTRY3+2       ; patch entry3 high
        RTS
    .jump_entry3:
        JMP     HRCG_ENTRY3         ; -> secondary entry (disabled)
        ORG     $9621
    HRCG_DRAW_CHAR:
        SUBROUTINE HRCG_DRAW_CHAR
    ; --- Main character rendering ---
        ASL     HRCG_MODE_FLAGS     ; shift mode flags
        BCS     HRCG_RTS            ; shifted -> RTS (ignore shifted printable)
        BIT     HRCG_INVERT_FLAG    ; check invert flag
        BPL     _HRCG_DRAW_CHAR.normal_char        ; not set -> normal
        BVC     .check_lower        ; bit 6 clear -> check lowercase
        ASL     HRCG_INVERT_FLAG    ; shift invert flag ($C0 -> $80)
        BCS     _HRCG_DRAW_CHAR.normal_char        ; was $C0, now $80 -> treat as normal
    .check_lower:
        BALT    #$41,_HRCG_DRAW_CHAR.normal_char ; 'A' ?
        BALT    #$5B,_HRCG_DRAW_CHAR.calc_offset ; '[' ?
    _HRCG_DRAW_CHAR.normal_char:
        SEC
        SBC     #$20                ; convert to font index (char - $20)
    _HRCG_DRAW_CHAR.calc_offset:
        ASL                         ; \ index * 8 = byte offset in font
        ROL     $EF                 ;  |
        ASL                         ;  |
        ROL     $EF                 ;  |
        ASL                         ;  |
        ROL     $EF                 ; /
        CLC
        ADC     HRCG_FONT_PTR_LO    ; + font pointer low
        STA     $EE                 ; font address low
        LDA     $EF
        AND     #$07                ; mask to 3 bits
        ADC     HRCG_FONT_PTR_HI    ; + font pointer high
        STA     $EF                 ; font address high
        JSR     HRCG_CALC_HIRES_ADDR
        ADC     $24                 ; + cursor X
        STA     $2A                 ; screen address low
        STA     $EC                 ; save copy
        LDA     $2B                 ; screen address high
        EOR     HRCG_PAGE_MASK      ; XOR with page mask (0 or $60)
        STA     $ED                 ; alt-page address high
        LDY     $24                 ; cursor X
        LDA     $FF                 ; original character
        STA     ($28),Y             ; store char in text page
        LDX     #$00
        LDY     #$00
    ; --- Render loop: 8 scan lines ---
    .render_loop:
        LDA     ($EE),Y             ; load font byte
        EOR     HRCG_MASK           ; apply mask (inverse)
        BIT     HRCG_DRAW_MODE      ; check draw mode
        BPL     .store              ; $00 -> replace mode
        BVS     .xor_mode           ; $C0 -> XOR mode
        BIT     HRCG_MASK           ; check mask for AND vs OR
        BVS     .and_mode           ; mask $7F + draw $80 -> AND
        ORA     ($EC,X)             ; OR mode: combine with screen
        BVC     .store              ; always -> store
    .and_mode:
        AND     ($EC,X)             ; AND mode: mask with screen
        BVS     .store              ; always -> store
    .xor_mode:
        EOR     ($EC,X)             ; XOR mode: toggle screen bits
    .store:
        STA     ($2A,X)             ; write to screen
        INY                         ; next font byte
        CPY     #$08                ; done 8 bytes?
        BCS     HRCG_ADVANCE_CURSOR ; yes -> advance cursor
        LDA     $2B                 ; screen high byte
        ADC     #$04                ; next scan line group (+$0400)
        STA     $2B
        EOR     HRCG_PAGE_MASK      ; update alt-page address
        STA     $ED
        BCC     .render_loop        ; loop (always, carry clear)
        ORG     $969E
    HRCG_ADVANCE_CURSOR:
        SUBROUTINE HRCG_ADVANCE_CURSOR
    ; --- Advance cursor after character ---
        INC     $24            ; CH++
        LDA     $24
        BALT    $21,_HRCG_ADVANCE_CURSOR.done ; >= WNDWDTH?
    HRCG_CR_ENTRY:
        STOB    #$00,$24 ; (CR entry point: set CH=0)
    ; --- HRCG_LINE_FEED ---
    HRCG_LINE_FEED:
        INC     $25            ; CV++
        LDA     $25
        BALT    $23,_HRCG_ADVANCE_CURSOR.update_cursor ; >= WNDBTM?
        BIT     HRCG_WRAP_FLAG ; check wrap flag
        BVS     .wrap          ; wrap mode -> wrap to top
        DEC     $25            ; CV-- (stay at bottom)
        JSR     HRCG_SCROLL_UP
        JMP     $FC70          ; ROM SCROLL
    .wrap:
        MOVB    $22,$25 ; WNDTOP
    _HRCG_ADVANCE_CURSOR.update_cursor:
        JSR     $FC24          ; ROM VTAB
    _HRCG_ADVANCE_CURSOR.done:
        RTS
        ORG     $96C7
    HRCG_SCROLL_UP:
        SUBROUTINE HRCG_SCROLL_UP
        LDA     $22                 ; WNDTOP (top row)
        PHA                         ; save for later
    .next_row:
        JSR     HRCG_CALC_HIRES_ROW ; calc hi-res addr for row A
    .copy_row:
        MOVB    $2A,$EC ; dest address low
        LDA     $2B                 ; dest address high
        AND     #$E3                ; mask off scan line bits
        STA     $ED
        PLA                         ; restore source row
        CLC
        ADC     #$01                ; source = dest + 1
        BAGE    $23,_HRCG_SCROLL_UP.clear_last ; >= WNDBTM?
        PHA
        JSR     HRCG_CALC_HIRES_ROW ; calc hi-res addr for source row
        LDX     #$07                ; 8 scan lines
    .copy_scanline:
        LDY     $21                 ; WNDWDTH
        DEY                         ; index = WNDWDTH - 1
    .copy_byte:
        LDA     ($2A),Y             ; read source
        STA     ($EC),Y             ; write dest
        DEY
        BPL     .copy_byte          ; next column
        DEX
        BMI     .copy_row           ; all 8 lines done -> next row
        CLC
        LDA     $2B                 ; advance source
        ADC     #$04
        STA     $2B
        LDA     $ED                 ; advance dest
        ADC     #$04
        STA     $ED
        BNE     .copy_scanline      ; always -> continue
    _HRCG_SCROLL_UP.clear_last:
        LDA     HRCG_DRAW_MODE      ; save draw mode
        PHA
        LDA     $23                 ; WNDBTM
        SBC     #$01                ; last row index
        LDY     #$00
        STY     HRCG_DRAW_MODE      ; draw mode = 0 (replace)
        JSR     HRCG_FILL_LINE      ; clear last row
        PULB    HRCG_DRAW_MODE ; restore draw mode
        RTS
        ORG     $9716
    HRCG_HOME:
        SUBROUTINE HRCG_HOME
    ; --- Set cursor to top-left and clear screen ---
        MOVB    $22,$25 ; WNDTOP
        LDY     #$00
        STY     $24                 ; CH = 0
        BEQ     .clear_entry        ; always -> fall into clear loop
    ; --- HRCG_CLRLINE: clear from current row to bottom ---
    HRCG_CLRLINE:
        LDY     $24                 ; preserve CH for fill start column
        LDA     $25                 ; CV = starting row
    .clear_entry:
        PHA                         ; save row
        JSR     HRCG_FILL_LINE      ; clear this row
        PLA
        CLC
        ADC     #$01                ; next row
        CMP     $23                 ; >= WNDBTM?
        LDY     #$00                ; fill from column 0 for next rows
        BCC     .clear_entry        ; continue loop
        RTS
    ; --- HRCG_CLREOL: clear from cursor to end of line ---
    HRCG_CLREOL:
        LDY     $24                 ; start column = CH
        LDA     $25                 ; current row = CV
    ; --- HRCG_FILL_LINE: fill one row ---
    HRCG_FILL_LINE:
        STY     $EE                 ; save start column
        JSR     HRCG_CALC_HIRES_ROW ; calc hi-res addr for row A
        LDX     #$07                ; 8 scan lines
    .fill_byte:
        LDA     HRCG_MASK           ; fill byte (mask value)
        BIT     HRCG_DRAW_MODE      ; check draw mode
        BPL     .write              ; replace mode -> write directly
        MOVB    $2A,$EC ; compute alt-page address
        LDA     $2B
        EOR     HRCG_PAGE_MASK
        STA     $ED
        LDA     ($EC),Y             ; read existing screen byte
        BIT     HRCG_DRAW_MODE      ; check draw mode again
        BVC     .write              ; OR mode -> skip XOR
        EOR     HRCG_MASK           ; XOR with mask
    .write:
        STA     ($2A),Y             ; write to screen
        INY                         ; next column
        CPY     $21                 ; >= WNDWDTH?
        BCC     .fill_byte          ; no -> continue
        DEX                         ; next scan line
        BMI     .fill_done          ; all 8 done -> return
        LDY     $EE                 ; reset column to start
        LDA     $2B                 ; advance to next scan line
        CLC
        ADC     #$04
        STA     $2B
        BNE     .fill_byte          ; always -> continue
    .fill_done:
        RTS
        ORG     $9771
    HRCG_CALC_HIRES_ADDR:
    ; --- Compute hi-res screen address from CV ---
    ; Input: ZP $25 (CV)   Output: $2A/$2B
        LDA     $25              ; CV (cursor Y)
    HRCG_CALC_HIRES_ROW:
    ; --- Alternate entry: row in A ---
        LSR                      ; A = row / 2
        TAX                      ; X = row / 2 (table index)
        AND     #$03             ; A = (row/2) % 4
        ORA     HRCG_VIDEO_MODE  ; combine with page base ($20/$40)
        STA     $2B              ; screen high byte
        LDA     HRCG_ROW_TABLE,X ; row base from table
        ROR                      ; rotate carry from LSR into bit 7
        ADC     $20              ; + WNDLFT
        STA     $2A              ; screen low byte
        RTS
    ; --- Row address table (12 entries) ---
    HRCG_ROW_TABLE:
        HEX     00 00 00 00 50 50 50 50 A0 A0 A0 A0
    ; --- Trailing data (20 bytes, purpose unknown) ---
    HRCG_TRAILING_DATA:
        HEX     B9 A2 A0 D2 E9 A0 F2 A0 A5 B9 C1 80 C4 CD A0 8C
        HEX     A0 AC A0 A0
        ORG     $97A5
    STD_FONT_DATA:
        INCLUDE "stdfontdata.asm"
*/
#include "ali_baba.h"

/* The high-resolution page the HRCG draws on, the high byte of its
 * address. The original keeps it in HRCG_VIDEO_MODE, which only the
 * shifted ctrl-A and ctrl-B change from $20, and the game sends
 * neither. */
#define HRCG_PAGE 0x20

/* A row of text is 8 scan lines, $400 bytes apart. */
#define HRCG_SCAN_LINE 0x400

/* What the next character is, after a ctrl-A or a ctrl-O. The
 * original's HRCG_MODE_FLAGS holds $80, $40, or 0, and shifts it left
 * twice for every character, so it is always 0 after one. */
enum {
    HRCG_NEXT_PLAIN = 0x00,
    HRCG_NEXT_SHIFTED = 0x40,   /* after ctrl-O: a control character's shifted variant */
    HRCG_NEXT_FONT = 0x80,      /* after ctrl-A: a font selector */
};

/* The parameters ctrl-B saves and ctrl-D restores, the original's
 * $9307-$930E without the three that never change in this game
 * (HRCG_VIDEO_MODE, HRCG_DRAW_MODE, and HRCG_PAGE_MASK). */
struct hrcg_params {
    uint8_t mask;           /* XORed into every glyph byte, and a clear's fill: $00 normal, $7F inverse */
    bool wrap;              /* a line feed at the window's bottom goes to its top when set and scrolls when clear */
    bool lowercase;         /* 'A'-'Z' draw the font's glyphs for 'a'-'z' */
    const uint8_t *font;    /* 96 glyphs of 8 bytes, for characters $20-$7F */
};

static struct hrcg_params hrcg;
static struct hrcg_params hrcg_saved;
static bool hrcg_saved_valid;       /* HRCG_SAVED_PARAMS+8, $FF while hrcg_saved holds a copy */
static uint8_t hrcg_saved_ch;
static uint8_t hrcg_saved_cv;
static uint8_t hrcg_next;           /* HRCG_NEXT_ */

/* The low byte of text row 8n's first scan line, indexed by n. The
 * original's HRCG_ROW_TABLE holds twice these values, indexed by
 * row / 2, and halves them with a ROR. */
static const uint8_t HRCG_ROW_BASE[3] = { 0x00, 0x28, 0x50 };

/* The standard font, which the ctrl-A selector '0' (or any selector
 * outside '1'-'9') chooses. */
static const uint8_t STD_FONT_DATA[0x300] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* $20 */
    0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x08, 0x00,  /* '!' */
    0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00,  /* '"' */
    0x08, 0x1C, 0x3E, 0x7F, 0x1C, 0x1C, 0x1C, 0x1C,  /* '#' */
    0x08, 0x3C, 0x0A, 0x1C, 0x28, 0x1E, 0x08, 0x00,  /* '$' */
    0x06, 0x26, 0x10, 0x08, 0x04, 0x32, 0x30, 0x00,  /* '%' */
    0x04, 0x0A, 0x0A, 0x04, 0x2A, 0x12, 0x2C, 0x00,  /* '&' */
    0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,  /* ''' */
    0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08, 0x00,  /* '(' */
    0x08, 0x10, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00,  /* ')' */
    0x08, 0x2A, 0x1C, 0x08, 0x1C, 0x2A, 0x08, 0x00,  /* $2A */
    0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00,  /* '+' */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x8C, 0x88, 0x84,  /* ',' */
    0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00,  /* '-' */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8C, 0x00,  /* '.' */
    0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00,  /* $2F */
    0x8C, 0x92, 0x22, 0x22, 0x22, 0x92, 0x8C, 0x00,  /* '0' */
    0x08, 0x0C, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00,  /* '1' */
    0x1C, 0x22, 0x20, 0x10, 0x84, 0x82, 0x3E, 0x00,  /* '2' */
    0x3E, 0x90, 0x10, 0x18, 0x20, 0x22, 0x1C, 0x00,  /* '3' */
    0x10, 0x18, 0x14, 0x12, 0x3E, 0x10, 0x10, 0x00,  /* '4' */
    0x3E, 0x02, 0x1E, 0x20, 0x20, 0x22, 0x1C, 0x00,  /* '5' */
    0x18, 0x04, 0x82, 0x1E, 0x22, 0x22, 0x1C, 0x00,  /* '6' */
    0x3E, 0x90, 0x10, 0x88, 0x08, 0x84, 0x04, 0x00,  /* '7' */
    0x1C, 0x22, 0x22, 0x1C, 0x22, 0x22, 0x1C, 0x00,  /* '8' */
    0x1C, 0x22, 0x22, 0x3C, 0x90, 0x10, 0x0C, 0x00,  /* '9' */
    0x00, 0x00, 0x8C, 0x00, 0x00, 0x8C, 0x00, 0x00,  /* ':' */
    0x00, 0x00, 0x8C, 0x00, 0x00, 0x8C, 0x88, 0x84,  /* ';' */
    0x08, 0x0C, 0x7E, 0x7F, 0x7E, 0x0C, 0x08, 0x00,  /* '<' */
    0x00, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x00, 0x00,  /* '=' */
    0x08, 0x18, 0x3F, 0x7F, 0x3F, 0x18, 0x08, 0x00,  /* '>' */
    0x1C, 0x22, 0x90, 0x88, 0x08, 0x00, 0x08, 0x00,  /* '?' */
    0x1C, 0x1C, 0x1C, 0x1C, 0x7F, 0x3E, 0x1C, 0x08,  /* '@' */
    0x08, 0x8C, 0x14, 0x92, 0x3E, 0x22, 0x22, 0x00,  /* 'A' */
    0x1E, 0x22, 0x22, 0x1E, 0x22, 0x22, 0x1E, 0x00,  /* 'B' */
    0x1C, 0x22, 0x02, 0x02, 0x02, 0x22, 0x1C, 0x00,  /* 'C' */
    0x1E, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1E, 0x00,  /* 'D' */
    0x3E, 0x02, 0x02, 0x1E, 0x02, 0x02, 0x3E, 0x00,  /* 'E' */
    0x3E, 0x02, 0x02, 0x1E, 0x02, 0x02, 0x02, 0x00,  /* 'F' */
    0x3C, 0x02, 0x02, 0x02, 0x32, 0x22, 0x3C, 0x00,  /* 'G' */
    0x22, 0x22, 0x22, 0x3E, 0x22, 0x22, 0x22, 0x00,  /* 'H' */
    0x1C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00,  /* 'I' */
    0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x1C, 0x00,  /* 'J' */
    0x22, 0x12, 0x0A, 0x06, 0x0A, 0x12, 0x22, 0x00,  /* 'K' */
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x3E, 0x00,  /* 'L' */
    0x22, 0x36, 0x2A, 0x2A, 0x22, 0x22, 0x22, 0x00,  /* 'M' */
    0x22, 0x22, 0x26, 0x2A, 0x32, 0x22, 0x22, 0x00,  /* 'N' */
    0x1C, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1C, 0x00,  /* 'O' */
    0x1E, 0x22, 0x22, 0x1E, 0x02, 0x02, 0x02, 0x00,  /* 'P' */
    0x1C, 0x22, 0x22, 0x22, 0x2A, 0x12, 0x2C, 0x00,  /* 'Q' */
    0x1E, 0x22, 0x22, 0x1E, 0x0A, 0x12, 0x22, 0x00,  /* 'R' */
    0x1C, 0x22, 0x02, 0x1C, 0x20, 0x22, 0x1C, 0x00,  /* 'S' */
    0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00,  /* 'T' */
    0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1C, 0x00,  /* 'U' */
    0x22, 0x22, 0x22, 0x92, 0x14, 0x8C, 0x08, 0x00,  /* 'V' */
    0x22, 0x22, 0x22, 0x2A, 0x2A, 0x36, 0x22, 0x00,  /* 'W' */
    0x22, 0x92, 0x14, 0x08, 0x14, 0x92, 0x22, 0x00,  /* 'X' */
    0x22, 0x92, 0x14, 0x08, 0x08, 0x08, 0x08, 0x00,  /* 'Y' */
    0x3E, 0x90, 0x10, 0x08, 0x04, 0x82, 0x3E, 0x00,  /* 'Z' */
    0x3E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x3E, 0x00,  /* '[' */
    0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00,  /* $5C */
    0x3E, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3E, 0x00,  /* ']' */
    0x00, 0x00, 0x08, 0x14, 0x22, 0x00, 0x00, 0x00,  /* '^' */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F,  /* '_' */
    0x04, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,  /* '`' */
    0x00, 0x00, 0x1C, 0x20, 0x3C, 0x22, 0x3C, 0x00,  /* 'a' */
    0x02, 0x02, 0x1E, 0x22, 0x22, 0x22, 0x1E, 0x00,  /* 'b' */
    0x00, 0x00, 0x3C, 0x02, 0x02, 0x02, 0x3C, 0x00,  /* 'c' */
    0x20, 0x20, 0x3C, 0x22, 0x22, 0x22, 0x3C, 0x00,  /* 'd' */
    0x00, 0x00, 0x1C, 0x22, 0x3E, 0x02, 0x3C, 0x00,  /* 'e' */
    0x18, 0x24, 0x04, 0x1E, 0x04, 0x04, 0x04, 0x00,  /* 'f' */
    0x00, 0x00, 0x1C, 0x22, 0x22, 0x3C, 0x20, 0x1C,  /* 'g' */
    0x02, 0x02, 0x1E, 0x22, 0x22, 0x22, 0x22, 0x00,  /* 'h' */
    0x08, 0x00, 0x0C, 0x08, 0x08, 0x08, 0x1C, 0x00,  /* 'i' */
    0x10, 0x00, 0x18, 0x10, 0x10, 0x10, 0x12, 0x0C,  /* 'j' */
    0x02, 0x02, 0x22, 0x12, 0x0E, 0x12, 0x22, 0x00,  /* 'k' */
    0x0C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00,  /* 'l' */
    0x00, 0x00, 0x36, 0x2A, 0x2A, 0x2A, 0x22, 0x00,  /* 'm' */
    0x00, 0x00, 0x1E, 0x22, 0x22, 0x22, 0x22, 0x00,  /* 'n' */
    0x00, 0x00, 0x1C, 0x22, 0x22, 0x22, 0x1C, 0x00,  /* 'o' */
    0x00, 0x00, 0x1E, 0x22, 0x22, 0x1E, 0x02, 0x02,  /* 'p' */
    0x00, 0x00, 0x3C, 0x22, 0x22, 0x3C, 0x20, 0x20,  /* 'q' */
    0x00, 0x00, 0x3A, 0x06, 0x02, 0x02, 0x02, 0x00,  /* 'r' */
    0x00, 0x00, 0x3C, 0x02, 0x1C, 0x20, 0x1E, 0x00,  /* 's' */
    0x04, 0x04, 0x1E, 0x04, 0x04, 0x24, 0x18, 0x00,  /* 't' */
    0x00, 0x00, 0x22, 0x22, 0x22, 0x32, 0x2C, 0x00,  /* 'u' */
    0x00, 0x00, 0x22, 0x92, 0x14, 0x8C, 0x08, 0x00,  /* 'v' */
    0x00, 0x00, 0x22, 0x22, 0x2A, 0x2A, 0x36, 0x00,  /* 'w' */
    0x00, 0x00, 0x22, 0x14, 0x08, 0x14, 0x22, 0x00,  /* 'x' */
    0x00, 0x00, 0x22, 0x92, 0x14, 0x8C, 0x08, 0x86,  /* 'y' */
    0x00, 0x00, 0x3E, 0x10, 0x08, 0x04, 0x3E, 0x00,  /* 'z' */
    0x38, 0x0C, 0x0C, 0x06, 0x0C, 0x0C, 0x38, 0x00,  /* '{' */
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  /* '|' */
    0x0E, 0x18, 0x18, 0x30, 0x18, 0x18, 0x0E, 0x00,  /* '}' */
    0x2C, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* '~' */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,  /* $7F */
};

/* Returns the address of the top scan line of text row `row` at column
 * `col` of the window, for rows 0-23. The column is added to the
 * address's low byte without a carry into the high byte, as DRAW_CHAR's
 * ADC does. */
static uint8_t *hrcg_row_address(uint8_t row, uint8_t col) {
    uint8_t hi = HRCG_PAGE | ((row >> 1) & 3);
    uint8_t lo = (uint8_t)(row << 7) | HRCG_ROW_BASE[row >> 3];
    lo += WNDLFT + col;
    return (uint8_t *)((uint16_t)hi << 8 | lo);
}

/* Fills the 8 scan lines of text row `row` with the mask, from column
 * `col` to the window's right edge. At least one byte of each line is
 * written, even when `col` is at or past the edge. */
static void hrcg_fill_line(uint8_t row, uint8_t col) {
    uint8_t *line = hrcg_row_address(row, 0);
    for (uint8_t n = 0; n < 8; n++) {
        uint8_t x = col;
        do {
            line[x] = hrcg.mask;
        } while (++x < WNDWDTH);
        line += HRCG_SCAN_LINE;
    }
}

/* Clears row `row` from column `col`, and every row below it to the
 * window's bottom from column 0. HRCG_HOME and HRCG_CLRLINE. */
static void hrcg_clear_from(uint8_t row, uint8_t col) {
    do {
        hrcg_fill_line(row, col);
        col = 0;
    } while (++row < WNDBTM);
}

/* Moves the window's rows up one on the high-resolution page and
 * clears its bottom row. The text page is scrolled by the Monitor's
 * SCROLL afterwards. */
static void hrcg_scroll_up(void) {
    for (uint8_t row = WNDTOP; (uint8_t)(row + 1) < WNDBTM; row++) {
        uint8_t *dst = hrcg_row_address(row, 0);
        const uint8_t *src = hrcg_row_address(row + 1, 0);
        for (uint8_t n = 0; n < 8; n++) {
            for (uint8_t x = WNDWDTH; x != 0;) {
                x--;
                dst[x] = src[x];
            }
            dst += HRCG_SCAN_LINE;
            src += HRCG_SCAN_LINE;
        }
    }
    hrcg_fill_line(WNDBTM - 1, 0);
}

/* Moves the cursor down a row. At the window's bottom it goes to the
 * top in wrap mode, and otherwise the window scrolls and the cursor
 * stays on the bottom row. */
static void hrcg_line_feed(void) {
    if (++CV < WNDBTM) {
        VTABZ(CV);
    } else if (hrcg.wrap) {
        CV = WNDTOP;
        VTABZ(CV);
    } else {
        CV--;
        hrcg_scroll_up();
        SCROLL();
    }
}

/* Moves the cursor right a column, and to the start of the next row
 * past the window's right edge. */
static void hrcg_advance_cursor(void) {
    if (++CH < WNDWDTH)
        return;
    CH = 0;
    hrcg_line_feed();
}

/* Draws character `c`, $20-$7F, at the cursor and advances the cursor.
 * `raw` is the character as hrcg_cout received it, high bit and all,
 * which goes on the text page at BASL[CH]. BASL is whatever the last
 * VTAB, VTABZ, HOME, CLREOP, or SCROLL left, so that byte can land on
 * a different row from the cursor's when the game has moved CV itself. */
static void hrcg_draw_char(uint8_t c, uint8_t raw) {
    uint8_t glyph = c - 0x20;
    if (hrcg.lowercase && c >= 'A' && c <= 'Z')
        glyph = c;
    const uint8_t *src = hrcg.font + ((uint16_t)glyph << 3);
    uint8_t *dst = hrcg_row_address(CV, CH);
    BASL[CH] = raw;
    for (uint8_t n = 0; n < 8; n++) {
        *dst = src[n] ^ hrcg.mask;
        dst += HRCG_SCAN_LINE;
    }
    hrcg_advance_cursor();
}

/* Chooses the font for selector `c`, the character after a ctrl-A
 * without its high bit. '1'-'9' are FONT_DATA + (c - '1') * $300, so
 * that '6'-'9' read whatever follows FONT_DATA's five fonts, as the
 * original's do. Any other selector is the standard font. */
static void hrcg_select_font(uint8_t c) {
    uint8_t i = c - '1';
    if (i < 9)
        hrcg.font = FONT_DATA[0] + ((uint16_t)(uint8_t)(i * 3) << 8);
    else
        hrcg.font = STD_FONT_DATA;
}

/* Saves the parameters, unless they're saved already, and the cursor.
 * The normal ctrl-B. */
static void hrcg_save(void) {
    if (!hrcg_saved_valid) {
        hrcg_saved_valid = true;
        hrcg_saved = hrcg;
    }
    hrcg_saved_ch = CH;
    hrcg_saved_cv = CV;
}

/* Moves the cursor to the saved column, and to the next row unless that
 * moves it right. The normal ctrl-C. */
static void hrcg_restore_column(void) {
    uint8_t old = CH;
    CH = hrcg_saved_ch;
    if (hrcg_saved_ch <= old)
        hrcg_line_feed();
}

/* Restores the saved parameters, if any, and forgets the saved cursor.
 * The normal ctrl-D. */
static void hrcg_restore(void) {
    if (!hrcg_saved_valid)
        return;
    hrcg = hrcg_saved;
    hrcg_saved_valid = false;
    hrcg_saved_ch = 0;
    hrcg_saved_cv = WNDTOP;
}

/* Moves the window's top left corner to the cursor, which goes to the
 * window's column 0. The normal ctrl-V. The carry into WNDLFT's add is
 * clear, because the dispatch reached here through a BCS not taken. */
static void hrcg_set_top_left(void) {
    uint8_t left = CH + WNDLFT;
    if (left >= 40)
        left = 39;
    WNDLFT = left;
    WNDWDTH -= CH;
    CH = 0;
    uint8_t top = CV;
    if (top >= 24)
        top = 23;
    WNDTOP = top;
    if (!hrcg_saved_valid)
        hrcg_saved_cv = top;
    VTAB();
}

/* Obeys control character `code`, $00-$1A, after a ctrl-O. Only the
 * shifted ctrl-S and ctrl-W, which choose scrolling or wrapping at the
 * bottom of the window, are here, since the game sends no others. */
static void hrcg_shifted_control(uint8_t code) {
    if (code == (HRCG_CTRL_S & 0x7F))
        hrcg.wrap = false;
    else if (code == (HRCG_CTRL_W & 0x7F))
        hrcg.wrap = true;
}

/* Obeys control character `code`, $00-$1A. Only the codes the game
 * sends are here. */
static void hrcg_control(uint8_t code) {
    switch (code) {
    case HRCG_CTRL_A & 0x7F:
        hrcg_next = HRCG_NEXT_FONT;
        break;
    case HRCG_CTRL_B & 0x7F:
        hrcg_save();
        break;
    case HRCG_CTRL_C & 0x7F:
        hrcg_restore_column();
        break;
    case HRCG_CTRL_D & 0x7F:
        hrcg_restore();
        break;
    case HRCG_CTRL_F & 0x7F:
        hrcg_clear_from(CV, CH);
        CLREOP();
        break;
    case HRCG_CTRL_I & 0x7F:
        hrcg.mask = 0x7F;
        break;
    case HRCG_CTRL_K & 0x7F:
        hrcg.lowercase = false;
        break;
    case HRCG_CTRL_L & 0x7F:
        hrcg.lowercase = true;
        break;
    case HRCG_CTRL_N & 0x7F:
        hrcg.mask = 0;
        break;
    case HRCG_CTRL_O & 0x7F:
        hrcg_next = HRCG_NEXT_SHIFTED;
        break;
    case HRCG_CTRL_P & 0x7F:
        CV = WNDTOP;
        CH = 0;
        hrcg_clear_from(WNDTOP, 0);
        HOME();
        break;
    case HRCG_CTRL_V & 0x7F:
        hrcg_set_top_left();
        break;
    }
}

void hrcg_init(void) {
    (void)MIXCLR;
    (void)HIRES;
    /* HRCG_RESET clears the parameters, the saved copy, and the saved
     * cursor. */
    hrcg.mask = 0;
    hrcg.wrap = false;
    hrcg.lowercase = false;
    hrcg_saved_valid = false;
    hrcg_saved_ch = 0;
    hrcg_saved_cv = 0;
    /* HRCG_RESET_WINDOW sets the window to the whole screen and keeps
     * the cursor's place on it. The carry into its add is clear on the
     * one path that reaches it. */
    CH += WNDLFT;
    WNDLFT = 0;
    WNDTOP = 0;
    WNDWDTH = 40;
    WNDBTM = 24;
    VTAB();
    hrcg.font = STD_FONT_DATA;
    /* HRCG_DISPLAY_PAGE, as INIT_HRCG patched it. */
    (void)TXTPAGE1;
    (void)TXTCLR;
}

/* The high bit of `c` is dropped, except on the text page. After a
 * ctrl-A the character selects a font. A printable character after a
 * ctrl-O is dropped, and codes $1B-$1F do nothing, as in the original. */
void hrcg_cout(uint8_t c) {
    uint8_t code = c & 0x7F;
    uint8_t next = hrcg_next;
    hrcg_next = HRCG_NEXT_PLAIN;
    if (next == HRCG_NEXT_FONT) {
        hrcg_select_font(code);
    } else if (code >= 0x20) {
        if (next == HRCG_NEXT_PLAIN)
            hrcg_draw_char(code, c);
    } else if (code < 0x1B) {
        if (next == HRCG_NEXT_SHIFTED)
            hrcg_shifted_control(code);
        else
            hrcg_control(code);
    }
}
