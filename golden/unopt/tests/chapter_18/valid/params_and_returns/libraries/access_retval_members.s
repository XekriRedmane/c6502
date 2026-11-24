SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

return_small_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 14 local bytes
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$65
   LDY   #$01
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$02
   LDY   #$07
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDA   #$66
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0D
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$09
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$02
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0F
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

return_nested_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 41 local bytes
   SEC
   LDA   SSP
   SBC   #$2B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2A
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<@3.ret
   LDY   #$01
   STA   (FP),Y
   LDA   #>@3.ret
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@2
   LDA   #$00
   JMP   .lnot_end@3
.lnot_true@2:
   LDA   #$01
.lnot_end@3:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_end@0
.lb_skip@0:
   LDA   #$01
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$09
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   calloc
   LDY   #$11
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDA   #<@3.ret
   INY
   STA   (FP),Y
   LDA   #>@3.ret
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$11
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$0C
   LDY   #$17
   STA   (FP),Y
   LDA   #<@3.ret
   INY
   STA   (FP),Y
   LDA   #>@3.ret
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$17
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$0D
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<@3.ret
   INY
   STA   (FP),Y
   LDA   #>@3.ret
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
.if_end@0:
   LDY   #$2C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   @3.ret
   LDY   #$00
   STA   (DPTR),Y
   LDA   @3.ret+1
   INY
   STA   (DPTR),Y
   LDA   @3.ret+2
   INY
   STA   (DPTR),Y
   LDA   @3.ret+3
   INY
   STA   (DPTR),Y
   LDA   @3.ret+4
   INY
   STA   (DPTR),Y
   LDA   @3.ret+5
   INY
   STA   (DPTR),Y
   LDA   @3.ret+6
   INY
   STA   (DPTR),Y
   LDA   @3.ret+7
   INY
   STA   (DPTR),Y
   LDA   @3.ret+8
   INY
   STA   (DPTR),Y
   LDA   @3.ret+9
   INY
   STA   (DPTR),Y
   LDA   @3.ret+10
   INY
   STA   (DPTR),Y
   LDA   @3.ret+11
   INY
   STA   (DPTR),Y
   LDA   @3.ret+12
   INY
   STA   (DPTR),Y
   LDA   @3.ret+13
   INY
   STA   (DPTR),Y
   LDA   @3.ret+14
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2A
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

@3.ret:
   DC.L  $00000000
   DC.L  $40000000
   DS.B  2
   DC.B  $0A
   DC.L  $0000000B
