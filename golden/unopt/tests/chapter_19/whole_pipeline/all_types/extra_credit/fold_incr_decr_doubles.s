SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

target:
   SUBROUTINE

   ; prologue: 0 arg bytes, 124 local bytes
   SEC
   LDA   SSP
   SBC   #$7E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$7D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$FF
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$3F
   INY
   STA   (FP),Y
   LDA   #$43
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   INY
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   LDY   #$11
   STA   (FP),Y
   LDA   HARGS+17
   INY
   STA   (FP),Y
   LDA   HARGS+18
   INY
   STA   (FP),Y
   LDA   HARGS+19
   INY
   STA   (FP),Y
   LDA   HARGS+20
   INY
   STA   (FP),Y
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   INY
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   LDY   #$21
   STA   (FP),Y
   LDA   HARGS+17
   INY
   STA   (FP),Y
   LDA   HARGS+18
   INY
   STA   (FP),Y
   LDA   HARGS+19
   INY
   STA   (FP),Y
   LDA   HARGS+20
   INY
   STA   (FP),Y
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$24
   INY
   STA   (FP),Y
   LDA   #$40
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   INY
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dsub
   LDA   HARGS+16
   LDY   #$39
   STA   (FP),Y
   LDA   HARGS+17
   INY
   STA   (FP),Y
   LDA   HARGS+18
   INY
   STA   (FP),Y
   LDA   HARGS+19
   INY
   STA   (FP),Y
   LDA   HARGS+20
   INY
   STA   (FP),Y
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   INY
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dsub
   LDA   HARGS+16
   LDY   #$51
   STA   (FP),Y
   LDA   HARGS+17
   INY
   STA   (FP),Y
   LDA   HARGS+18
   INY
   STA   (FP),Y
   LDA   HARGS+19
   INY
   STA   (FP),Y
   LDA   HARGS+20
   INY
   STA   (FP),Y
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   CMP   #$43
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDY   #$20
   LDA   (FP),Y
   CMP   #$43
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$3F
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$65
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$65
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@2
.lb_skip@0:
   LDY   #$19
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   INY
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@2
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   LDY   #$69
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$69
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDY   #$08
   LDA   (FP),Y
   LDY   #$30
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$07
   LDA   (FP),Y
   LDY   #$2F
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$06
   LDA   (FP),Y
   LDY   #$2E
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$05
   LDA   (FP),Y
   LDY   #$2D
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$04
   LDA   (FP),Y
   LDY   #$2C
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$03
   LDA   (FP),Y
   LDY   #$2B
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$02
   LDA   (FP),Y
   LDY   #$2A
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$01
   LDA   (FP),Y
   LDY   #$29
   CMP   (FP),Y
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$6D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$6D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@9
   LDA   #$00
   JMP   .lnot_end@10
.lnot_true@9:
   LDA   #$01
.lnot_end@10:
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$7E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7D
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
.if_end@6:
   LDY   #$38
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$20
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@11
.thread@3:
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@9
   LDY   #$48
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$22
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@14
.thread@4:
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@9
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@10
.and_false@9:
   LDA   #$00
   LDY   #$75
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@10:
   LDY   #$75
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDY   #$60
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   CMP   #$22
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@17
.thread@5:
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@8
.and_false@7:
   LDA   #$00
   LDY   #$79
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@8:
   LDY   #$79
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@20
   LDA   #$00
   JMP   .lnot_end@21
.lnot_true@20:
   LDA   #$01
.lnot_end@21:
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@11
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$7E
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
   PLA
   TAX
   PLA
   RTS
.if_end@11:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$7E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7D
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

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 2 local bytes
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$03
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   target
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
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
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$03
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
