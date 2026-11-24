SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_3	EQU	$83
__local_check__r_2	EQU	$84
__local_check__r_1	EQU	$85
__local_check__r_0	EQU	$86
__local_check__0	EQU	$87
__local_check__1	EQU	$88
__local_check__2	EQU	$89
__local_check__3	EQU	$8A
__local_check__4	EQU	$8B
__local_check__5	EQU	$8C
__local_check__6	EQU	$8D
__local_check__7	EQU	$8E
__local_check__8	EQU	$8F
__local_check__9	EQU	$90

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12,__local_check__13,__local_check__14 indirect=false in_cycle=false
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
; runtime hargs_size=32 hargs_need=23
; @zp-link-meta-end

check:
   SUBROUTINE

   SEC
   LDA   v3+1
   SBC   #$80
   LDA   v3+2
   SBC   #$00
   LDA   v3+3
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__r_0
   LDA   #$00
   BPL   .sx_done@2
   LDA   #$FF
.sx_done@2:
   LDX   __local_check__r_0
   STX   __local_check__7
   STA   __local_check__5
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   AND   #$7F
   STA   __local_check__1
   LDA   #$80
   CLC
   ADC   v0
   STA   __local_check__0
   LDA   #$FF
   ADC   v0+1
   STA   __local_check__r_0
   LDA   #$FF
   ADC   v0+2
   STA   __local_check__r_1
   LDA   #$FF
   ADC   v0+3
   STA   __local_check__r_2
   LDA   #$FF
   ADC   v0+4
   STA   __local_check__r_3
   LDA   #$FF
   ADC   v0+5
   STA   __local_check__fail_0
   LDA   #$7F
   ADC   v0+6
   AND   #$7F
   STA   __local_check__fail_1
   LDA   __local_check__7
   AND   __local_check__0
   STA   __local_check__8
   LDA   #$00
   AND   __local_check__r_0
   STA   __local_check__7
   LDA   __local_check__5
   AND   __local_check__r_1
   STA   __local_check__6
   LDA   __local_check__4
   AND   __local_check__r_2
   STA   __local_check__5
   LDA   __local_check__3
   AND   __local_check__r_3
   STA   __local_check__4
   LDA   __local_check__2
   AND   __local_check__fail_0
   STA   __local_check__3
   LDA   __local_check__1
   AND   __local_check__fail_1
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   HARGS+2
   STA   HARGS+3
   LDA   v2
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   v3
   STA   HARGS+4
   LDA   v3+1
   STA   HARGS+5
   LDA   v3+2
   STA   HARGS+6
   LDA   v3+3
   STA   HARGS+7
   JSR   mul32
   LDA   v4
   EOR   v4
   STA   __local_check__r_2
   LDA   v4+1
   EOR   v4+1
   STA   __local_check__r_3
   LDA   v4+2
   EOR   v4+2
   STA   __local_check__fail_0
   LDA   v4+3
   EOR   v4+3
   STA   __local_check__fail_1
   LDA   HARGS+8
   SEC
   SBC   __local_check__r_2
   LDA   HARGS+9
   SBC   __local_check__r_3
   LDA   HARGS+10
   SBC   __local_check__fail_0
   LDA   HARGS+11
   SBC   __local_check__fail_1
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__r_0
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   __local_check__r_1
   STA   __local_check__r_2
   STA   __local_check__r_3
   STA   __local_check__fail_0
   LDA   __local_check__r_0
   STA   __local_check__0
   LDA   __local_check__r_1
   AND   #$7F
   STA   __local_check__fail_1
   LDA   __local_check__2
   CMP   __local_check__fail_1
   BEQ   .lb_skip@6
   JMP   .check@asm_ssa_split@0
.lb_skip@6:
   LDA   __local_check__3
   CMP   __local_check__fail_0
   BEQ   .lb_skip@5
   JMP   .check@asm_ssa_split@0
.lb_skip@5:
   LDA   __local_check__4
   CMP   __local_check__r_3
   BEQ   .lb_skip@4
   JMP   .check@asm_ssa_split@0
.lb_skip@4:
   LDA   __local_check__5
   CMP   __local_check__r_2
   BEQ   .lb_skip@3
   JMP   .check@asm_ssa_split@0
.lb_skip@3:
   LDA   __local_check__6
   CMP   __local_check__r_1
   BEQ   .lb_skip@2
   JMP   .check@asm_ssa_split@0
.lb_skip@2:
   LDA   __local_check__7
   BEQ   .lb_skip@1
   JMP   .check@asm_ssa_split@0
