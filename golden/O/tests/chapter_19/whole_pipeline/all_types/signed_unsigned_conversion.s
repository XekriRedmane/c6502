SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_int_to_uint params= locals= indirect=false in_cycle=false
; def target_long_to_ulong params= locals= indirect=false in_cycle=false
; def target_uint_to_int params= locals= indirect=false in_cycle=false
; def target_ulong_to_long params= locals= indirect=false in_cycle=false
; call main -> target_int_to_uint
; call main -> target_long_to_ulong
; call main -> target_uint_to_int
; call main -> target_ulong_to_long
; item target_int_to_uint section=.text local=false zero=false
; item target_uint_to_int section=.text local=false zero=false
; item target_ulong_to_long section=.text local=false zero=false
; item target_long_to_ulong section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

target_int_to_uint:
   SUBROUTINE

   LDX   #$19
   LDA   #$99
   RTS

target_uint_to_int:
   SUBROUTINE

   LDX   #$FF
   LDA   #$FE
   RTS

target_ulong_to_long:
   SUBROUTINE

   LDA   #$17
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

target_long_to_ulong:
   SUBROUTINE

   LDA   #$85
   STA   HARGS+8
   LDA   #$99
   STA   HARGS+9
   STA   HARGS+10
   LDA   #$19
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   JSR   target_int_to_uint
   STA   __local_main__1
   LDA   #$00
   CMP   #$19
   BNE   .jcmp_skip@0
   CMP   #$99
   BNE   .jcmp_skip@0
   CPX   #$99
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$99
   BEQ   .if_end@2
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   JSR   target_uint_to_int
   CPX   #$FF
   BNE   .jcmp_skip@1
   CMP   #$FE
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@3:
   JSR   target_ulong_to_long
   LDX   #$00
   LDA   #$03
   RTS
