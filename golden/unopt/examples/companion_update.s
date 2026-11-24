SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

compute_screen_x:
   SUBROUTINE

   ; prologue: 3 arg bytes, 48 local bytes
   SEC
   LDA   SSP
   SBC   #$32
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$31
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$34
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   TAX
   LDA   perspective_xoff_hi,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   TAX
   LDA   perspective_xoff_lo,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   ORA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   ORA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   SEC
   LDY   #$17
   SBC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   SBC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   TAX
   LDA   companion_pos_hi,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   TAX
   LDA   companion_pos_lo,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   ORA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2A
   ORA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$13
   SBC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   SBC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$35
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$31
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS

find_active_entity:
   SUBROUTINE

   ; prologue: 3 arg bytes, 27 local bytes
   SEC
   LDA   SSP
   SBC   #$1D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1C
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$03
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$04
   STA   (FP),Y
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   TAX
   LDA   entity_hit_state,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   TXA
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   TAX
   LDA   entity_hit_row,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$08
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$19
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$20
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1C
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@0:
   TXA
   LDY   #$1A
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$20
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1C
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

entity_proximity:
   SUBROUTINE

   ; prologue: 3 arg bytes, 80 local bytes
   SEC
   LDA   SSP
   SBC   #$52
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$51
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   CLC
   LDA   FP
   ADC   #$01
   LDY   #$02
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$55
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   find_active_entity
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@8
   LDA   #$00
   JMP   .lnot_end@9
.lnot_true@8:
   LDA   #$01
.lnot_end@9:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1

   ; epilogue
   CLC
   LDA   FP
   ADC   #$55
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$51
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@1:
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   TAX
   LDA   companion_row,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2

   ; epilogue
   CLC
   LDA   FP
   ADC   #$55
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$51
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@2:
   LDY   #$54
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$40
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@3
   LDY   #$54
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$47
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   ASL
   LDA   #$00
   ROL
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@3
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@4:
   LDY   #$19
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .if_end@5
.lb_skip@3:
   LDA   #$FF
   LDY   #$1B
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   TAX
   LDY   #$1B
   LDA   (FP),Y
   STA   companion_state,X
   LDY   #$53
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   TAX
   LDA   companion_row,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   TAX
   LDY   #$27
   LDA   (FP),Y
   STA   companion_row,X

   ; epilogue
   CLC
   LDA   FP
   ADC   #$55
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$51
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@5:
   LDY   #$53
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   TAX
   LDA   companion_dir,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .if_else@7
.lb_skip@2:
   LDY   #$54
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$30
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   LDY   #$54
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$38
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@17
   EOR   #$80
.cmp_novf@17:
   ASL
   LDA   #$00
   ROL
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@9
.and_false@8:
   LDA   #$00
   LDY   #$3D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@9:
   LDY   #$3D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .if_end@6
.lb_skip@1:
   LDA   #$00
   LDY   #$3F
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   TAX
   LDY   #$3F
   LDA   (FP),Y
   STA   companion_state,X
   JMP   .if_end@6
.if_else@7:
   LDY   #$54
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$50
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@11
   LDY   #$54
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$58
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   ASL
   LDA   #$00
   ROL
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@11
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@12
.and_false@11:
   LDA   #$00
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@12:
   LDY   #$4B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@13
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   TAX
   LDY   #$4D
   LDA   (FP),Y
   STA   companion_state,X
.if_end@13:
.if_end@6:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$55
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$51
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

smc_body_draw:
   SUBROUTINE

   ; prologue: 6 arg bytes, 95 local bytes
   SEC
   LDA   SSP
   SBC   #$61
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$60
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   TAX
   LDA   companion_dir,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@15
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .if_end@14
.if_else@15:
   LDY   #$0C
   LDA   (FP),Y
   BEQ   .cond_else@16
   LDA   #<neg_walk_next
   LDY   #$13
   STA   (FP),Y
   LDA   #>neg_walk_next
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   JMP   .cond_end@17
.cond_else@16:
   LDA   #<pos_walk_next
   LDY   #$17
   STA   (FP),Y
   LDA   #>pos_walk_next
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
.cond_end@17:
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$21
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$23
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
.if_end@14:
   LDY   #$0C
   LDA   (FP),Y
   BNE   .lb_skip@4
   JMP   .if_else@19
