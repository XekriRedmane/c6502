SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
i	EQU	$26
j	EQU	$28
__local_main__0	EQU	$80
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def decr_j params= locals= indirect=false in_cycle=false
; def incr_i params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; call main -> decr_j
; call main -> incr_i
; item incr_i section=.text local=false zero=false
; item decr_j section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp i size=2 at=auto def=true init=$00,$00 weight=10
; zp j size=2 at=auto def=true init=$00,$00 weight=7
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

incr_i:
   SUBROUTINE

   LDA   i+1
   BNE   .if_end@0
   LDA   i
   CMP   #$01
   BNE   .if_end@0
   INC   i
   BNE   .inc_done@0
   INC   i+1
.inc_done@0:
   INC   i
   BNE   .inc_done@1
   INC   i+1
.inc_done@1:
.if_end@0:
   LDX   #$00
   TXA
   RTS

decr_j:
   SUBROUTINE

   LDA   j+1
   CMP   #$FF
   BNE   .if_end@1
   LDA   j
   CMP   #$FF
   BNE   .if_end@1
   ORA   #$00
   BNE   .carry_inc@0
   DEC   j+1
.carry_inc@0:
   DEC   j
.if_end@1:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   i
   STA   __local_main__1
   LDA   i+1
   STA   __local_main__0
   INC   i
   BNE   .inc_done@2
   INC   i+1
.inc_done@2:
   LDA   __local_main__1
   ORA   __local_main__0
   BNE   .cond_end@3
   JSR   incr_i
.cond_end@3:
   LDA   i+1
   BNE   .jcmp_skip@0
   LDA   i
   CMP   #$03
   BEQ   .if_end@4
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@4:
   LDA   j
   SEC
   SBC   #$01
   STA   j
   BCS   .carry_inc@1
   DEC   j+1
.carry_inc@1:
   ORA   j+1
   BEQ   .main@cthread@0
   JSR   decr_j
   LDA   j+1
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   j
   CMP   #$FE
   BEQ   .if_end@7
.jcmp_skip@1:
.main@cthread@0:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@7:
   LDX   #$00
   TXA
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   i
   STA   i+1
   STA   j
   STA   j+1
   RTS
