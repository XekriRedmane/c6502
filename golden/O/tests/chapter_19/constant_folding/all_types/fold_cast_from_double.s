SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_implicit params= locals= indirect=false in_cycle=false
; def target_to_char params= locals= indirect=false in_cycle=false
; def target_to_int params= locals= indirect=false in_cycle=false
; def target_to_long params= locals= indirect=false in_cycle=false
; def target_to_uchar params= locals= indirect=false in_cycle=false
; def target_to_uint params= locals= indirect=false in_cycle=false
; def target_to_ulong params= locals= indirect=false in_cycle=false
; call main -> target_implicit
; call main -> target_to_char
; call main -> target_to_int
; call main -> target_to_long
; call main -> target_to_uchar
; call main -> target_to_uint
; call main -> target_to_ulong
; item target_to_char section=.text local=false zero=false
; item target_to_uchar section=.text local=false zero=false
; item target_to_int section=.text local=false zero=false
; item target_to_uint section=.text local=false zero=false
; item target_to_long section=.text local=false zero=false
; item target_to_ulong section=.text local=false zero=false
; item target_implicit section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

target_to_char:
   SUBROUTINE

   LDA   #$7E
   RTS

target_to_uchar:
   SUBROUTINE

   LDA   #$FE
   RTS

target_to_int:
   SUBROUTINE

   LDX   #$00
   LDA   #$05
   RTS

target_to_uint:
   SUBROUTINE

   LDX   #$00
   LDA   #$66
   RTS

target_to_long:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+8
   LDA   #$FC
   STA   HARGS+9
   LDA   #$FF
   STA   HARGS+10
   STA   HARGS+11
   RTS

target_to_ulong:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STA   HARGS+11
   RTS

target_implicit:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+8
   STA   HARGS+9
   LDY   #$10
   STY   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   JSR   target_to_char
   CMP   #$7E
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   target_to_uchar
   CMP   #$FE
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   target_to_int
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$05
   BEQ   .if_end@2
.jcmp_skip@0:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   target_to_uint
   STA   __local_main__1
   LDA   #$00
   CMP   #$80
   BNE   .jcmp_skip@1
   CMP   #$00
   BNE   .jcmp_skip@1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$66
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JSR   target_to_long
   LDX   #$00
   LDA   #$05
   RTS
