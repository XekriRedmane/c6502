SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def f params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; call main -> f
; item f section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @0.i section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

f:
   SUBROUTINE

   LDA   @0.i+1
   BNE   .if_end@0
   LDA   @0.i
   CMP   #$05
   BNE   .if_end@0
   LDX   #$00
   TXA
   RTS
.if_end@0:
   LDA   #$05
   STA   @0.i
   LDA   #$00
   STA   @0.i+1
   LDX   #$00
   LDA   #$01
   RTS

main:
   SUBROUTINE

   JSR   f
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   JSR   f
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS

@0.i:
   DC.W  $000A
