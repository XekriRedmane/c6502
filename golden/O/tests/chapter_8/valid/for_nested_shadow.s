SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__i	EQU	$80
__local_main__k_1	EQU	$81
__local_main__k_0	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__i,__local_main__k_1,__local_main__k_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__k_0
   LDA   #$00
   STA   __local_main__k_1
   LDA   #$64
   STA   __local_main__i
   LDX   #$00
   BEQ   .loop@0_start
.main@rot@0:
   ORA   __local_main__i
   BEQ   .loop@0_break
   LDA   #$01
   CLC
   ADC   __local_main__k_0
   STA   __local_main__k_0
   LDA   #$00
   ADC   __local_main__k_1
   STA   __local_main__k_1
   LDA   __local_main__i
   SEC
   SBC   #$01
   STA   __local_main__i
   BCS   .loop@0_start
   DEX
.loop@0_start:
   TXA
   BPL   .main@rot@0
.loop@0_break:
   LDA   __local_main__k_1
   BNE   .and_false@0
   LDA   __local_main__k_0
   CMP   #$65
   BNE   .and_false@0
   LDA   #$01
   LDX   #$00
   RTS
.and_false@0:
   LDA   #$00
   LDX   #$00
   RTS
