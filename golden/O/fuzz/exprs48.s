SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_3	EQU	$83
__local_check__r_2	EQU	$84
__local_check__r_1	EQU	$85

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.data local=false zero=false
; item v3 section=.bss local=false zero=true
; item v4 section=.data local=false zero=false
; item v5 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   SEC
   LDA   #$00
   SBC   v0
   STA   __local_check__r_2
   LDA   #$00
   SBC   v0+1
   STA   __local_check__r_3
   LDA   #$00
   SBC   v0+2
   STA   __local_check__fail_0
   LDA   #$00
   SBC   v0+3
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_check__fail_0
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_check__r_3
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_check__r_2
   CMP   #$FF
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v2
   EOR   #$FF
   STA   __local_check__r_2
   LDA   #$00
   EOR   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   __local_check__r_2
   BEQ   .if_end@1
.jcmp_skip@1:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   v0+3
   BNE   .jcmp_skip@2
   LDA   v0+2
   BNE   .jcmp_skip@2
   LDA   v0+1
   BNE   .jcmp_skip@2
   LDA   v0
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@2:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v3
   EOR   v0
   STA   HARGS+4
   LDA   #$00
   EOR   v0+1
   STA   HARGS+5
   LDA   __local_check__r_2
   EOR   v0+2
   STA   HARGS+6
   LDA   __local_check__r_3
   EOR   v0+3
   STA   HARGS+7
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STA   HARGS+3
   JSR   sdivmod32
   LDA   v0
   SEC
   SBC   #$79
   LDA   v0+1
   SBC   #$6B
   LDA   v0+2
   SBC   #$00
   LDA   v0+3
   SBC   #$00
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__r_2
   LDA   v5
   SEC
   SBC   __local_check__r_2
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__r_1
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   HARGS+15
   CMP   __local_check__r_3
   BNE   .cmp_true@9
   LDA   HARGS+14
   CMP   __local_check__r_2
   BNE   .cmp_true@9
   LDA   HARGS+13
   BNE   .cmp_true@9
   LDA   HARGS+12
   CMP   __local_check__r_1
   BNE   .cmp_true@9
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   __local_check__r_3
   LDA   #$00
   BNE   .jcmp_skip@12
   LDA   __local_check__r_3
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@12:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   v3
   ORA   #$02
   EOR   #$45
   CMP   #$47
   BEQ   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   #$07
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   LDA   v1
   STA   HARGS+4
   LDA   v1+1
   STA   HARGS+5
   LDA   v1+2
   STA   HARGS+6
   LDA   v1+3
   STA   HARGS+7
   JSR   udivmod32
   LDA   #$01
   SEC
   SBC   HARGS+12
   LDA   #$00
   SBC   HARGS+13
   LDA   #$00
   SBC   HARGS+14
   LDA   #$00
   SBC   HARGS+15
   LDA   #$00
   ROL
   STA   __local_check__r_3
   LDA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_check__r_3
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$7F
   STY   HARGS+4
   LDY   #$00
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+15
   BNE   .jcmp_skip@15
   LDA   HARGS+14
   BNE   .jcmp_skip@15
   LDA   HARGS+13
   BNE   .jcmp_skip@15
   LDA   HARGS+12
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@15:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@7:
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
   DC.L  $00000001

v1:
   DC.L  $00000001

v2:
   DC.B  $FF

v3:
   DS.B  1

v4:
   DC.B  $0A

v5:
   DC.B  $01
