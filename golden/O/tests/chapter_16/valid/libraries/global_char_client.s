SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; ext update_global_chars params=
; call main -> update_global_chars
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   c
   CMP   #$64
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   uc
   CMP   #$FA
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   sc
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   ORA   sc
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   update_global_chars
   LDA   c
   CMP   #$6E
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   uc
   CMP   #$04
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   sc
   BPL   .jcmp_skip@4
   CMP   #$F6
   BEQ   .if_end@5
.jcmp_skip@4:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS
