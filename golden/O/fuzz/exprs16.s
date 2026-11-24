SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v3	EQU	$26
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_3	EQU	$83
__local_check__r_2	EQU	$84
__local_check__r_1	EQU	$85
__local_check__r_0	EQU	$86

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.bss local=false zero=true
; item v4 section=.data local=false zero=false
; item v5 section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp v3 size=2 at=auto def=true init=$0A,$00 weight=7
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v0
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v3
   STA   HARGS
   STA   HARGS+2
   LDA   v3+1
   STA   HARGS+1
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   EOR   #$FF
   STA   HARGS+2
   LDA   HARGS+5
   EOR   #$FF
   STA   HARGS+3
   LDA   v3
   STA   HARGS
   LDA   v3+1
   STA   HARGS+1
   JSR   mul16
   LDA   HARGS+5
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+4
   CMP   #$EC
   BEQ   .if_end@1
.jcmp_skip@0:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   v3+1
   BNE   .jcmp_skip@1
   LDA   v3
   CMP   #$0A
   BEQ   .if_end@2
.jcmp_skip@1:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   v5
   ORA   v5+1
   ORA   v5+2
   ORA   v5+3
   BEQ   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   #$7F
   ORA   v0
   STA   __local_check__r_3
   LDA   v3
   SEC
   SBC   __local_check__r_3
   LDA   v3+1
   SBC   v0+1
   LDA   #$00
   ROL
   TAX
   LDA   v1
   STA   __local_check__r_3
   LDA   v1+1
   STA   __local_check__r_2
   CPX   #$00
   BEQ   .shift_done@3
.shift_loop@2:
   ASL   __local_check__r_3
   ROL   __local_check__r_2
   DEX
   BNE   .shift_loop@2
.shift_done@3:
   LDA   __local_check__r_2
   CMP   #$3A
   BNE   .jcmp_skip@4
   LDA   __local_check__r_3
   CMP   #$05
   BEQ   .if_end@4
.jcmp_skip@4:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   SEC
   LDA   #$00
   SBC   v1
   STA   __local_check__r_0
   LDA   #$00
   SBC   v1+1
   STA   __local_check__r_1
   LDA   v1
   AND   v3
   STA   __local_check__r_2
   LDA   v1+1
   AND   v3+1
   STA   __local_check__r_3
   LDA   __local_check__r_0
   SEC
   SBC   __local_check__r_2
   LDA   __local_check__r_1
   SBC   __local_check__r_3
   LDA   #$00
   ROL
   ORA   v3
   STA   __local_check__r_2
   LDA   v3+1
   BNE   .jcmp_skip@5
   LDA   __local_check__r_2
   CMP   #$0B
   BEQ   .if_end@5
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   #$00
   CMP   v5+3
   BNE   .cmp_true@6
   CMP   v5+2
   BNE   .cmp_true@6
   LDA   #$01
   CMP   v5+1
   BNE   .cmp_end@7
   LDA   #$00
   CMP   v5
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   __local_check__r_3
   LDA   #$00
   CMP   v1+1
   BNE   .cmp_true@9
   LDA   __local_check__r_3
   CMP   v1
   BNE   .cmp_true@9
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   __local_check__r_1
   LDA   v0
   ORA   v0+1
   BEQ   .and_false@6
   LDA   v0
   ORA   v0+1
   BEQ   .and_false@6
   LDA   #$01
   STA   __local_check__r_2
   JMP   .and_end@7
.and_false@6:
   LDA   #$00
   STA   __local_check__r_2
.and_end@7:
   LDA   __local_check__r_1
   SEC
   SBC   __local_check__r_2
   LDA   #$00
   SBC   #$00
   BVC   .jcmp_novf@12
   EOR   #$80
.jcmp_novf@12:
   BPL   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   v2
   AND   v5
   STA   HARGS
   LDA   v2+1
   AND   v5+1
   STA   HARGS+1
   LDA   #$00
   AND   v5+2
   STA   HARGS+2
   LDA   #$00
   AND   v5+3
   STA   HARGS+3
   LDA   v2
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   ORA   HARGS+6
   ORA   HARGS+7
   ORA   HARGS+8
   BEQ   .if_end@9
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@9:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@0

main:
   SUBROUTINE

   JMP   check

v0:
   DC.W  $8000

v1:
   DC.W  $3A05

v2:
   DS.B  2

v4:
   DC.B  $80

v5:
   DS.B  4

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   v3+1
   LDA   #$0A
   STA   v3
   RTS
