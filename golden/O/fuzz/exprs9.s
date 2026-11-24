SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r	EQU	$83
__local_check__0	EQU	$84
__local_check__1	EQU	$85
__local_check__2	EQU	$86
__local_check__3	EQU	$87
__local_check__4	EQU	$88
__local_check__5	EQU	$89
__local_check__6	EQU	$8A
__local_check__7	EQU	$8B
__local_check__8	EQU	$8C

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.bss local=false zero=true
; item v2 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.bss local=false zero=true
; item v5 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v5
   EOR   #$FF
   STA   __local_check__fail_0
   LDA   v5+1
   EOR   #$FF
   STA   __local_check__0
   LDA   v2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__r
   LDA   v2
   SEC
   SBC   #$80
   STA   __local_check__fail_1
   BCS   .carry_inc@0
   DEC   __local_check__r
.carry_inc@0:
   LDA   __local_check__fail_0
   ORA   __local_check__fail_1
   STA   __local_check__fail_0
   LDA   __local_check__0
   ORA   __local_check__r
   STA   __local_check__fail_1
   LDA   v1
   CLC
   ADC   __local_check__fail_0
   STA   __local_check__fail_0
   LDA   #$00
   ADC   __local_check__fail_1
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_check__fail_0
   CMP   #$7F
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@2:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   CLC
   SBC   v0
   LDA   #$00
   SBC   v0+1
   BCC   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   v0
   STA   HARGS
   LDA   v0+1
   STA   HARGS+1
   LDA   v5
   STA   HARGS+2
   LDA   v5+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   ORA   HARGS+5
   BNE   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   #$6F
   CLC
   SBC   v5
   LDA   #$00
   SBC   v5+1
   LDA   #$00
   ROL
   STA   __local_check__8
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   __local_check__7
   STA   __local_check__6
   LDA   #$DD
   ORA   v3
   STA   __local_check__5
   LDA   #$FF
   ORA   v3+1
   STA   __local_check__4
   LDA   #$FF
   ORA   v3+2
   STA   __local_check__3
   LDA   #$FF
   ORA   v3+3
   STA   __local_check__2
   LDA   v0
   SEC
   SBC   v4
   LDA   v0+1
   SBC   v4+1
   LDA   #$00
   SBC   v4+2
   LDA   #$00
   SBC   v4+3
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__1
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   __local_check__0
   STA   __local_check__r
   LDA   __local_check__5
   CLC
   ADC   __local_check__1
   STA   __local_check__1
   LDA   __local_check__4
   ADC   #$00
   STA   __local_check__4
   LDA   __local_check__3
   ADC   __local_check__0
   STA   __local_check__0
   LDA   __local_check__2
   ADC   __local_check__r
   STA   __local_check__r
   LDA   __local_check__8
   SEC
   SBC   __local_check__1
   LDA   #$00
   SBC   __local_check__4
   LDA   __local_check__7
   SBC   __local_check__0
   LDA   __local_check__6
   SBC   __local_check__r
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__r
   LDA   #$00
   BNE   .jcmp_skip@9
   LDA   __local_check__r
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   v2
   CMP   #$FF
   BEQ   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   v3+1
   BNE   .jcmp_skip@10
   LDA   v3
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
   LDA   v4
   EOR   #$FF
   STA   __local_check__2
   LDA   v4+1
   EOR   #$FF
   STA   __local_check__1
   LDA   v4+2
   EOR   #$FF
   STA   __local_check__0
   LDA   v4+3
   EOR   #$FF
   STA   __local_check__r
   LDA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   ORA   __local_check__r
   BEQ   .check@ssa_block@10
   LDA   v1
   BEQ   .if_end@9
.check@ssa_block@10:
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
   DS.B  1

v2:
   DC.B  $FF

v3:
   DC.L  $00000001

v4:
   DS.B  4

v5:
   DC.W  $FFFF