.lb_skip@4:
   LDY   #$12
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$27
   STA   (FP),Y
   TAX
   LDA   companion_neg_lo_tbl,X
   INY
   STA   (FP),Y
   LDA   companion_neg_lo_tbl+1,X
   INY
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   CLC
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$33
   STA   (FP),Y
   TAX
   LDA   companion_neg_hi_tbl,X
   INY
   STA   (FP),Y
   LDA   companion_neg_hi_tbl+1,X
   INY
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   CLC
   LDY   #$36
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$37
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3A
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .if_end@18
.if_else@19:
   LDY   #$12
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$3F
   STA   (FP),Y
   TAX
   LDA   companion_pos_lo_tbl,X
   INY
   STA   (FP),Y
   LDA   companion_pos_lo_tbl+1,X
   INY
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   CLC
   LDY   #$42
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$43
   ADC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$4A
   STA   (FP),Y
   TAX
   LDA   companion_pos_hi_tbl,X
   INY
   STA   (FP),Y
   LDA   companion_pos_hi_tbl+1,X
   INY
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   CLC
   LDY   #$4D
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$4E
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
.if_end@18:
   LDY   #$52
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$58
   ORA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$59
   ORA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$08
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   JSR   draw_sprite

   ; epilogue
   CLC
   LDA   FP
   ADC   #$67
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$60
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

player_catch:
   SUBROUTINE

   ; prologue: 3 arg bytes, 43 local bytes
   SEC
   LDA   SSP
   SBC   #$2D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2C
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$2F
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$40
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@24
   EOR   #$80
.cmp_novf@24:
   ASL
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@20
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$50
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@25
   EOR   #$80
.cmp_novf@25:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@20
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@21
.or_true@20:
   LDA   #$01
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@21:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@22

   ; epilogue
   CLC
   LDA   FP
   ADC   #$30
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2C
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@22:
   LDY   #$30
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$08
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   TAX
   LDA   companion_row,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   SEC
   LDY   #$17
   SBC   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$18
   SBC   (FP),Y
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$30
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2C
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@23:
   LDY   #$10
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$1A
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   TAX
   LDA   companion_row,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   SEC
   LDY   #$27
   SBC   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$28
   SBC   (FP),Y
   BVC   .cmp_novf@27
   EOR   #$80
.cmp_novf@27:
   ASL
   LDA   #$00
   ROL
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@24

   ; epilogue
   CLC
   LDA   FP
   ADC   #$30
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2C
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@24:
   LDA   #$FF
   LDY   #$2B
   STA   (FP),Y
   STA   hit_flag

   ; epilogue
   CLC
   LDA   FP
   ADC   #$30
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

active_pos_step:
   SUBROUTINE

   ; prologue: 2 arg bytes, 68 local bytes
   SEC
   LDA   SSP
   SBC   #$46
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$45
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   prng
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$05
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@25
   LDA   #$FF
   INY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   TAX
   LDY   #$06
   LDA   (FP),Y
   STA   companion_dir,X
.if_end@25:
   LDY   #$47
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   TAX
   LDA   companion_pos_hi,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   TAX
   LDA   companion_pos_lo,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   ORA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   ORA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   TAX
   LDY   #$20
   LDA   (FP),Y
   STA   companion_pos_lo,X
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   TAX
   LDY   #$26
   LDA   (FP),Y
   STA   companion_pos_hi,X
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@29
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@26
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$52
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@31
   EOR   #$80
.cmp_novf@31:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@26
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@27
.and_false@26:
   LDA   #$00
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@27:
   LDY   #$33
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .if_end@28
.lb_skip@5:
   LDA   #$FF
   LDY   #$35
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   TAX
   LDY   #$35
   LDA   (FP),Y
   STA   companion_dir,X
   LDY   #$48
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   TAX
   LDA   floor_thresh,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   TAX
   LDY   #$41
   LDA   (FP),Y
   STA   companion_row,X
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$48
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$45
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@28:
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$48
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$45
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

active_neg_step:
   SUBROUTINE

   ; prologue: 2 arg bytes, 68 local bytes
   SEC
   LDA   SSP
   SBC   #$46
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$45
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   prng
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$05
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@32
   EOR   #$80
.cmp_novf@32:
   ASL
   LDA   #$00
   ROL
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@29
   LDA   #$01
   INY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   TAX
   LDY   #$06
   LDA   (FP),Y
   STA   companion_dir,X
.if_end@29:
   LDY   #$47
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   TAX
   LDA   companion_pos_hi,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   TAX
   LDA   companion_pos_lo,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   ORA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   ORA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$03
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   TAX
   LDY   #$20
   LDA   (FP),Y
   STA   companion_pos_lo,X
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   TAX
   LDY   #$26
   LDA   (FP),Y
   STA   companion_pos_hi,X
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@30
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$3E
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@35
   EOR   #$80
