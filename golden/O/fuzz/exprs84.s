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
__local_check__9	EQU	$8D

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.bss local=false zero=true
; item v2 section=.data local=false zero=false
; item v3 section=.bss local=false zero=true
; item v4 section=.data local=false zero=false
; item v5 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=9
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v5
   STA   __local_check__r
   LDA   v5+1
   STA   __local_check__0
   ASL
   ROR   __local_check__0
   ROR   __local_check__r
   SEC
   LDA   #$00
   SBC   __local_check__r
   STA   __local_check__fail_0
   LDA   #$00
   SBC   __local_check__0
   BNE   .jcmp_skip@0
   LDA   __local_check__fail_0
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@3:
   LDA   v3
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_check__5
   LDA   v2
   CLC
   SBC   v2
   LDA   v2+1
   SBC   v2+1
   LDA   v2+2
   SBC   v2+2
   LDA   v2+3
   SBC   v2+3
   LDA   #$00
   ROL
   STA   __local_check__r
   LDA   v1
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   LDA   v1
   SEC
   SBC   v0
   LDA   __local_check__3
   SBC   v0+1
   LDA   __local_check__2
   SBC   v0+2
   LDA   __local_check__1
   SBC   v0+3
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   TAX
   LDA   #$00
   STA   __local_check__0
   CPX   #$00
   BEQ   .shift_done@7
.shift_loop@6:
   ASL   __local_check__r
   ROL   __local_check__0
   DEX
   BNE   .shift_loop@6
.shift_done@7:
   LDA   v3
   SEC
   SBC   __local_check__r
   LDA   __local_check__5
   SBC   __local_check__0
   BVC   .jcmp_novf@8
   EOR   #$80
.jcmp_novf@8:
   BPL   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@4:
   LDA   v0+1
   EOR   #$01
   CMP   #$F5
   BNE   .jcmp_skip@9
   LDA   v0
   CMP   #$1D
   BEQ   .if_end@5
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   #$02
   CLC
   SBC   v4
   LDA   #$00
   SBC   v4+1
   LDA   #$00
   SBC   v4+2
   LDA   #$00
   SBC   v4+3
   LDA   #$00
   ROL
   STA   __local_check__r
   LDA   #$00
   BNE   .jcmp_skip@10
   LDA   __local_check__r
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   v1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   CMP   v0+3
   BNE   .thread@0
   CMP   v0+2
   BNE   .thread@0
   CMP   v0+1
   BNE   .thread@0
   LDA   v1
   CMP   v0
   BEQ   .cmp_true@13
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   __local_check__3
   LDA   #$00
   CLC
   SBC   v5
   LDA   #$00
   SBC   v5+1
   LDA   #$00
   SBC   v5+2
   LDA   #$00
   SBC   v5+3
   LDA   #$00
   ROL
   SEC
   SBC   __local_check__3
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__9
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   __local_check__7
   LDA   v2
   ORA   v2+1
   ORA   v2+2
   ORA   v2+3
   BEQ   .and_false@7
   LDA   #$01
   STA   __local_check__6
   LDA   #$00
   BEQ   .and_end@8
.and_false@7:
   LDA   #$00
   STA   __local_check__6
.and_end@8:
   ORA   #$00
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   STA   __local_check__4
   STA   __local_check__3
   LDA   __local_check__6
   EOR   v2
   STA   __local_check__2
   LDA   #$00
   EOR   v2+1
   STA   __local_check__1
   LDA   __local_check__4
   EOR   v2+2
   STA   __local_check__0
   LDA   __local_check__3
   EOR   v2+3
   STA   __local_check__r
   LDA   __local_check__7
   CMP   __local_check__r
   BNE   .cmp_true@21
   CMP   __local_check__0
   BNE   .cmp_true@21
   LDA   #$00
   CMP   __local_check__1
   BNE   .cmp_true@21
   LDA   __local_check__9
   CMP   __local_check__2
   BNE   .cmp_true@21
   LDA   #$00
   BEQ   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   STA   __local_check__r
   LDA   #$00
   BNE   .jcmp_skip@24
   LDA   __local_check__r
   CMP   #$01
   BEQ   .if_end@9
.jcmp_skip@24:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   LDA   v3
   BEQ   .if_end@10
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@10:
   LDA   #$01
   SEC
   SBC   v2
   STA   __local_check__3
   LDA   #$00
   SBC   v2+1
   STA   __local_check__2
   LDA   #$00
   SBC   v2+2
   STA   __local_check__1
   LDA   #$00
   SBC   v2+3
   STA   __local_check__0
   LDA   v0
   SEC
   SBC   __local_check__3
   LDA   #$00
   SBC   __local_check__2
   LDA   #$00
   SBC   __local_check__1
   LDA   #$00
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   __local_check__5
   LDA   #$00
   BPL   .sx_done@26
   LDA   #$FF
.sx_done@26:
   STA   __local_check__4
   STA   __local_check__3
   LDA   v4
   CLC
   SBC   #$07
   LDA   v4+1
   SBC   #$00
   LDA   v4+2
   SBC   #$00
   LDA   v4+3
   SBC   #$00
   LDA   #$00
   ROL
   STA   HARGS+4
   LDA   v4
   STA   HARGS
   LDA   v4+1
   STA   HARGS+1
   LDA   v4+2
   STA   HARGS+2
   LDA   v4+3
   STA   HARGS+3
   JSR   lsr32
   LDA   HARGS+5
   SEC
   SBC   __local_check__5
   LDA   HARGS+6
   SBC   #$00
   LDA   HARGS+7
   SBC   __local_check__4
   LDA   HARGS+8
   SBC   __local_check__3
   BCS   .if_end@11
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@11:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@3

main:
   SUBROUTINE

   JMP   check

v0:
   DC.L  $E9FFF41D

v1:
   DS.B  1

v2:
   DC.L  $FFFFFFFF

v3:
   DS.B  1

v4:
   DC.L  $00000001

v5:
   DC.L  $FFFFFFFF