.lb_skip@1:
   LDA   __local_check__8
   CMP   __local_check__0
   BEQ   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   SEC
   SBC   v1
   STA   __local_check__2
   LDA   #$00
   SBC   v1+1
   STA   __local_check__1
   LDA   #$00
   SBC   v1+2
   STA   __local_check__0
   LDA   #$00
   SBC   v1+3
   STA   __local_check__r_0
   LDA   #$00
   SBC   v1+4
   STA   __local_check__r_1
   LDA   #$00
   SBC   v1+5
   STA   __local_check__r_2
   LDA   #$00
   SBC   v1+6
   AND   #$7F
   STA   __local_check__r_3
   SEC
   LDA   #$00
   SBC   __local_check__2
   STA   __local_check__2
   LDA   #$00
   SBC   __local_check__1
   STA   __local_check__1
   LDA   #$00
   SBC   __local_check__0
   STA   __local_check__0
   LDA   #$00
   SBC   __local_check__r_0
   STA   __local_check__r_0
   LDA   #$00
   SBC   __local_check__r_1
   STA   __local_check__r_1
   LDA   #$00
   SBC   __local_check__r_2
   STA   __local_check__r_2
   LDA   #$00
   SBC   __local_check__r_3
   LDX   __local_check__2
   STX   __local_check__9
   LDX   __local_check__1
   STX   __local_check__8
   LDX   __local_check__0
   STX   __local_check__7
   LDX   __local_check__r_0
   STX   __local_check__6
   LDX   __local_check__r_1
   STX   __local_check__5
   LDX   __local_check__r_2
   STX   __local_check__4
   AND   #$7F
   STA   __local_check__3
   LDA   #$7F
   SEC
   SBC   v3
   STA   __local_check__r_0
   LDA   #$00
   SBC   v3+1
   STA   __local_check__r_1
   LDA   #$00
   SBC   v3+2
   STA   __local_check__r_2
   LDA   #$00
   SBC   v3+3
   STA   __local_check__r_3
   SEC
   LDA   #$00
   SBC   __local_check__r_0
   STA   __local_check__2
   LDA   #$00
   SBC   __local_check__r_1
   STA   __local_check__1
   LDA   #$00
   SBC   __local_check__r_2
   STA   __local_check__0
   LDA   #$00
   SBC   __local_check__r_3
   STA   __local_check__r_0
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_check__r_1
   STA   __local_check__r_2
   AND   #$7F
   STA   __local_check__r_3
   LDA   __local_check__2
   SEC
   SBC   __local_check__9
   LDA   __local_check__1
   SBC   __local_check__8
   LDA   __local_check__0
   SBC   __local_check__7
   LDA   __local_check__r_0
   SBC   __local_check__6
   LDA   __local_check__r_1
   SBC   __local_check__5
   LDA   __local_check__r_2
   SBC   __local_check__4
   LDA   __local_check__r_3
   SBC   __local_check__3
   BCS   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@3:
   LDX   v3
   LDA   #$02
   STA   __local_check__r_1
   LDA   #$00
   STA   __local_check__r_0
   CPX   #$00
   BEQ   .shift_done@11
.shift_loop@10:
   ASL   __local_check__r_1
   ROL   __local_check__r_0
   DEX
   BNE   .shift_loop@10
.shift_done@11:
   LDA   __local_check__r_0
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   __local_check__r_1
   SEC
   SBC   v3
   LDA   __local_check__r_0
   SBC   v3+1
   LDA   __local_check__r_2
   SBC   v3+2
   LDA   __local_check__r_3
   SBC   v3+3
   BVC   .jcmp_novf@14
   EOR   #$80
.jcmp_novf@14:
   BPL   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   v3+3
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __local_check__r_1
   STA   __local_check__r_2
   AND   #$7F
   STA   __local_check__r_3
   LDA   #$00
   CMP   __local_check__r_3
   BNE   .cmp_end@18
   CMP   __local_check__r_2
   BNE   .cmp_end@18
   CMP   __local_check__r_1
   BNE   .cmp_end@18
   CMP   v3+3
   BNE   .cmp_end@18
   CMP   v3+2
   BNE   .cmp_end@18
   CMP   v3+1
   BNE   .cmp_end@18
   LDA   #$80
   CMP   v3
   BEQ   .cmp_true@17
   LDA   #$00
   BEQ   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   STA   __local_check__1
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   LDX   __local_check__1
   STX   HARGS+8
   LDX   #$00
   STX   HARGS+9
   STA   HARGS+10
   STA   HARGS+11
   STA   HARGS+12
   STA   HARGS+13
   AND   #$07
   STA   HARGS+14
   LDA   #$02
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STY   HARGS+6
   STY   HARGS+7
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+22
   AND   #$07
   STA   __local_check__r_3
   LDA   HARGS+16
   ORA   HARGS+17
   ORA   HARGS+18
   ORA   HARGS+19
   ORA   HARGS+20
   ORA   HARGS+21
   ORA   __local_check__r_3
   BEQ   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   v4+3
   BNE   .jcmp_skip@22
   LDA   v4+2
   BNE   .jcmp_skip@22
   LDA   v4+1
   BNE   .jcmp_skip@22
   LDA   v4
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@22:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
   LDA   #$FF
   CMP   v2
   BEQ   .cmp_true@23
   LDA   #$00
   BEQ   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   STA   __local_check__r_1
   LDA   v4
   AND   v3
   TAX
   LDA   #$00
   STA   __local_check__r_0
   CPX   #$00
   BEQ   .shift_done@26
.shift_loop@25:
   LDA   __local_check__r_0
   ASL
   ROR   __local_check__r_0
   ROR   __local_check__r_1
   DEX
   BNE   .shift_loop@25
.shift_done@26:
   LDA   __local_check__r_1
   EOR   #$FF
   STA   __local_check__r_2
   LDA   __local_check__r_0
   EOR   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@27
   LDA   __local_check__r_2
   CMP   #$FF
   BEQ   .if_end@7
.jcmp_skip@27:
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
   DS.B  6
   DC.B  $40

v1:
   DS.B  7

v2:
   DC.B  $0A

v3:
   DS.B  4

v4:
   DC.L  $00000001

v5:
   DC.B  $0A
