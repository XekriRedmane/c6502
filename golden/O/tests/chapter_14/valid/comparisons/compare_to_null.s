SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__3	EQU	$83
__local_main__x	EQU	$84

; @zp-link-meta-begin
; def get_null_pointer params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__x,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; call main -> get_null_pointer
; item get_null_pointer section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

get_null_pointer:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   JSR   get_null_pointer
   STA   __local_main__3
   LDA   #<__local_main__x
   ORA   #>__local_main__x
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__3
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #<__local_main__x
   ORA   #>__local_main__x
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__3
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS
