SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; call main -> overlap_with_globvar
; call main -> overlap_with_pointer
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 36 local bytes
   SEC
   LDA   SSP
   SBC   #$26
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$25
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
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   overlap_with_globvar
   LDY   #$01
   LDA   (FP),Y
   STA   globvar
   INY
   LDA   (FP),Y
   STA   globvar+1
   INY
   LDA   (FP),Y
   STA   globvar+2
   INY
   LDA   (FP),Y
   STA   globvar+3
   INY
   LDA   (FP),Y
   STA   globvar+4
   INY
   LDA   (FP),Y
   STA   globvar+5
   INY
   LDA   (FP),Y
   STA   globvar+6
   INY
   LDA   (FP),Y
   STA   globvar+7
   INY
   LDA   (FP),Y
   STA   globvar+8
   INY
   LDA   (FP),Y
   STA   globvar+9
   INY
   LDA   (FP),Y
   STA   globvar+10
   INY
   LDA   (FP),Y
   STA   globvar+11
   LDA   globvar
   STA   $83
   LDA   globvar+1
   STA   $82
   LDA   globvar+2
   STA   $81
   LDA   globvar+3
   BNE   .main@jthread@1
   LDA   $81
   BNE   .main@jthread@1
   LDA   $82
   CMP   #$01
   BNE   .main@jthread@1
   LDA   $83
   CMP   #$90
   BNE   .main@jthread@1
   LDA   globvar+4
   STA   $83
   LDA   globvar+5
   STA   $82
   LDA   globvar+6
   STA   $81
   LDA   globvar+7
   BNE   .main@jthread@1
   LDA   $81
   BNE   .main@jthread@1
   LDA   $82
   CMP   #$01
   BNE   .main@jthread@1
   LDA   $83
   CMP   #$F4
   BNE   .main@jthread@1
   LDA   globvar+8
   STA   $83
   LDA   globvar+9
   STA   $82
   LDA   globvar+10
   STA   $81
   LDA   globvar+11
   BNE   .main@jthread@1
   LDA   $81
   BNE   .main@jthread@1
   LDA   $82
   CMP   #$02
   BNE   .main@jthread@1
   LDA   $83
   CMP   #$58
   BEQ   .if_end@4
.main@jthread@1:
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
   LDA   #$0A
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$09
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$08
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0D
   STA   $82
   LDA   FP+1
   ADC   #$00
   STA   $83
   CLC
   LDA   FP
   ADC   #$19
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   $83
   INY
   STA   (SSP),Y
   JSR   overlap_with_pointer
   LDY   #$19
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   STA   $83
   INY
   LDA   (FP),Y
   STA   $82
   INY
   LDA   (FP),Y
   STA   $81
   INY
   LDA   (FP),Y
   BNE   .main@jthread@3
   LDA   $81
   BNE   .main@jthread@3
   LDA   $82
   BNE   .main@jthread@3
   LDA   $83
   CMP   #$14
   BNE   .main@jthread@3
   INY
   LDA   (FP),Y
   STA   $83
   INY
   LDA   (FP),Y
   STA   $82
   INY
   LDA   (FP),Y
   STA   $81
   INY
   LDA   (FP),Y
   BNE   .main@jthread@3
   LDA   $81
   BNE   .main@jthread@3
   LDA   $82
   BNE   .main@jthread@3
   LDA   $83
   CMP   #$12
   BNE   .main@jthread@3
   INY
   LDA   (FP),Y
   STA   $83
   INY
   LDA   (FP),Y
   STA   $82
   INY
   LDA   (FP),Y
   STA   $81
   INY
   LDA   (FP),Y
   BNE   .main@jthread@3
   LDA   $81
   BNE   .main@jthread@3
   LDA   $82
   BNE   .main@jthread@3
   LDA   $83
   CMP   #$10
   BEQ   .if_end@9
.main@jthread@3:
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
.if_end@9:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
