SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__zpabi_f0__p1_0	EQU	$80
__zpabi_f0__p1_1	EQU	$81
__zpabi_f0__p2_0	EQU	$82
__zpabi_f0__p2_1	EQU	$83
__zpabi_f0__p2_2	EQU	$84
__zpabi_f0__p2_3	EQU	$85
__local_main__0	EQU	$86
__local_main__2	EQU	$88
__local_main__3	EQU	$89
__local_f0__p0_1	EQU	$8D
__local_f0__p0_0	EQU	$8E
__local_f0__1	EQU	$90

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p2_0,__zpabi_f0__p2_1,__zpabi_f0__p2_2,__zpabi_f0__p2_3 locals=__local_f0__p0_1,__local_f0__p0_0,__local_f0__0,__local_f0__1 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params= locals=__local_f2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$11 weight=7
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f2:
   SUBROUTINE

   ; prologue: 5 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   LDA   g0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod16
   LDA   HARGS+4

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$07
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

f1:
   SUBROUTINE

   ; prologue: 8 arg bytes, 8 local bytes, 8 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$09
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1
   LDA   $C0
   LDY   #$01
   STA   (FP),Y
   LDA   $C1
   INY
   STA   (FP),Y
   LDA   $C2
   INY
   STA   (FP),Y
   LDA   $C3
   INY
   STA   (FP),Y
   LDA   $C4
   INY
   STA   (FP),Y
   LDA   $C5
   INY
   STA   (FP),Y
   LDA   $C6
   INY
   STA   (FP),Y
   LDA   $C7
   INY
   STA   (FP),Y

   LDY   #$0B
   LDA   (FP),Y
   SEC
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $82
   LDA   #$00
   BPL   .sx_done@2
   LDA   #$FF
.sx_done@2:
   STA   $81
   STA   $80
   LDA   $82
   SEC
   LDY   #$0F
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   $81
   INY
   SBC   (FP),Y
   LDA   $80
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $C7
   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   STA   $81
   STA   $80
   LDY   #$0B
   LDA   (FP),Y
   SEC
   SBC   $C7
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   $81
   INY
   LDA   (FP),Y
   SBC   $80
   LDA   #$00
   ROL
   STA   $C0
   LDY   #$0B
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDA   #$FF
   STA   HARGS
   LDA   #$7F
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   STA   HARGS+3
   JSR   sdivmod32
   LDA   HARGS+8
   STA   $87
   LDA   HARGS+9
   STA   $86
   LDA   HARGS+10
   STA   $85
   LDA   HARGS+11
   STA   $84
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@6:
   BCC   .mul_skip@7
   CLC
   ADC   $C0
.mul_skip@7:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@6
   STA   $83
   LDA   HARGS
   STA   $82
   LDA   $83
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   $81
   STA   $80
   LDA   $84
   CMP   $80
   BNE   .thread@0
   LDA   $85
   CMP   $81
   BNE   .thread@0
   LDA   $86
   CMP   $83
   BNE   .thread@0
   LDA   $87
   CMP   $82
   BEQ   .cmp_true@10
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   $C6
   LDA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   STA   $C5
   STA   $C4
   LDA   #<f2
   STA   $C2
   LDA   #>f2
   STA   $C3
   LDA   $C0
   SEC
   SBC   $C6
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   $C5
   LDA   #$00
   SBC   $C4
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   STA   $C1
   LDA   g0
   STA   $C0
   LDY   #$0B
   LDA   (FP),Y
   AND   #$07
   STA   HARGS+4
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   asr32
   LDA   HARGS+5
   STA   $81
   LDA   HARGS+6
   STA   $80
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C1
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $C0
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $C2
   STA   DPTR
   LDA   $C3
   STA   DPTR+1
   JSR   icall
   STA   $80
   LDA   $C7
   EOR   $80
   CLC
   ADC   g0
   STA   g0
   LDA   $C6
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDA   $C5
   STA   HARGS+10
   LDA   $C4
   STA   HARGS+11

   ; epilogue
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0_0
   LDA   g0
   BEQ   .and_false@2
   TXA
   ORA   __local_f0__p0_0
   BEQ   .and_false@2
   LDA   #$01
   STA   __local_f0__1
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_f0__1
.and_end@3:
   LDA   __zpabi_f0__p1_0
   ORA   __zpabi_f0__p2_0
   STA   __local_f0__p0_0
   LDA   #$00
   CMP   __zpabi_f0__p1_1
   BNE   .cmp_end@17
   LDA   __local_f0__1
   CMP   __local_f0__p0_0
   BEQ   .cmp_true@16
   LDA   #$00
   BEQ   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   STA   __local_f0__p0_1
   LDA   #$00
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f0__p0_1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

main:
   SUBROUTINE

   LDA   #$C6
   STA   __zpabi_f0__p1_0
   LDA   #$A7
   STA   __zpabi_f0__p2_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p2_1
   STA   __zpabi_f0__p2_2
   STA   __zpabi_f0__p2_3
   LDX   #$00
   LDA   #$B2
   JSR   f0
   LDA   HARGS+8
   EOR   g0
   STA   __local_main__3
   LDA   HARGS+9
   STA   __local_main__2
   STA   HARGS+1
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
   STA   HARGS+3
   LDA   __local_main__3
   STA   HARGS
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   __local_main__3
   EOR   HARGS+5
   STA   __local_main__0
   LDA   __local_main__2
   EOR   HARGS+6
   TAX
   LDA   __local_main__0
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$11
   STA   g0
   RTS
