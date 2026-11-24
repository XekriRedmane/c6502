SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v2_0	EQU	$82
__zpabi_check__v2_1	EQU	$83
__zpabi_check__v3	EQU	$84
__zpabi_check__v4	EQU	$85
__zpabi_check__v5_0	EQU	$86
__zpabi_check__v5_1	EQU	$87
__local_check__fail_1	EQU	$89
__local_check__fail_0	EQU	$8A
__local_check__0	EQU	$8B
__local_check__1	EQU	$8C
__local_check__2	EQU	$8D
__local_check__3	EQU	$8E
__local_check__4	EQU	$8F
__local_check__5	EQU	$90
__local_check__v0	EQU	$91

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__v0 indirect=false in_cycle=false param_regs=A,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=9
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   LDA   __zpabi_check__v3
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __local_check__v0
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__4
   LDA   __local_check__v0
   CLC
   ADC   #$FF
   STA   __local_check__1
   LDA   __local_check__4
   ADC   #$00
   STA   __local_check__0
   LDA   #$23
   CMP   __local_check__0
   BNE   .thread@0
   LDA   #$45
   CMP   __local_check__1
   BEQ   .cmp_true@2
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   STA   __local_check__0
   LDA   __local_check__v0
   SEC
   SBC   __zpabi_check__v5_0
   LDA   __local_check__4
   SBC   __zpabi_check__v5_1
   LDA   #$00
   ROL
   TAX
   LDA   #$00
   STA   __local_check__1
   CPX   #$00
   BEQ   .shift_done@8
.shift_loop@7:
   ASL   __local_check__0
   ROL   __local_check__1
   DEX
   BNE   .shift_loop@7
.shift_done@8:
   LDA   __local_check__0
   ORA   __local_check__1
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v4
   SEC
   SBC   __zpabi_check__v5_0
   LDA   #$00
   SBC   __zpabi_check__v5_1
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@9
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v2_0
   EOR   __zpabi_check__v5_0
   STA   __local_check__2
   LDA   __zpabi_check__v2_1
   EOR   __zpabi_check__v5_1
   STA   __local_check__1
   LDA   __zpabi_check__v2_0
   ORA   __zpabi_check__v2_1
   BEQ   .lnot_true@10
   LDA   #$00
   BEQ   .lnot_end@11
.lnot_true@10:
   LDA   #$01
.lnot_end@11:
   STA   __local_check__0
   LDA   __local_check__2
   EOR   __local_check__0
   STA   __local_check__3
   LDA   __local_check__1
   STA   __local_check__2
   LDA   __zpabi_check__v4
   BEQ   .and_false@3
   LDA   #$00
   CMP   __local_check__4
   BNE   .and_false@3
   LDA   #$02
   CMP   __local_check__v0
   BNE   .and_false@3
   LDA   #$01
   STA   __local_check__1
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   STA   __local_check__1
.and_end@4:
   LDA   __local_check__3
   CLC
   SBC   __local_check__1
   LDA   __local_check__2
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@12
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@12:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v2_1
   CMP   #$80
   BNE   .jcmp_skip@13
   LDA   __zpabi_check__v2_0
   BEQ   .vthread@0
.jcmp_skip@13:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
   LDA   __zpabi_check__v2_0
.vthread@0:
   STA   HARGS
   LDA   __zpabi_check__v2_1
   STA   HARGS+1
   LDA   __zpabi_check__v1_0
   STA   HARGS+2
   LDA   __zpabi_check__v1_1
   STA   HARGS+3
   JSR   udivmod16
   LDA   __zpabi_check__v2_1
   ASL
   LDA   #$00
   ROL
   STA   __local_check__4
   LDA   #$38
   EOR   __zpabi_check__v4
   SEC
   SBC   __local_check__4
   LDA   #$3E
   SBC   #$00
   LDA   #$D5
   SBC   #$00
   LDA   #$4E
   SBC   #$00
   LDA   #$00
   ROL
   CLC
   ADC   HARGS+4
   STA   __local_check__1
   LDA   #$00
   ADC   #$00
   BNE   .jcmp_skip@14
   LDA   __local_check__1
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@14:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __local_check__v0
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __local_check__5
   STA   __local_check__4
   STA   __local_check__3
   LDA   __zpabi_check__v5_1
   BNE   .thread@1
   LDA   __zpabi_check__v5_0
   CMP   #$01
   BEQ   .cmp_true@17
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__v0
   SEC
   SBC   __local_check__2
   LDA   __local_check__5
   SBC   #$00
   LDA   __local_check__4
   SBC   __local_check__1
   LDA   __local_check__3
   SBC   __local_check__0
   BCS   .if_end@9
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
   STA   __zpabi_check__v1_0
   LDA   #$80
   STA   __zpabi_check__v2_1
   LDA   #$00
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v3
   STA   __zpabi_check__v4
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   LDA   #$FF
   JMP   check
