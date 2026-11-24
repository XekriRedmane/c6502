SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def check_arguments params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check_arguments
; item check_arguments section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

check_arguments:
   SUBROUTINE

   ; prologue: 74 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$0A
   LDA   (FP),Y
   CMP   #$3F
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   CMP   #$F0
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
.if_end@0:
   LDY   #$12
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
   LDY   #$1C
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   CMP   #$08
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
   LDY   #$24
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
   LDY   #$32
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
   LDY   #$3A
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   CMP   #$18
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
   LDY   #$42
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   CMP   #$1C
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$07

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
   LDY   #$4C
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   CMP   #$20
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   #$00
   LDA   #$08

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
.if_end@7:
   LDY   #$14
   LDA   (FP),Y
   BNE   .jcmp_skip@8
   DEY
   LDA   (FP),Y
   CMP   #$65
   BEQ   .if_end@8
.jcmp_skip@8:
   LDX   #$00
   LDA   #$09

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
.if_end@8:
   LDY   #$26
   LDA   (FP),Y
   BNE   .jcmp_skip@9
   DEY
   LDA   (FP),Y
   CMP   #$66
   BEQ   .if_end@9
.jcmp_skip@9:
   LDX   #$00
   LDA   #$0A

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
   LDY   #$28
   LDA   (FP),Y
   BNE   .jcmp_skip@10
   DEY
   LDA   (FP),Y
   CMP   #$67
   BEQ   .if_end@10
.jcmp_skip@10:
   LDX   #$00
   LDA   #$0B

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
.if_end@10:
   LDY   #$2A
   LDA   (FP),Y
   BNE   .jcmp_skip@11
   DEY
   LDA   (FP),Y
   CMP   #$68
   BEQ   .if_end@11
.jcmp_skip@11:
   LDX   #$00
   LDA   #$0C

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
   LDY   #$44
   LDA   (FP),Y
   BNE   .jcmp_skip@12
   DEY
   LDA   (FP),Y
   CMP   #$69
   BEQ   .if_end@12
.jcmp_skip@12:
   LDX   #$00
   LDA   #$0D

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
.if_end@12:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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
   SBC   #$4A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
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
   LDA   #$F0
   INY
   STA   (SSP),Y
   LDA   #$3F
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
   LDA   #$40
   INY
   STA   (SSP),Y
   LDA   #$65
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
   LDA   #$08
   INY
   STA   (SSP),Y
   LDA   #$40
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
   LDA   #$10
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   LDA   #$66
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$67
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$68
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
   LDA   #$14
   INY
   STA   (SSP),Y
   LDA   #$40
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
   LDA   #$18
   INY
   STA   (SSP),Y
   LDA   #$40
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
   LDA   #$1C
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   LDA   #$69
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
   LDA   #$20
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   JMP   check_arguments
