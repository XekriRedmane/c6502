SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__ret	EQU	$80
__local_main__i_1	EQU	$81
__local_main__i_0	EQU	$82

; @zp-link-meta-begin
; def foo params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__ret,__local_main__i_1,__local_main__i_0 indirect=false in_cycle=false
; call main -> foo
; item foo section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @0.x section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

foo:
   SUBROUTINE

   INC   @0.x
   BNE   .inc_done@0
   INC   @0.x+1
.inc_done@0:
   LDX   @0.x+1
   LDA   @0.x
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

   LDA   #$00
   STA   __local_main__i_0
   STA   __local_main__i_1
   LDY   #$01
   LDA   (FP),Y
   STA   __local_main__ret
   INY
   LDA   (FP),Y
   TAX
.loop@0_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$04
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   JSR   foo
   STA   __local_main__ret
   INC   __local_main__i_0
   BNE   .loop@0_start
   INC   __local_main__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_main__ret

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

@0.x:
   DS.B  2
