SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def addr_of_arg params=__zpabi_addr_of_arg__a_0,__zpabi_addr_of_arg__a_1 locals=__local_addr_of_arg__0,__local_addr_of_arg__1,__local_addr_of_arg__a,__local_addr_of_arg__3 indirect=false in_cycle=false param_regs=A,X changes=__zpabi_addr_of_arg__a_0,__zpabi_addr_of_arg__a_1
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; call main -> addr_of_arg
; item addr_of_arg section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

addr_of_arg:
   SUBROUTINE

   LDX   #$00
   LDA   #$0A
   RTS

main:
   SUBROUTINE

   LDX   #$FF
   LDA   #$EC
   JSR   addr_of_arg
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #$00
   LDA   #$64
   JSR   addr_of_arg
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS
