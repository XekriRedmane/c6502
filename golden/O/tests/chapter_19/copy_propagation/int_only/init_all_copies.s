SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
counter	EQU	$26
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def increment_counter params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params= locals= indirect=false in_cycle=false
; call main -> target
; call target -> increment_counter
; item increment_counter section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp counter size=2 at=auto def=true init=$00,$00 weight=11
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

increment_counter:
   SUBROUTINE

   INC   counter
   BNE   .inc_done@0
   INC   counter+1
.inc_done@0:
   LDX   #$00
   TXA
   RTS

target:
   SUBROUTINE

.loop@0_start:
   JSR   increment_counter
   LDA   counter
   SEC
   SBC   #$05
   LDA   counter+1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .loop@0_start
   LDX   #$00
   LDA   #$03
   RTS

main:
   SUBROUTINE

   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$03
   BEQ   .if_end@0
.jcmp_skip@1:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   counter+1
   BNE   .jcmp_skip@2
   LDA   counter
   CMP   #$05
   BEQ   .if_end@1
.jcmp_skip@2:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   counter
   STA   counter+1
   RTS
