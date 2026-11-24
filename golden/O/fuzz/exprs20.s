SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1	EQU	$80
__zpabi_check__v2_0	EQU	$81
__zpabi_check__v2_1	EQU	$82
__zpabi_check__v3	EQU	$83
__zpabi_check__v4_0	EQU	$84
__zpabi_check__v4_1	EQU	$85
__zpabi_check__v4_2	EQU	$86
__zpabi_check__v4_3	EQU	$87
__zpabi_check__v4_4	EQU	$88
__zpabi_check__v4_5	EQU	$89
__zpabi_check__v4_6	EQU	$8A
__zpabi_check__v4_7	EQU	$8B
__zpabi_check__v5_0	EQU	$8C
__zpabi_check__v5_1	EQU	$8D
__local_check__fail_1	EQU	$8F
__local_check__fail_0	EQU	$90
__local_check__0	EQU	$91
__local_check__1	EQU	$92
__local_check__2	EQU	$93
__local_check__3	EQU	$94
__local_check__4	EQU	$95
__local_check__5	EQU	$96
__local_check__6	EQU	$97
__local_check__7	EQU	$98
__local_check__8	EQU	$99
__local_check__v0_1	EQU	$9A
__local_check__v0_0	EQU	$9B

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v4_4,__zpabi_check__v4_5,__zpabi_check__v4_6,__zpabi_check__v4_7,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__v0_1,__local_check__v0_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0_0
   STX   __local_check__v0_1
   SEC
   LDA   #$00
   SBC   __zpabi_check__v5_0
   STA   __local_check__fail_0
   LDA   #$00
   SBC   __zpabi_check__v5_1
   ORA   __local_check__fail_0
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v3
   SEC
   SBC   #$02
   LDA   #$00
   ROL
   SEC
   SBC   __zpabi_check__v5_0
   LDA   #$00
   SBC   __zpabi_check__v5_1
   BCS   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v5_0
   ORA   __zpabi_check__v5_1
   BEQ   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v3
   CMP   #$80
   BEQ   .vthread@0
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
   LDA   __zpabi_check__v3
.vthread@0:
   STA   __local_check__8
   LDX   __zpabi_check__v1
   BEQ   .shift_done@1
.shift_loop@0:
   LSR   __local_check__8
   DEX
   BNE   .shift_loop@0
.shift_done@1:
   LDA   __zpabi_check__v4_0
   STA   HARGS
   LDA   __zpabi_check__v4_1
   STA   HARGS+1
   LDA   __zpabi_check__v4_2
   STA   HARGS+2
   LDA   __zpabi_check__v4_3
   STA   HARGS+3
   LDA   __zpabi_check__v4_4
   STA   HARGS+4
   LDA   __zpabi_check__v4_5
   STA   HARGS+5
   LDA   __zpabi_check__v4_6
   STA   HARGS+6
   LDA   __zpabi_check__v4_7
   STA   HARGS+7
   LDA   __zpabi_check__v1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   __local_check__8
   CLC
   SBC   HARGS+16
   LDA   #$00
   SBC   HARGS+17
   LDA   #$00
   SBC   HARGS+18
   LDA   #$00
   SBC   HARGS+19
   LDA   #$00
   SBC   HARGS+20
   LDA   #$00
   SBC   HARGS+21
   LDA   #$00
   SBC   HARGS+22
   LDA   #$00
   SBC   HARGS+23
   LDA   #$00
   ROL
   STA   __local_check__6
   LDA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   STA   __local_check__5
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v4_0
   CLC
   ADC   __local_check__6
   STA   __local_check__7
   LDA   __zpabi_check__v4_1
   ADC   #$00
   STA   __local_check__6
   LDA   __zpabi_check__v4_2
   ADC   __local_check__5
   STA   __local_check__5
   LDA   __zpabi_check__v4_3
   ADC   __local_check__4
   STA   __local_check__4
   LDA   __zpabi_check__v4_4
   ADC   __local_check__3
   STA   __local_check__3
   LDA   __zpabi_check__v4_5
   ADC   __local_check__2
   STA   __local_check__2
   LDA   __zpabi_check__v4_6
   ADC   __local_check__1
   STA   __local_check__1
   LDA   __zpabi_check__v4_7
   ADC   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_check__1
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_check__2
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_check__3
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_check__4
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_check__5
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_check__6
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_check__7
   CMP   #$FF
   BEQ   .if_end@7
.jcmp_skip@4:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v3
   AND   #$3F
   BEQ   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   #$00
   CMP   __zpabi_check__v4_7
   BNE   .cmp_true@5
   CMP   __zpabi_check__v4_6
   BNE   .cmp_true@5
   CMP   __zpabi_check__v4_5
   BNE   .cmp_true@5
   CMP   __zpabi_check__v4_4
   BNE   .cmp_true@5
   CMP   __zpabi_check__v4_3
   BNE   .cmp_true@5
   CMP   __zpabi_check__v4_2
   BNE   .cmp_true@5
   LDA   #$01
   CMP   __zpabi_check__v4_1
   BNE   .cmp_end@6
   LDA   #$00
   CMP   __zpabi_check__v4_0
   BEQ   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   STA   __local_check__2
   SEC
   LDA   #$00
   SBC   __local_check__v0_0
   STA   __local_check__1
   LDA   #$00
   SBC   __local_check__v0_1
   STA   __local_check__0
   LDA   #$00
   CMP   __local_check__0
   BNE   .cmp_end@9
   LDA   __local_check__2
   CMP   __local_check__1
   BEQ   .cmp_true@8
   LDA   #$00
   BEQ   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   AND   #$3F
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

   LDA   #$01
   STA   __zpabi_check__v1
   LDA   #$FF
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   STA   __zpabi_check__v4_4
   STA   __zpabi_check__v4_5
   STA   __zpabi_check__v4_6
   STA   __zpabi_check__v4_7
   LDA   #$80
   STA   __zpabi_check__v3
   LDA   #$00
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   LDX   #$00
   JMP   check
