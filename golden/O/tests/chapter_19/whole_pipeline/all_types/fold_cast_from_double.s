SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_to_char params= locals= indirect=false in_cycle=false
; def target_to_int params= locals= indirect=false in_cycle=false
; def target_to_long params= locals= indirect=false in_cycle=false
; call main -> target_to_char
; call main -> target_to_int
; call main -> target_to_long
; item target_to_char section=.text local=false zero=false
; item target_to_int section=.text local=false zero=false
; item target_to_long section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

target_to_char:
   SUBROUTINE

   LDA   #$82
   RTS

target_to_int:
   SUBROUTINE

   LDX   #$FF
   LDA   #$FB
   RTS

target_to_long:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+8
   LDY   #$04
   STY   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   JSR   target_to_char
   STA   __local_main__0
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_main__0
   CMP   #$82
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   target_to_int
   CPX   #$FF
   BNE   .jcmp_skip@1
   CMP   #$FB
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   target_to_long
   LDX   #$00
   LDA   #$03
   RTS
