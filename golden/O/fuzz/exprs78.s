SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_check__fail	EQU	$81
__local_check__r_1	EQU	$82
__local_check__r_0	EQU	$83
__local_check__0	EQU	$84
__local_check__1	EQU	$85

; @zp-link-meta-begin
; def check params= locals=__local_check__fail,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.bss local=false zero=true
; item v1 section=.bss local=false zero=true
; item v2 section=.bss local=false zero=true
; item v3 section=.data local=false zero=false
; item v4 section=.bss local=false zero=true
; item v5 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   #$00
   CMP   v1+3
   BNE   .cmp_end@1
   CMP   v1+2
   BNE   .cmp_end@1
   CMP   v1+1
   BNE   .cmp_end@1
   CMP   v1
   BNE   .cmp_end@1
   LDA   #$01
.cmp_end@1:
   STA   __local_check__1
   LDA   v5
   SEC
   SBC   #$FF
   LDA   v5+1
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   __local_check__r_1
   STA   __local_check__fail
   LDA   v4
   CLC
   SBC   v3
   LDA   v4+1
   SBC   #$00
   LDA   v4+2
   SBC   __local_check__r_1
   LDA   v4+3
   SBC   __local_check__fail
   LDA   #$00
   ROL
   STA   __local_check__fail
   LDA   __local_check__0
   EOR   __local_check__fail
   STA   __local_check__r_1
   LDA   __local_check__1
   SEC
   SBC   __local_check__r_1
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__fail
   LDA   #$00
   BNE   .jcmp_skip@6
   LDA   __local_check__fail
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@6:
   LDA   #$02
   STA   __local_check__fail
   LDX   #$00
.if_end@1:
   LDA   #$07
   SEC
   SBC   v4
   TXA
   SBC   v4+1
   TXA
   SBC   v4+2
   TXA
   SBC   v4+3
   TXA
   ROL
   STA   __local_check__r_0
   TXA
   BNE   .thread@0
   LDA   __local_check__r_0
   CMP   v0
   BEQ   .cmp_true@7
.thread@0:
   TXA
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   STA   __local_check__r_1
   TXA
   SEC
   SBC   __local_check__r_1
   TXA
   SBC   #$00
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   TXA
   ROL
   EOR   #$01
   STA   __local_check__r_1
   TXA
   BNE   .jcmp_skip@11
   LDA   __local_check__r_1
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@11:
   LDA   __local_check__fail
   ORA   #$04
   STA   __local_check__fail
.if_end@2:
   LDA   v4
   ORA   v4+1
   BEQ   .if_end@3
   LDA   __local_check__fail
   ORA   #$08
   STA   __local_check__fail
.if_end@3:
   LDA   v3
   EOR   #$FF
   STA   __local_check__r_0
   TXA
   EOR   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   __local_check__r_0
   BEQ   .if_end@4
.jcmp_skip@12:
   LDA   __local_check__fail
   ORA   #$10
   STA   __local_check__fail
.if_end@4:
   LDA   v0
   BEQ   .if_end@5
   LDA   __local_check__fail
   ORA   #$20
   STA   __local_check__fail
.if_end@5:
   LDA   v2
   ORA   v2+1
   BEQ   .if_end@6
   LDA   __local_check__fail
   ORA   #$40
   STA   __local_check__fail
.if_end@6:
   LDA   v2
   ORA   v2+1
   BEQ   .if_end@7
   LDA   __local_check__fail
   ORA   #$80
   STA   __local_check__fail
.if_end@7:
   LDA   __local_check__fail
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail
   LDX   #$00
   JMP   .if_end@1

main:
   SUBROUTINE

   JMP   check

v0:
   DS.B  1

v1:
   DS.B  4

v2:
   DS.B  2

v3:
   DC.B  $FF

v4:
   DS.B  4

v5:
   DC.W  $0001
