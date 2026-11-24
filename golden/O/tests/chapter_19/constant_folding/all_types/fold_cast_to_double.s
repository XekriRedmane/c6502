SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_from_int params= locals= indirect=false in_cycle=false
; def target_from_long params= locals= indirect=false in_cycle=false
; def target_from_uint params= locals= indirect=false in_cycle=false
; def target_from_ulong params= locals= indirect=false in_cycle=false
; def target_implicit params= locals= indirect=false in_cycle=false
; call main -> target_from_int
; call main -> target_from_long
; call main -> target_from_uint
; call main -> target_from_ulong
; call main -> target_implicit
; item target_from_int section=.text local=false zero=false
; item target_from_int@tail0 section=.text local=true zero=false
; item target_from_int@tail0@run0 section=.text local=true zero=false
; item target_from_uint section=.text local=false zero=false
; item target_from_long section=.text local=false zero=false
; item target_from_ulong section=.text local=false zero=false
; item target_implicit section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target_from_int:
   SUBROUTINE

   JMP   target_from_int@tail0

target_from_int@tail0:
   SUBROUTINE

   JSR   target_from_int@tail0@run0
   STY   HARGS+19
   STA   HARGS+20
   LDA   #$40
   STA   HARGS+21
   LDY   #$8F
   STY   HARGS+22
   STA   HARGS+23
   RTS

target_from_int@tail0@run0:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   RTS

target_from_uint:
   SUBROUTINE

   JSR   target_from_int@tail0@run0
   STA   HARGS+19
   LDA   #$FF
   STA   HARGS+21
   LDA   #$EF
   STA   HARGS+22
   LDA   #$40
   STA   HARGS+20
   STA   HARGS+23
   RTS

target_from_long:
   SUBROUTINE

   JSR   target_from_int@tail0@run0
   STY   HARGS+19
   STY   HARGS+20
   STA   HARGS+21
   LDA   #$D0
   STA   HARGS+22
   LDA   #$43
   STA   HARGS+23
   RTS

target_from_ulong:
   SUBROUTINE

   JSR   target_from_int@tail0@run0
   STY   HARGS+19
   STA   HARGS+20
   LDA   #$04
   STA   HARGS+21
   LDA   #$90
   STA   HARGS+22
   LDA   #$40
   STA   HARGS+23
   RTS

target_implicit:
   SUBROUTINE

   JMP   target_from_int@tail0

main:
   SUBROUTINE

   JSR   target_from_int
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   HARGS+22
   CMP   #$8F
   BNE   .jcmp_skip@0
   LDA   HARGS+21
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   HARGS+20
   BNE   .jcmp_skip@0
   LDA   HARGS+19
   BNE   .jcmp_skip@0
   LDA   HARGS+18
   BNE   .jcmp_skip@0
   LDA   HARGS+17
   BNE   .jcmp_skip@0
   LDA   HARGS+16
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   target_from_uint
   LDA   HARGS+23
   CMP   #$41
   BNE   .jcmp_skip@1
   LDA   HARGS+22
   CMP   #$EF
   BNE   .jcmp_skip@1
   LDA   HARGS+21
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   HARGS+20
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   HARGS+19
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   HARGS+18
   CMP   #$40
   BNE   .jcmp_skip@1
   LDA   HARGS+17
   BNE   .jcmp_skip@1
   LDA   HARGS+16
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   target_from_long
   LDA   HARGS+23
   CMP   #$43
   BNE   .jcmp_skip@2
   LDA   HARGS+22
   CMP   #$D0
   BNE   .jcmp_skip@2
   LDA   HARGS+21
   BNE   .jcmp_skip@2
   LDA   HARGS+20
   BNE   .jcmp_skip@2
   LDA   HARGS+19
   BNE   .jcmp_skip@2
   LDA   HARGS+18
   BNE   .jcmp_skip@2
   LDA   HARGS+17
   BNE   .jcmp_skip@2
   LDA   HARGS+16
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   target_from_ulong
   LDA   HARGS+23
   CMP   #$43
   BNE   .jcmp_skip@3
   LDA   HARGS+22
   CMP   #$E0
   BNE   .jcmp_skip@3
   LDA   HARGS+21
   BNE   .jcmp_skip@3
   LDA   HARGS+20
   BNE   .jcmp_skip@3
   LDA   HARGS+19
   BNE   .jcmp_skip@3
   LDA   HARGS+18
   BNE   .jcmp_skip@3
   LDA   HARGS+17
   BNE   .jcmp_skip@3
   LDA   HARGS+16
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JSR   target_implicit
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@4
   LDA   HARGS+22
   CMP   #$8F
   BNE   .jcmp_skip@4
   LDA   HARGS+21
   CMP   #$40
   BNE   .jcmp_skip@4
   LDA   HARGS+20
   BNE   .jcmp_skip@4
   LDA   HARGS+19
   BNE   .jcmp_skip@4
   LDA   HARGS+18
   BNE   .jcmp_skip@4
   LDA   HARGS+17
   BNE   .jcmp_skip@4
   LDA   HARGS+16
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS
