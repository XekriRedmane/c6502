SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def incr_i params= locals= indirect=false in_cycle=false
; def incr_j params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call main -> incr_i
; call main -> incr_j
; item incr_i section=.text local=false zero=false
; item incr_j section=.text local=false zero=false
; item main section=.text local=false zero=false
; item i section=.data local=false zero=false
; item j section=.data local=false zero=false
; item flag_1 section=.data local=false zero=false
; item flag_0 section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

incr_i:
   SUBROUTINE

   INC   i
   BNE   .inc_done@0
   INC   i+1
.inc_done@0:
   RTS

incr_j:
   SUBROUTINE

   INC   j
   BNE   .inc_done@1
   INC   j+1
.inc_done@1:
   RTS

main:
   SUBROUTINE

   LDA   flag_1
   ORA   flag_1+1
   BEQ   .cond_else@0
   JSR   incr_i
   JMP   .cond_end@1
.cond_else@0:
   JSR   incr_j
.cond_end@1:
   LDA   flag_0
   ORA   flag_0+1
   BEQ   .cond_else@2
   JSR   incr_i
   JMP   .cond_end@3
.cond_else@2:
   JSR   incr_j
.cond_end@3:
   LDA   i+1
   BNE   .jcmp_skip@0
   LDA   i
   CMP   #$05
   BEQ   .if_end@4
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@4:
   LDA   j+1
   BNE   .jcmp_skip@1
   LDA   j
   CMP   #$06
   BEQ   .if_end@5
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@5:
   LDA   flag_0
   ORA   flag_0+1
   BEQ   .cond_else@6
   JSR   incr_j
   JMP   .cond_end@7
.cond_else@6:
   LDA   flag_1
   ORA   flag_1+1
   BEQ   .cond_else@8
   JSR   incr_i
   JMP   .cond_end@9
.cond_else@8:
   JSR   incr_j
.cond_end@9:
.cond_end@7:
   LDA   i+1
   BNE   .jcmp_skip@2
   LDA   i
   CMP   #$06
   BEQ   .if_end@10
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@10:
   LDA   j+1
   BNE   .jcmp_skip@3
   LDA   j
   CMP   #$06
   BEQ   .if_end@11
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@11:
   LDX   #$00
   TXA
   RTS

i:
   DC.W  $0004

j:
   DC.W  $0005

flag_1:
   DC.W  $0001

flag_0:
   DS.B  2
