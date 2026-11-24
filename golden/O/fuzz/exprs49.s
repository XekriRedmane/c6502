SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v3	EQU	$26
v2	EQU	$28
v5	EQU	$29
__local_check__fail	EQU	$81
__local_check__0	EQU	$82
__local_check__1	EQU	$83
__local_check__2	EQU	$84
__local_check__3	EQU	$85
__local_check__6	EQU	$88
__local_check__7	EQU	$89

; @zp-link-meta-begin
; def check params= locals=__local_check__fail,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v3 size=2 at=auto def=true init=$00,$00 weight=7
; zp v2 size=1 at=auto def=true init=$00 weight=3
; zp v5 size=1 at=auto def=true init=$01 weight=3
; runtime hargs_size=32 hargs_need=1
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v3
   ORA   v3+1
   BNE   .or_true@0
   LDA   v5
   BNE   .or_true@0
   LDA   #$00
   STA   __local_check__fail
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_check__fail
.or_end@1:
   LDA   #$33
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@0:
   BCC   .mul_skip@1
   CLC
   ADC   __local_check__fail
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
   STA   __local_check__2
   LDA   v3
   AND   v3
   TAX
   LDA   #$00
   STA   __local_check__fail
   STA   __local_check__0
   CPX   #$00
   BEQ   .shift_done@3
.shift_loop@2:
   ASL   __local_check__fail
   ROL   __local_check__0
   DEX
   BNE   .shift_loop@2
.shift_done@3:
   LDA   __local_check__2
   CMP   __local_check__0
   BEQ   .lb_skip@1
   JMP   .check@asm_ssa_split@0
.lb_skip@1:
   LDA   HARGS
   CMP   __local_check__fail
   BEQ   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail
   LDY   #$00
.if_end@2:
   LDA   v2
   STA   __local_check__0
   BMI   .sx_neg@4
   TYA
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_check__1
   LDA   v1+1
   BMI   .sx_neg@6
   TYA
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_check__7
   CLC
   ADC   #$80
   STA   __local_check__6
   LDA   v3+1
   BMI   .sx_neg@8
   TYA
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_check__3
   STA   __local_check__2
   LDA   __local_check__6
   CMP   __local_check__2
   BNE   .thread@0
   LDA   __local_check__7
   CMP   __local_check__3
   BNE   .thread@0
   LDA   v1+1
   CMP   v3+1
   BNE   .thread@0
   LDA   v1
   CMP   v3
   BEQ   .cmp_true@10
.thread@0:
   TYA
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   TAX
   BEQ   .shift_done@14
.shift_loop@13:
   ASL   __local_check__0
   ROL   __local_check__1
   DEX
   BNE   .shift_loop@13
.shift_done@14:
   LDA   __local_check__0
   ORA   __local_check__1
   BEQ   .if_end@3
   LDA   __local_check__fail
   ORA   #$02
   STA   __local_check__fail
.if_end@3:
   LDA   v0
   BMI   .sx_neg@15
   TYA
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __local_check__2
   LDA   v0
   BMI   .sx_neg@17
   TYA
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_check__0
   LDA   __local_check__2
   CMP   __local_check__0
   BNE   .cmp_true@19
   LDA   v0
   CMP   v0
   BNE   .cmp_true@19
   TYA
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   STA   __local_check__1
   LDA   v2
   BEQ   .lnot_true@22
   TYA
   JMP   .lnot_end@23
.lnot_true@22:
   LDA   #$01
.lnot_end@23:
   STA   __local_check__0
   LDA   __local_check__1
   EOR   __local_check__0
   STA   __local_check__0
   STY   __local_check__1
   LDA   v1
   SEC
   SBC   v3
   STA   __local_check__3
   LDA   v1+1
   SBC   v3+1
   ORA   __local_check__3
   BEQ   .and_false@4
   LDA   v3
   BEQ   .and_false@4
   LDX   #$01
   BNE   .and_end@5
.and_false@4:
   LDX   #$00
.and_end@5:
   BEQ   .shift_done@25
.shift_loop@24:
   ASL   __local_check__0
   ROL   __local_check__1
   DEX
   BNE   .shift_loop@24
.shift_done@25:
   LDA   __local_check__1
   BNE   .jcmp_skip@26
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@26:
   LDA   __local_check__fail
   ORA   #$04
   STA   __local_check__fail
.if_end@6:
   LDA   v5
   SEC
   SBC   v1
   TYA
   SBC   v1+1
   BVC   .cmp_novf@27
   EOR   #$80
.cmp_novf@27:
   ASL
   TYA
   ROL
   SEC
   SBC   v5
   TYA
   SBC   #$00
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   TYA
   ROL
   STA   __local_check__0
   SEC
   TYA
   SBC   __local_check__0
   STA   __local_check__1
   TYA
   SBC   #$00
   CMP   #$FF
   BNE   .jcmp_skip@29
   LDA   __local_check__1
   CMP   #$FF
   BEQ   .if_end@7
.jcmp_skip@29:
   LDA   __local_check__fail
   ORA   #$08
   STA   __local_check__fail
.if_end@7:
   LDA   v3
   AND   v1
   STA   __local_check__1
   LDA   v3+1
   AND   v1+1
   STA   __local_check__0
   LDA   #$44
   SEC
   SBC   __local_check__1
   TYA
   SBC   __local_check__0
   BVC   .cmp_novf@30
   EOR   #$80
.cmp_novf@30:
   ASL
   TYA
   ROL
   STA   __local_check__2
   LDA   v2
   BMI   .sx_neg@31
   TYA
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   __local_check__0
   CPY   __local_check__0
   BNE   .jcmp_skip@33
   LDA   __local_check__2
   CMP   v2
   BEQ   .if_end@8
.jcmp_skip@33:
   LDA   __local_check__fail
   ORA   #$10
   STA   __local_check__fail
.if_end@8:
   TYA
   TAX
   LDA   __local_check__fail
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail
   LDY   #$00
   JMP   .if_end@2

main:
   SUBROUTINE

   JMP   check

v0:
   DC.B  $58

v1:
   DC.W  $0001

v4:
   DC.B  $01

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   v2
   STA   v3
   STA   v3+1
   LDA   #$01
   STA   v5
   RTS
