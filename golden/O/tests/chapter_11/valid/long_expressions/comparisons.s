SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80

; @zp-link-meta-begin
; def compare_constants params= locals= indirect=false in_cycle=false
; def compare_constants_2 params= locals= indirect=false in_cycle=false
; def l_eq_l2 params= locals=__local_l_eq_l2__0 indirect=false in_cycle=false
; def l_geq_2_30 params= locals=__local_l_geq_2_30__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def ulong_max_leq_l params= locals=__local_ulong_max_leq_l__0 indirect=false in_cycle=false
; call main -> compare_constants
; call main -> compare_constants_2
; call main -> l_eq_l2
; call main -> l_geq_2_30
; call main -> ulong_max_leq_l
; item compare_constants section=.text local=false zero=false
; item compare_constants@tail1 section=.text local=true zero=false
; item compare_constants_2 section=.text local=false zero=false
; item l_geq_2_30 section=.text local=false zero=false
; item l_geq_2_30@tail0 section=.text local=true zero=false
; item ulong_max_leq_l section=.text local=false zero=false
; item l_eq_l2 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; item l section=.bss local=false zero=true
; item l2 section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

compare_constants:
   SUBROUTINE

   JMP   compare_constants@tail1

compare_constants@tail1:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

compare_constants_2:
   SUBROUTINE

   JMP   compare_constants@tail1

l_geq_2_30:
   SUBROUTINE

   SEC
   LDA   l+2
   LDA   l+3
   SBC   #$40
   JMP   l_geq_2_30@tail0

l_geq_2_30@tail0:
   SUBROUTINE

   LDA   l+4
   SBC   #$00
   LDA   l+5
   SBC   #$00
   LDA   l+6
   SBC   #$00
   LDA   l+7
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDX   #$00
   RTS

ulong_max_leq_l:
   SUBROUTINE

   LDA   l
   SEC
   SBC   #$FF
   LDA   l+1
   SBC   #$FF
   LDA   l+2
   SBC   #$00
   LDA   l+3
   SBC   #$00
   JMP   l_geq_2_30@tail0

l_eq_l2:
   SUBROUTINE

   LDA   l+7
   CMP   l2+7
   BNE   .thread@0
   LDA   l+6
   CMP   l2+6
   BNE   .thread@0
   LDA   l+5
   CMP   l2+5
   BNE   .thread@0
   LDA   l+4
   CMP   l2+4
   BNE   .thread@0
   LDA   l+3
   CMP   l2+3
   BNE   .thread@0
   LDA   l+2
   CMP   l2+2
   BNE   .thread@0
   LDA   l+1
   CMP   l2+1
   BNE   .thread@0
   LDA   l
   CMP   l2
   BEQ   .cmp_true@2
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   JSR   compare_constants
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   JMP   compare_constants@tail1
.if_end@0:
   JSR   compare_constants_2
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$01
   STA   l
   LDA   #$00
   STA   l+1
   STA   l+2
   LDA   #$80
   STA   l+3
   LDA   #$FF
   STA   l+4
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   ulong_max_leq_l
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #$40
   STA   l+3
   LDA   #$00
   STA   l
   STA   l+1
   STA   l+2
   STA   l+4
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   JSR   ulong_max_leq_l
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   l
   STA   l2
   LDA   l+1
   STA   l2+1
   LDA   l+2
   STA   l2+2
   LDA   l+3
   STA   l2+3
   LDA   l+4
   STA   l2+4
   LDA   l+5
   STA   l2+5
   LDA   l+6
   STA   l2+6
   LDA   l+7
   STA   l2+7
   JSR   l_eq_l2
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   STA   l+5
   STA   l+6
   STA   l+7
   JMP   l_geq_2_30

l:
   DS.B  8

l2:
   DS.B  8
