SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__4	EQU	$84
__local_main__7	EQU	$87

; @zp-link-meta-begin
; def implicit params= locals= indirect=false in_cycle=false
; def int_to_uint params= locals= indirect=false in_cycle=false
; def long_to_ulong params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_int_to_long params= locals= indirect=false in_cycle=false
; def target_int_to_ulong params= locals= indirect=false in_cycle=false
; def uint_to_int params= locals= indirect=false in_cycle=false
; def uint_to_long params= locals= indirect=false in_cycle=false
; def uint_to_ulong params= locals= indirect=false in_cycle=false
; def ulong_to_long params= locals= indirect=false in_cycle=false
; call main -> implicit
; call main -> int_to_uint
; call main -> long_to_ulong
; call main -> target_int_to_long
; call main -> target_int_to_ulong
; call main -> uint_to_int
; call main -> uint_to_long
; call main -> uint_to_ulong
; call main -> ulong_to_long
; item uint_to_long section=.text local=false zero=false
; item uint_to_long@tail0 section=.text local=true zero=false
; item uint_to_long@tail0@run0 section=.text local=true zero=false
; item uint_to_ulong section=.text local=false zero=false
; item target_int_to_ulong section=.text local=false zero=false
; item target_int_to_long section=.text local=false zero=false
; item uint_to_int section=.text local=false zero=false
; item int_to_uint section=.text local=false zero=false
; item ulong_to_long section=.text local=false zero=false
; item long_to_ulong section=.text local=false zero=false
; item implicit section=.text local=false zero=false
; item main section=.text local=false zero=false
; item one section=.data local=false zero=false
; item ninety_six section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

uint_to_long:
   SUBROUTINE

   JMP   uint_to_long@tail0

uint_to_long@tail0:
   SUBROUTINE

   JSR   uint_to_long@tail0@run0
   STY   HARGS+10
   STA   HARGS+11
   RTS

uint_to_long@tail0@run0:
   SUBROUTINE

   LDA   #$FF
   STA   HARGS+8
   LDY   #$FF
   STY   HARGS+9
   RTS

uint_to_ulong:
   SUBROUTINE

   JMP   uint_to_long@tail0

target_int_to_ulong:
   SUBROUTINE

   JSR   uint_to_long@tail0@run0
   STA   HARGS+10
   LDA   #$7F
   STA   HARGS+11
   RTS

target_int_to_long:
   SUBROUTINE

   LDA   #$01
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

uint_to_int:
   SUBROUTINE

   LDX   #$FF
   LDA   #$A0
   RTS

int_to_uint:
   SUBROUTINE

   LDX   #$F1
   LDA   #$C0
   RTS

ulong_to_long:
   SUBROUTINE

   JMP   uint_to_long@tail0

long_to_ulong:
   SUBROUTINE

   LDA   #$02
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   STA   HARGS+10
   LDA   #$80
   STA   HARGS+11
   RTS

implicit:
   SUBROUTINE

   JMP   uint_to_long@tail0

main:
   SUBROUTINE

   JSR   uint_to_long
   LDA   HARGS+11
   BMI   .jcmp_skip@2
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   HARGS+8
   CMP   #$FF
   BEQ   .if_end@0
.jcmp_skip@2:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   uint_to_ulong
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   HARGS+8
   CMP   #$FF
   BEQ   .if_end@1
.jcmp_skip@3:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   target_int_to_ulong
   LDA   HARGS+11
   CMP   #$7F
   BNE   .jcmp_skip@4
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   HARGS+8
   CMP   #$FF
   BEQ   .if_end@2
.jcmp_skip@4:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   target_int_to_long
   LDA   HARGS+11
   BNE   .jcmp_skip@5
   LDA   HARGS+10
   BNE   .jcmp_skip@5
   LDA   HARGS+9
   BNE   .jcmp_skip@5
   LDA   HARGS+8
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@5:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JSR   uint_to_int
   STA   __local_main__7
   TXA
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_main__4
   SEC
   LDA   #$00
   SBC   ninety_six
   STA   __local_main__3
   LDA   #$00
   SBC   ninety_six+1
   STA   __local_main__2
   LDA   #$00
   SBC   ninety_six+2
   STA   __local_main__1
   LDA   #$00
   SBC   ninety_six+3
   STA   __local_main__0
   LDA   __local_main__4
   CMP   __local_main__0
   BNE   .jcmp_skip@8
   CMP   __local_main__1
   BNE   .jcmp_skip@8
   CPX   __local_main__2
   BNE   .jcmp_skip@8
   LDA   __local_main__7
   CMP   __local_main__3
   BEQ   .if_end@4
.jcmp_skip@8:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   JSR   int_to_uint
   STA   __local_main__1
   LDA   #$00
   CMP   #$7F
   BNE   .jcmp_skip@9
   CMP   #$FF
   BNE   .jcmp_skip@9
   CPX   #$F1
   BNE   .jcmp_skip@9
   LDA   __local_main__1
   CMP   #$C0
   BEQ   .if_end@5
.jcmp_skip@9:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   JSR   ulong_to_long
   SEC
   LDA   #$00
   SBC   one
   STA   __local_main__3
   LDA   #$00
   SBC   one+1
   STA   __local_main__2
   LDA   #$00
   SBC   one+2
   STA   __local_main__1
   LDA   #$00
   SBC   one+3
   STA   __local_main__0
   LDA   HARGS+11
   CMP   __local_main__0
   BNE   .jcmp_skip@10
   LDA   HARGS+10
   CMP   __local_main__1
   BNE   .jcmp_skip@10
   LDA   HARGS+9
   CMP   __local_main__2
   BNE   .jcmp_skip@10
   LDA   HARGS+8
   CMP   __local_main__3
   BEQ   .if_end@6
.jcmp_skip@10:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   JSR   long_to_ulong
   LDA   HARGS+11
   CMP   #$80
   BNE   .jcmp_skip@11
   LDA   HARGS+10
   BNE   .jcmp_skip@11
   LDA   HARGS+9
   BNE   .jcmp_skip@11
   LDA   HARGS+8
   CMP   #$02
   BEQ   .if_end@7
.jcmp_skip@11:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   JSR   implicit
   SEC
   LDA   #$00
   SBC   one
   STA   __local_main__3
   LDA   #$00
   SBC   one+1
   STA   __local_main__2
   LDA   #$00
   SBC   one+2
   STA   __local_main__1
   LDA   #$00
   SBC   one+3
   STA   __local_main__0
   LDA   HARGS+11
   CMP   __local_main__0
   BNE   .jcmp_skip@12
   LDA   HARGS+10
   CMP   __local_main__1
   BNE   .jcmp_skip@12
   LDA   HARGS+9
   CMP   __local_main__2
   BNE   .jcmp_skip@12
   LDA   HARGS+8
   CMP   __local_main__3
   BEQ   .if_end@8
.jcmp_skip@12:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDX   #$00
   TXA
   RTS

one:
   DC.L  $00000001

ninety_six:
   DC.L  $00000060