.cmp_novf@35:
   ASL
   LDA   #$00
   ROL
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@30
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@31
.and_false@30:
   LDA   #$00
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@31:
   LDY   #$33
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .if_end@32
.lb_skip@6:
   LDA   #$01
   LDY   #$35
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   TAX
   LDY   #$35
   LDA   (FP),Y
   STA   companion_dir,X
   LDY   #$48
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   TAX
   LDA   floor_thresh,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   TAX
   LDY   #$41
   LDA   (FP),Y
   STA   companion_row,X
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$48
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$45
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@32:
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$48
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$45
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

drift_step:
   SUBROUTINE

   ; prologue: 3 arg bytes, 80 local bytes
   SEC
   LDA   SSP
   SBC   #$52
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$51
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   TAX
   LDA   companion_row,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$63
   BEQ   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@35
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$8B
   BEQ   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@35
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@36
.or_true@35:
   LDA   #$01
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@36:
   LDY   #$0E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@33
   LDY   #$05
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$B3
   BEQ   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@33
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@34
.or_true@33:
   LDA   #$01
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@34:
   LDY   #$14
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .if_else@38
.lb_skip@7:
   LDY   #$05
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$04
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$01
   LDY   #$1B
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   TAX
   LDY   #$1B
   LDA   (FP),Y
   STA   companion_state,X
   LDY   #$53
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   TAX
   LDA   companion_pos_hi,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   TAX
   LDA   companion_pos_lo,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2D
   ORA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2E
   ORA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   TAX
   LDA   companion_dir,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@40
   LDY   #$31
   LDA   (FP),Y
   SEC
   SBC   #$03
   LDY   #$3B
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   SBC   #$00
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   JMP   .if_end@39
.if_else@40:
   LDY   #$31
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$3D
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
.if_end@39:
   LDY   #$31
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   TAX
   LDY   #$3F
   LDA   (FP),Y
   STA   companion_pos_lo,X
   LDY   #$32
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   TAX
   LDY   #$45
   LDA   (FP),Y
   STA   companion_pos_hi,X
   JMP   .if_end@37
.if_else@38:
   LDY   #$05
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   TAX
   LDY   #$05
   LDA   (FP),Y
   STA   companion_row,X
   LDY   #$54
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$05
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
.if_end@37:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$55
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$51
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

companion_update:
   SUBROUTINE

   ; prologue: 7 arg bytes, 115 local bytes
   SEC
   LDA   SSP
   SBC   #$75
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$74
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@41

   ; epilogue
   CLC
   LDA   FP
   ADC   #$7C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$74
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@41:
   LDA   #$01
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$07
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$08
   STA   (FP),Y
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@12
   JMP   .loop@1_break
.lb_skip@12:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   TAX
   LDA   companion_state,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@11
   JMP   .if_end@42
.lb_skip@11:
   TXA
   LDY   #$14
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$15
   LDY   #$16
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$14
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   drift_step
   LDY   #$06
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   compute_screen_x
   LDY   #$19
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   TAX
   LDA   proj_screen_col,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   TAX
   LDA   proj_frame_idx,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   TAX
   LDA   companion_state,X
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$29
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   smc_body_draw
   LDY   #$06
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   player_catch
   JMP   .loop@1_continue
.if_end@42:
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@48
   LDA   #$00
   JMP   .cmp_end@49
.cmp_true@48:
   LDA   #$01
.cmp_end@49:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@44
   LDA   #$01
   INY
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@50
   LDA   #$00
   JMP   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   TAX
   LDY   #$33
   LDA   (FP),Y
   STA   companion_state,X
   JMP   .if_end@43
.if_else@44:
   TXA
   LDY   #$37
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   TAX
   LDA   companion_dir,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@45
   LDY   #$06
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   active_neg_step
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   JMP   .cond_end@46
.cond_else@45:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   active_pos_step
   LDY   #$46
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
.cond_end@46:
   LDY   #$43
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@54
   LDA   #$00
   JMP   .lnot_end@55
.lnot_true@54:
   LDA   #$01
.lnot_end@55:
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .loop@1_continue
.lb_skip@10:
.if_end@43:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   compute_screen_x
   LDY   #$4E
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@56
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$54
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .loop@1_continue
.lb_skip@9:
   LDY   #$50
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$9A
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@58
   EOR   #$80
