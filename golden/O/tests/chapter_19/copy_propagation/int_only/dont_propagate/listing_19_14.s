SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def f params= locals= indirect=false in_cycle=true
; def indirect_update params= locals= indirect=false in_cycle=true
; def main params= locals= indirect=false in_cycle=false
; call f -> indirect_update
; call indirect_update -> f
; call main -> f
; item f section=.text local=false zero=false
; item indirect_update section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @1.total section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

f:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
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

   INY
   LDA   (FP),Y
   STA   @1.total
   INY
   LDA   (FP),Y
   STA   @1.total+1
   LDA   #$64
   SEC
   SBC   @1.total
   LDA   #$00
   SBC   @1.total+1
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .if_end@0
   LDX   #$00
   TXA

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
   LDA   #$0A
   STA   @1.total
   LDA   #$00
   STA   @1.total+1
   JSR   indirect_update
   LDX   @1.total+1
   LDA   @1.total

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

indirect_update:
   SUBROUTINE

   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$65
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   SEC
   LDA   SSP
   SBC   #$02
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
   JMP   f

@1.total:
   DS.B  2
