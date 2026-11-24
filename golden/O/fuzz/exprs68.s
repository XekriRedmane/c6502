SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v0_0	EQU	$80
__zpabi_check__v0_1	EQU	$81
__zpabi_check__v0_2	EQU	$82
__zpabi_check__v0_3	EQU	$83
__zpabi_check__v1_0	EQU	$84
__zpabi_check__v1_1	EQU	$85
__zpabi_check__v1_2	EQU	$86
__zpabi_check__v1_3	EQU	$87
__zpabi_check__v2	EQU	$88
__zpabi_check__v3_0	EQU	$89
__zpabi_check__v3_1	EQU	$8A
__zpabi_check__v4_0	EQU	$8B
__zpabi_check__v4_1	EQU	$8C
__zpabi_check__v4_2	EQU	$8D
__zpabi_check__v4_3	EQU	$8E
__zpabi_check__v5_0	EQU	$8F
__zpabi_check__v5_1	EQU	$90
__local_check__fail_1	EQU	$92
__local_check__fail_0	EQU	$93
__local_check__0	EQU	$94
__local_check__1	EQU	$95
__local_check__2	EQU	$96
__local_check__3	EQU	$97
__local_check__4	EQU	$98

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__0
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   __zpabi_check__v2
   CLC
   ADC   __zpabi_check__v0_0
   STA   __local_check__4
   LDA   __local_check__0
   ADC   __zpabi_check__v0_1
   STA   __local_check__3
   LDA   __local_check__fail_0
   ADC   __zpabi_check__v0_2
   STA   __local_check__2
   LDA   __local_check__fail_1
   ADC   __zpabi_check__v0_3
   STA   __local_check__1
   LDA   __zpabi_check__v3_0
   SEC
   SBC   #$01
   LDA   __zpabi_check__v3_1
   SBC   #$00
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   __local_check__4
   SEC
   SBC   __local_check__0
   LDA   __local_check__3
   SBC   #$00
   LDA   __local_check__2
   SBC   __local_check__fail_0
   LDA   __local_check__1
   SBC   __local_check__fail_1
   LDA   #$00
   ROL
   SEC
   SBC   #$FF
   STA   __local_check__fail_0
   LDA   #$00
   SBC   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_check__fail_0
   CMP   #$02
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@4:
   LDA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@2:
   LDA   __zpabi_check__v3_0
   STA   __local_check__1
   LDA   __zpabi_check__v3_1
   STA   __local_check__2
   LDX   __zpabi_check__v5_0
   BEQ   .shift_done@6
.shift_loop@5:
   LSR   __local_check__2
   ROR   __local_check__1
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   LDA   #$00
   SEC
   SBC   __zpabi_check__v4_0
   LDA   #$01
   SBC   __zpabi_check__v4_1
   LDA   #$00
   SBC   __zpabi_check__v4_2
   LDA   #$00
   SBC   __zpabi_check__v4_3
   LDA   #$00
   ROL
   CLC
   ADC   __local_check__1
   CMP   #$01
   BEQ   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@4:
   LDA   __zpabi_check__v1_3
   BNE   .jcmp_skip@7
   LDA   __zpabi_check__v1_2
   BNE   .jcmp_skip@7
   LDA   __zpabi_check__v1_1
   BNE   .jcmp_skip@7
   LDA   __zpabi_check__v1_0
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@7:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v3_1
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
   LDA   __zpabi_check__v3_0
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   HARGS+3
   LDA   #$45
   STA   HARGS
   LDA   #$23
   STA   HARGS+1
   LDA   __zpabi_check__v3_0
   STA   HARGS+2
   JSR   mul16
   LDA   HARGS+4
   ORA   HARGS+5
   BEQ   .if_end@7
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
   JMP   .if_end@2

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v4_0
   LDA   #$09
   STA   __zpabi_check__v2
   LDA   #$00
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   JMP   check
