SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def fun params= locals= indirect=false in_cycle=true
; def main params= locals= indirect=false in_cycle=false
; call fun -> fun
; call main -> fun
; item fun section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

fun:
   SUBROUTINE

   ; prologue: 30 arg bytes, 18 local bytes, 18 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$14
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$13
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1
   LDA   $C0
   LDY   #$01
   STA   (FP),Y
   LDA   $C1
   INY
   STA   (FP),Y
   LDA   $C2
   INY
   STA   (FP),Y
   LDA   $C3
   INY
   STA   (FP),Y
   LDA   $C4
   INY
   STA   (FP),Y
   LDA   $C5
   INY
   STA   (FP),Y
   LDA   $C6
   INY
   STA   (FP),Y
   LDA   $C7
   INY
   STA   (FP),Y
   LDA   $C8
   INY
   STA   (FP),Y
   LDA   $C9
   INY
   STA   (FP),Y
   LDA   $CA
   INY
   STA   (FP),Y
   LDA   $CB
   INY
   STA   (FP),Y
   LDA   $CC
   INY
   STA   (FP),Y
   LDA   $CD
   INY
   STA   (FP),Y
   LDA   $CE
   INY
   STA   (FP),Y
   LDA   $CF
   INY
   STA   (FP),Y
   LDA   $D0
   INY
   STA   (FP),Y
   LDA   $D1
   INY
   STA   (FP),Y

   LDY   #$15
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   STA   $87
   LDA   HARGS+3
   STA   $86
   LDA   HARGS+4
   STA   $85
   LDA   HARGS+5
   STA   $84
   LDA   HARGS+6
   STA   $83
   LDA   HARGS+7
   STA   $82
   LDA   HARGS+8
   STA   $81
   LDA   HARGS+9
   LDY   #$32
   CMP   (FP),Y
   BNE   .jcmp_skip@0
   LDA   $81
   DEY
   CMP   (FP),Y
   BNE   .jcmp_skip@0
   LDA   $82
   DEY
   CMP   (FP),Y
   BNE   .jcmp_skip@0
   LDA   $83
   DEY
   CMP   (FP),Y
   BNE   .jcmp_skip@0
   LDA   $84
   DEY
   CMP   (FP),Y
   BNE   .jcmp_skip@0
   LDA   $85
   DEY
   CMP   (FP),Y
   BNE   .jcmp_skip@0
   LDA   $86
   DEY
   CMP   (FP),Y
   BNE   .jcmp_skip@0
   LDA   $87
   DEY
   CMP   (FP),Y
   BNE   .lb_skip@6
   JMP   .if_end@0