.cmp_novf@58:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .loop@1_continue
.lb_skip@8:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   entity_proximity
   LDY   #$06
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@59
   LDA   #$00
   JMP   .sx_done@60
.sx_neg@59:
   LDA   #$FF
.sx_done@60:
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   TAX
   LDA   companion_row,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   TAX
   LDA   proj_screen_col,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   TAX
   LDA   proj_frame_idx,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@61
   LDA   #$00
   JMP   .sx_done@62
.sx_neg@61:
   LDA   #$FF
.sx_done@62:
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   TAX
   LDA   companion_state,X
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   smc_body_draw
   LDY   #$06
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   player_catch
.loop@1_continue:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$7C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$74
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

proj_screen_col:
   DC.B  $00, $00, $00, $00, $01, $01, $01, $02, $02, $02, $02, $03, $03, $03, $04, $04
   DC.B  $04, $04, $05, $05, $05, $06, $06, $06, $06, $07, $07, $07, $08, $08, $08, $08
   DC.B  $09, $09, $09, $0A, $0A, $0A, $0A, $0B, $0B, $0B, $0C, $0C, $0C, $0C, $0D, $0D
   DC.B  $0D, $0E, $0E, $0E, $0E, $0F, $0F, $0F, $10, $10, $10, $10, $11, $11, $11, $12
   DC.B  $12, $12, $12, $13, $13, $13, $14, $14, $14, $14, $15, $15, $15, $16, $16, $16
   DC.B  $16, $17, $17, $17, $18, $18, $18, $18, $19, $19, $19, $1A, $1A, $1A, $1A, $1B
   DC.B  $1B, $1B, $1C, $1C, $1C, $1C, $1D, $1D, $1D, $1E, $1E, $1E, $1E, $1F, $1F, $1F
   DC.B  $20, $20, $20, $20, $21, $21, $21, $22, $22, $22, $22, $23, $23, $23, $24, $24
   DC.B  $24, $24, $25, $25

proj_frame_idx:
   DC.B  $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01
   DC.B  $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03
   DC.B  $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05
   DC.B  $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00
   DC.B  $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02
   DC.B  $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04
   DC.B  $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06
   DC.B  $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01
   DC.B  $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03
   DC.B  $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05
   DC.B  $06, $00, $01, $02, $03

companion_pos_pose1_lo:
   DC.B  $51, $71, $91, $B1, $D1, $F1, $11

companion_pos_pose1_hi:
   DC.B  $A0, $A0, $A0, $A0, $A0, $A0, $A1

companion_pos_pose2_lo:
   DC.B  $31, $51, $71, $91, $B1, $D1, $F1

companion_pos_pose2_hi:
   DC.B  $A1, $A1, $A1, $A1, $A1, $A1, $A1

companion_pos_pose3_lo:
   DC.B  $11, $31, $51, $71, $91, $B1, $D1

companion_pos_pose3_hi:
   DC.B  $A2, $A2, $A2, $A2, $A2, $A2, $A2

companion_neg_pose1_lo:
   DC.B  $B1, $D1, $F1, $11, $31, $51, $71

companion_neg_pose1_hi:
   DC.B  $9D, $9D, $9D, $9E, $9E, $9E, $9E

companion_neg_pose2_lo:
   DC.B  $91, $B1, $D1, $F1, $11, $31, $51

companion_neg_pose2_hi:
   DC.B  $9E, $9E, $9E, $9E, $9F, $9F, $9F

companion_neg_pose3_lo:
   DC.B  $71, $91, $B1, $D1, $F1, $11, $31

companion_neg_pose3_hi:
   DC.B  $9F, $9F, $9F, $9F, $9F, $A0, $A0

companion_pos_lo_tbl:
   DC.W  companion_pos_pose1_lo
   DC.W  companion_pos_pose2_lo
   DC.W  companion_pos_pose3_lo

companion_pos_hi_tbl:
   DC.W  companion_pos_pose1_hi
   DC.W  companion_pos_pose2_hi
   DC.W  companion_pos_pose3_hi

companion_neg_lo_tbl:
   DC.W  companion_neg_pose1_lo
   DC.W  companion_neg_pose2_lo
   DC.W  companion_neg_pose3_lo

companion_neg_hi_tbl:
   DC.W  companion_neg_pose1_hi
   DC.W  companion_neg_pose2_hi
   DC.W  companion_neg_pose3_hi

pos_walk_next:
   DS.B  1

neg_walk_next:
   DS.B  1
