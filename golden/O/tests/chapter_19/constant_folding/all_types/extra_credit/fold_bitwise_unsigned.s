SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_target_shift_uint_left__0	EQU	$84
__local_target_shift_uint_left__1	EQU	$85

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; def target_and params= locals= indirect=false in_cycle=false
; def target_or params= locals= indirect=false in_cycle=false
; def target_shift_uint_left params= locals=__local_target_shift_uint_left__0,__local_target_shift_uint_left__1 indirect=false in_cycle=false
; def target_shift_uint_right params= locals= indirect=false in_cycle=false
; def target_shift_ulong_left params= locals=__local_target_shift_ulong_left__0 indirect=false in_cycle=false
; def target_shift_ulong_right params= locals= indirect=false in_cycle=false
; def target_xor params= locals= indirect=false in_cycle=false
; call main -> target_and
; call main -> target_or
; call main -> target_shift_uint_left
; call main -> target_shift_uint_right
; call main -> target_shift_ulong_left
; call main -> target_shift_ulong_right
; call main -> target_xor
; item target_and section=.text local=false zero=false
; item target_or section=.text local=false zero=false
; item target_xor section=.text local=false zero=false
; item target_shift_uint_left section=.text local=false zero=false
; item target_shift_ulong_left section=.text local=false zero=false
; item target_shift_uint_right section=.text local=false zero=false
; item target_shift_ulong_right section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

target_and:
   SUBROUTINE

   LDX   #$F0
   LDA   #$00
   RTS

target_or:
   SUBROUTINE

   LDA   #$0F
   STA   HARGS+8
   LDY   #$FF
   STY   HARGS+9
   STA   HARGS+10
   STY   HARGS+11
   RTS

target_xor:
   SUBROUTINE

   LDX   #$FF
   LDA   #$00
   RTS

target_shift_uint_left:
   SUBROUTINE

   LDA   #$0A
   STA   __local_target_shift_uint_left__0
   LDA   #$00
   STA   __local_target_shift_uint_left__1
   LDX   #$18
.shift_loop@0:
   ASL   __local_target_shift_uint_left__0
   ROL   __local_target_shift_uint_left__1
   DEX
   BNE   .shift_loop@0
   LDX   __local_target_shift_uint_left__1
   LDA   __local_target_shift_uint_left__0
   RTS

target_shift_ulong_left:
   SUBROUTINE

   LDA   #$47
   STA   HARGS
   LDA   #$67
   STA   HARGS+1
   LDA   #$45
   STA   HARGS+2
   LDA   #$88
   STA   HARGS+3
   LDA   #$21
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+6
   STA   HARGS+9
   LDA   HARGS+7
   STA   HARGS+10
   LDA   HARGS+8
   STA   HARGS+11
   LDA   HARGS+5
   STA   HARGS+8
   RTS

target_shift_uint_right:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_shift_ulong_right:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+8
   LDY   #$08
   STY   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   JSR   target_and
   STA   __local_main__1
   LDA   #$00
   CMP   #$F0
   BNE   .jcmp_skip@2
   CMP   #$00
   BNE   .jcmp_skip@2
   CPX   #$F0
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   BEQ   .if_end@0
.jcmp_skip@2:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   target_or
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@3
   CMP   #$0F
   BNE   .jcmp_skip@3
   CMP   #$FF
   BNE   .jcmp_skip@3
   CMP   #$0F
   BNE   .jcmp_skip@3
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   HARGS+10
   CMP   #$0F
   BNE   .jcmp_skip@3
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   HARGS+8
   CMP   #$0F
   BEQ   .if_end@1
.jcmp_skip@3:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   target_xor
   STA   __local_main__1
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@4
   CMP   #$00
   BNE   .jcmp_skip@4
   CPX   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_main__1
   BEQ   .if_end@2
.jcmp_skip@4:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   target_shift_uint_left
   STA   __local_main__1
   LDA   #$00
   CMP   #$0A
   BNE   .jcmp_skip@5
   CMP   #$00
   BNE   .jcmp_skip@5
   TXA
   BNE   .jcmp_skip@5
   LDA   __local_main__1
   BEQ   .if_end@3
.jcmp_skip@5:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JSR   target_shift_ulong_left
   LDA   #$00
   CMP   #$10
   BNE   .jcmp_skip@6
   CMP   #$8A
   BNE   .jcmp_skip@6
   CMP   #$CE
   BNE   .jcmp_skip@6
   CMP   #$8E
   BNE   .jcmp_skip@6
   LDA   HARGS+11
   BNE   .jcmp_skip@6
   LDA   HARGS+10
   BNE   .jcmp_skip@6
   LDA   HARGS+9
   BNE   .jcmp_skip@6
   LDA   HARGS+8
   BEQ   .if_end@4
.jcmp_skip@6:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   JSR   target_shift_uint_right
   STA   __local_main__1
   LDA   #$00
   CMP   #$01
   BNE   .jcmp_skip@7
   TXA
   BNE   .jcmp_skip@7
   LDA   __local_main__1
   BEQ   .if_end@5
.jcmp_skip@7:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   JSR   target_shift_ulong_right
   LDA   #$00
   CMP   #$04
   BNE   .jcmp_skip@8
   CMP   #$00
   BNE   .jcmp_skip@8
   LDA   HARGS+11
   BNE   .jcmp_skip@8
   LDA   HARGS+10
   BNE   .jcmp_skip@8
   LDA   HARGS+9
   CMP   #$08
   BNE   .jcmp_skip@8
   LDA   HARGS+8
   BEQ   .if_end@6
.jcmp_skip@8:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDX   #$00
   TXA
   RTS