.lb_skip@6:
.jcmp_skip@0:
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$01
   STA   $85
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $84
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$01
   STA   $83
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $82
   LDY   #$29
   LDA   (FP),Y
   CLC
   ADC   #$01
   STA   $81
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $80
   SEC
   LDA   SSP
   SBC   #$1E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $85
   LDY   #$01
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$14
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$17
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$18
   STA   (SSP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$19
   STA   (SSP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$1A
   STA   (SSP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$1B
   STA   (SSP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$1C
   STA   (SSP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$1D
   STA   (SSP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$1E
   STA   (SSP),Y
   JSR   fun
   STA   $D1
   STX   $D0
   LDY   #$17
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
   STA   $CF
   LDA   HARGS+17
   STA   $CE
   LDA   HARGS+18
   STA   $CD
   LDA   HARGS+19
   STA   $CC
   LDA   HARGS+20
   STA   $CB
   LDA   HARGS+21
   STA   $CA
   LDA   HARGS+22
   STA   $C9
   LDA   HARGS+23
   STA   $C8
   LDY   #$21
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
   STA   $C7
   LDA   HARGS+17
   STA   $C6
   LDA   HARGS+18
   STA   $C5
   LDA   HARGS+19
   STA   $C4
   LDA   HARGS+20
   STA   $C3
   LDA   HARGS+21
   STA   $C2
   LDA   HARGS+22
   STA   $C1
   LDA   HARGS+23
   STA   $C0
   LDY   #$2B
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
   STA   $87
   LDA   HARGS+17
   STA   $86
   LDA   HARGS+18
   STA   $85
   LDA   HARGS+19
   STA   $84
   LDA   HARGS+20
   STA   $83
   LDA   HARGS+21
   STA   $82
   LDA   HARGS+22
   STA   $81
   LDA   HARGS+23
   STA   $80
   SEC
   LDA   SSP
   SBC   #$1E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$15
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDA   $CF
   INY
   STA   (SSP),Y
   LDA   $CE
   INY
   STA   (SSP),Y
   LDA   $CD
   INY
   STA   (SSP),Y
   LDA   $CC
   INY
   STA   (SSP),Y
   LDA   $CB
   INY
   STA   (SSP),Y
   LDA   $CA
   INY
   STA   (SSP),Y
   LDA   $C9
   INY
   STA   (SSP),Y
   LDA   $C8
   INY
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDA   $C7
   INY
   STA   (SSP),Y
   LDA   $C6
   INY
   STA   (SSP),Y
   LDA   $C5
   INY
   STA   (SSP),Y
   LDA   $C4
   INY
   STA   (SSP),Y
   LDA   $C3
   INY
   STA   (SSP),Y
   LDA   $C2
   INY
   STA   (SSP),Y
   LDA   $C1
   INY
   STA   (SSP),Y
   LDA   $C0
   INY
   STA   (SSP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$15
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$16
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   LDA   $86
   INY
   STA   (SSP),Y
   LDA   $85
   INY
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   JSR   fun
   STA   $81
   STX   $80
   LDA   $D1
   ORA   $D0
   BNE   .lb_skip@5
   JMP   .if_end@1
.lb_skip@5:
   LDX   $D0
   LDA   $D1

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   CLC
   LDA   FP
   ADC   #$32
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
.if_end@1:
   LDA   $81
   ORA   $80
   BNE   .lb_skip@4
   JMP   .if_end@2
.lb_skip@4:
   LDA   $81

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   CLC
   LDA   FP
   ADC   #$32
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
.if_end@2:
.if_end@0:
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$02
   STA   $81
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $80
   LDY   #$20
   LDA   (FP),Y
   CMP   $80
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   CMP   $81
   BNE   .lb_skip@3
   JMP   .if_end@3
.lb_skip@3:
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   CLC
   LDA   FP
   ADC   #$32
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
.if_end@3:
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$04
   STA   $81
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $80
   LDY   #$2A
   LDA   (FP),Y
   CMP   $80
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   CMP   $81
   BNE   .lb_skip@2
   JMP   .if_end@4
.lb_skip@2:
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   CLC
   LDA   FP
   ADC   #$32
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
.if_end@4:
   LDY   #$2B
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
   LDA   #$10
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dsub
   LDA   HARGS+16
   STA   $87
   LDA   HARGS+17
   STA   $86
   LDA   HARGS+18
   STA   $85
   LDA   HARGS+19
   STA   $84
   LDA   HARGS+20
   STA   $83
   LDA   HARGS+21
   STA   $82
   LDA   HARGS+22
   STA   $81
   LDA   HARGS+23
   STA   $80
   LDY   #$1E
   LDA   (FP),Y
   CMP   $80
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   CMP   $81
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   CMP   $82
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   CMP   $83
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   CMP   $84
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   CMP   $85
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   CMP   $86
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   CMP   $87
   BNE   .lb_skip@1
   JMP   .if_end@5
.lb_skip@1:
.jcmp_skip@3:
   LDX   #$00
   LDA   #$0B

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   CLC
   LDA   FP
   ADC   #$32
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
.if_end@5:
   LDY   #$2B
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
   STY   HARGS+13
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dsub
   LDA   HARGS+16
   STA   $87
   LDA   HARGS+17
   STA   $86
   LDA   HARGS+18
   STA   $85
   LDA   HARGS+19
   STA   $84
   LDA   HARGS+20
   STA   $83
   LDA   HARGS+21
   STA   $82
   LDA   HARGS+22
   STA   $81
   LDA   HARGS+23
   STA   $80
   LDY   #$28
   LDA   (FP),Y
   CMP   $80
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   CMP   $81
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   CMP   $82
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   CMP   $83
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   CMP   $84
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   CMP   $85
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   CMP   $86
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   CMP   $87
   BNE   .lb_skip@0
   JMP   .if_end@6
.lb_skip@0:
.jcmp_skip@4:
   LDX   #$00
   LDA   #$0C

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   CLC
   LDA   FP
   ADC   #$32
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
.if_end@6:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   CLC
   LDA   FP
   ADC   #$32
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

main:
   SUBROUTINE

   SEC
   LDA   SSP
   SBC   #$1E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   LDA   #$03
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$10
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   LDA   #$05
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$18
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   JMP   fun
