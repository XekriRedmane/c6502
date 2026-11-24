SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v2	EQU	$26
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

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item v5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v2 size=1 at=auto def=true init=$01 weight=4
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v2
   STA   __local_check__fail_1
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@0:
   BCC   .mul_skip@1
   CLC
   ADC   __local_check__fail_1
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
   ORA   HARGS
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v3
   ORA   v3+1
   ORA   v3+2
   ORA   v3+3
   ORA   v3+4
   BEQ   .lnot_true@2
   LDA   #$00
   BEQ   .lnot_end@3
.lnot_true@2:
   LDA   #$01
.lnot_end@3:
   STA   __local_check__r_0
   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   STA   __local_check__r_2
   STA   __local_check__r_3
   ORA   __local_check__r_0
   ORA   __local_check__r_2
   ORA   __local_check__r_3
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   v0+3
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   v0+2
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   v0+1
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   v0
   CMP   #$FF
   BEQ   .sx_neg@7
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
   LDA   v0+3
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_check__r_3
   LDA   v0
   EOR   #$FF
   STA   __local_check__5
   LDA   v0+1
   EOR   #$FF
   STA   __local_check__4
   LDA   v0+2
   EOR   #$FF
   STA   __local_check__3
   LDA   v0+3
   EOR   #$FF
   STA   __local_check__2
   LDA   __local_check__r_3
   EOR   #$FF
   STA   __local_check__1
   LDA   v4
   STA   HARGS+4
   LDA   v4+1
   STA   HARGS+5
   LDA   v4+2
   STA   HARGS+6
   LDA   v4+3
   STA   HARGS+7
   LDA   v0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   JSR   sdivmod32
   LDA   HARGS+11
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_check__r_3
   LDA   __local_check__5
   SEC
   SBC   HARGS+8
   STA   __local_check__0
   LDA   __local_check__4
   SBC   HARGS+9
   STA   __local_check__r_0
   LDA   __local_check__3
   SBC   HARGS+10
   STA   __local_check__r_1
   LDA   __local_check__2
   SBC   HARGS+11
   STA   __local_check__r_2
   LDA   __local_check__1
   SBC   __local_check__r_3
   AND   #$03
   EOR   #$02
   SEC
   SBC   #$02
   STA   __local_check__r_3
   LDA   __local_check__0
   ORA   __local_check__r_0
   ORA   __local_check__r_1
   ORA   __local_check__r_2
   ORA   __local_check__r_3
   BEQ   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   v5
   STA   HARGS+4
   LDA   v4
   STA   HARGS
   LDA   v4+1
   STA   HARGS+1
   LDA   v4+2
   STA   HARGS+2
   LDA   v4+3
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   #$07
   SEC
   SBC   HARGS+12
   LDA   #$00
   SBC   HARGS+13
   LDA   #$00
   SBC   HARGS+14
   LDA   #$00
   SBC   HARGS+15
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__r_3
   SEC
   LDA   #$00
   SBC   __local_check__r_3
   STA   __local_check__r_2
   LDA   #$00
   SBC   #$00
   ORA   __local_check__r_2
   BEQ   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   v2
   BNE   .if_end@10
   LDA   v1+1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_check__r_1
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v3+4
   CMP   __local_check__r_3
   BNE   .cmp_true@14
   LDA   v3+3
   CMP   __local_check__r_2
   BNE   .cmp_true@14
   LDA   v3+2
   CMP   __local_check__r_1
   BNE   .cmp_true@14
   LDA   v3+1
   CMP   v1+1
   BNE   .cmp_true@14
   LDA   v3
   CMP   v1
   BNE   .cmp_true@14
   LDA   #$00
   BEQ   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   STA   __local_check__r_1
   LDA   v4
   ORA   v4+1
   ORA   v4+2
   ORA   v4+3
   BEQ   .and_false@8
   LDA   #$01
   STA   __local_check__r_2
   JMP   .vthread@0
.and_false@8:
   LDA   #$00
   STA   __local_check__r_2
   CMP   #$00
   BNE   .if_end@10
.vthread@0:
   LDA   __local_check__r_1
   CMP   __local_check__r_2
   BNE   .if_end@10
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@10:
   LDA   v0+3
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_check__r_3
   LDA   v3
   SEC
   SBC   v0
   STA   __local_check__0
   LDA   v3+1
   SBC   v0+1
   STA   __local_check__r_0
   LDA   v3+2
   SBC   v0+2
   STA   __local_check__r_1
   LDA   v3+3
   SBC   v0+3
   STA   __local_check__r_2
   LDA   v3+4
   SBC   __local_check__r_3
   AND   #$03
   LDX   __local_check__0
   STX   __local_check__5
   LDX   __local_check__r_0
   STX   __local_check__4
   LDX   __local_check__r_1
   STX   __local_check__3
   LDX   __local_check__r_2
   STX   __local_check__2
   EOR   #$02
   SEC
   SBC   #$02
   STA   __local_check__1
   LDA   v0+3
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_check__r_3
   LDA   __local_check__5
   SEC
   SBC   v0
   STA   __local_check__0
   LDA   __local_check__4
   SBC   v0+1
   STA   __local_check__r_0
   LDA   __local_check__3
   SBC   v0+2
   STA   __local_check__r_1
   LDA   __local_check__2
   SBC   v0+3
   STA   __local_check__r_2
   LDA   __local_check__1
   SBC   __local_check__r_3
   AND   #$03
   LDX   __local_check__0
   STX   __local_check__5
   LDX   __local_check__r_0
   STX   __local_check__4
   LDX   __local_check__r_1
   STX   __local_check__3
   LDX   __local_check__r_2
   STX   __local_check__2
   EOR   #$02
   SEC
   SBC   #$02
   STA   __local_check__1
   LDA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v0+3
   CMP   __local_check__r_3
   BNE   .cmp_true@23
   LDA   v0+2
   CMP   __local_check__r_2
   BNE   .cmp_true@23
   LDA   v0+1
   BNE   .cmp_true@23
   LDA   v0
   CMP   v2
   BNE   .cmp_true@23
   LDA   #$00
   BEQ   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   ORA   v4
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   STA   __local_check__r_1
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   __local_check__1
   CMP   __local_check__r_3
   BNE   .if_end@11
   LDA   __local_check__2
   CMP   __local_check__r_2
   BNE   .if_end@11
   LDA   __local_check__3
   CMP   __local_check__r_1
   BNE   .if_end@11
   LDA   __local_check__4
   BNE   .if_end@11
   LDA   __local_check__5
   CMP   __local_check__0
   BNE   .if_end@11
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
   JMP   .if_end@0

main:
   SUBROUTINE

   JMP   check

v0:
   DC.L  $FFFFFFFF

v1:
   DC.W  $0001

v3:
   DC.B  $DF
   DC.B  $7C
   DC.B  $C5
   DC.B  $33
   DC.B  $01

v4:
   DC.L  $695E57A1

v5:
   DC.B  $7F

__zp_init:
   SUBROUTINE

   LDA   #$01
   STA   v2
   RTS
